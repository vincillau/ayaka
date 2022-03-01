/**
 * Copyright (C) 2022 Vincil Lau.
 *
 * Ayaka is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ayaka is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Ayaka. If not, see <https://www.gnu.org/licenses/>.
 */

#include "tcp.hpp"

#include <sys/socket.h>

#include <utility>

#include "uv.h"

namespace ayaka {

namespace {

thread_local std::unordered_map<uv_tcp_t *, Tcp *> gTcpMap;

}  // namespace

Tcp::Tcp(const std::shared_ptr<Loop> &loop)
    : uv_tcp_(new uv_tcp_t), loop_(loop), closed_(false) {
  auto status = uv_tcp_init(loop->uv_loop(), uv_tcp_);
  AYAKA_TERM_IF(status != 0, "uv_tcp_init failed: {}", UvLastError(status));
  gTcpMap[uv_tcp_] = this;
}

Tcp::Tcp(Tcp &&other) noexcept
    : uv_tcp_(other.uv_tcp_),
      loop_(std::move(other.loop_)),
      on_accept_(std::move(other.on_accept_)),
      on_close_(std::move(other.on_close_)),
      on_recv_(std::move(other.on_recv_)),
      closed_(other.closed_) {
  other.uv_tcp_ = nullptr;
  other.closed_ = true;
}

Tcp &Tcp::operator=(Tcp &&other) noexcept {
  if (this != &other) {
    uv_tcp_ = other.uv_tcp_;
    loop_ = std::move(other.loop_);
    on_accept_ = std::move(other.on_accept_);
    on_close_ = std::move(other.on_close_);
    on_recv_ = std::move(other.on_recv_);
    closed_ = other.closed_;

    other.uv_tcp_ = nullptr;
    other.closed_ = true;
  }
  return *this;
}

Tcp::~Tcp() {
  AYAKA_TERM_IF(!closed_, "Tcp must be closed before destruction");
  gTcpMap.erase(uv_tcp_);
}

void Tcp::Bind(const InetAddr &addr) {
  Open();
  ReuseAddr();

  sockaddr sock_addr{};
  addr.ToSockAddr(&sock_addr);
  auto status = uv_tcp_bind(uv_tcp_, &sock_addr, 0);
  AYAKA_TERM_IF(status != 0, "uv_tcp_bind failed: {}", UvLastError(status));
  AYAKA_LOG_INFO("bind on {}:{}", addr.ip(), addr.port());
}

void Tcp::Listen() {
  auto status = uv_listen(reinterpret_cast<uv_stream_t *>(uv_tcp_),
                          kDefaultBacklog, OnConn);
  AYAKA_TERM_IF(status != 0, "uv_listen failed: {}", UvLastError(status));
}

void Tcp::Send(const std::shared_ptr<SendPack> &send_pack,
               std::function<void()> on_finish) {
  send_packs_[send_pack] = std::move(on_finish);
  send_pack->set_on_finish(
      [this, send_pack](int status) { OnSendFinish(send_pack, status); });
  auto status = uv_write(
      send_pack->uv_write(), reinterpret_cast<uv_stream_t *>(uv_tcp_),
      static_cast<const std::shared_ptr<SendPack> &>(send_pack)->bufs().data(),
      send_pack->bufs().size(), SendPack::UvOnWrite);
  AYAKA_TERM_IF(status != 0, "uv_write failed: {}", UvLastError(status));
}

std::shared_ptr<Tcp> Tcp::Accept() {
  auto client = std::make_shared<Tcp>(loop_);
  auto status = uv_accept(reinterpret_cast<uv_stream_t *>(uv_tcp_),
                          reinterpret_cast<uv_stream_t *>(client->uv_tcp_));
  AYAKA_TERM_IF(status != 0, "uv_accept failed: {}", UvLastError(status));
  return client;
}

void Tcp::Close() {
  if (closed_ ||
      (uv_is_closing(reinterpret_cast<uv_handle_t *>(uv_tcp_)) != 0)) {
    return;
  }
  uv_close(reinterpret_cast<uv_handle_t *>(uv_tcp_), OnClose);
}

void Tcp::Open() {
  auto socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  AYAKA_TERM_IF(socket_fd == -1, "socket failed: {}", LastError());
  auto status = uv_tcp_open(uv_tcp_, socket_fd);
  AYAKA_TERM_IF(status != 0, "uv_tcp_open failed: {}", UvLastError(status));
}

void Tcp::ReuseAddr() {
  int socket_fd = -1;
  auto status = uv_fileno(reinterpret_cast<uv_handle_t *>(uv_tcp_), &socket_fd);
  AYAKA_TERM_IF(status != 0, "uv_fileno failed: {}", UvLastError(status));

  int val = 1;
  status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));
  AYAKA_TERM_IF(status != 0, "setsockopt failed: {}", LastError());
}

void Tcp::OnSendFinish(const std::shared_ptr<SendPack> &send_pack, int status) {
  if (status != 0) {
    AYAKA_LOG_CRITICAL("uv_write failed: {}", UvLastError(status));
    Close();
    return;
  }

  auto on_finish = send_packs_.at(send_pack);
  if (on_finish) {
    on_finish();
  }
  send_packs_.erase(send_pack);
}

void Tcp::OnAlloc(uv_handle_t * /*handle*/, size_t /*suggested_size*/,
                  uv_buf_t *buf) {
  buf->base = new char[kDefaultBufSize];
  buf->len = kDefaultBufSize;
}

void Tcp::OnConn(uv_stream_t *server, int status) {
  auto *listener = gTcpMap.at(reinterpret_cast<uv_tcp_t *>(server));
  auto client = listener->Accept();
  status = uv_read_start(reinterpret_cast<uv_stream_t *>(client->uv_tcp_),
                         OnAlloc, OnRecv);
  AYAKA_TERM_IF(status != 0, "uv_read_start failed: {}", UvLastError(status));
  listener->on_accept_(client);
}

void Tcp::OnClose(uv_handle_t *handle) {
  auto *listener = gTcpMap.at(reinterpret_cast<uv_tcp_t *>(handle));
  listener->closed_ = true;
  listener->on_close_();
}

void Tcp::OnRecv(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
  if (nread == 0) {
    return;
  }
  auto *tcp = gTcpMap.at(reinterpret_cast<uv_tcp_t *>(stream));
  if (nread == UV_EOF) {
    tcp->Close();
    return;
  }
  AYAKA_TERM_IF(nread < 0, "uv_tcp_read failed: {}", uv_strerror(nread));
  tcp->on_recv_(buf->base, nread);
  delete[] buf->base;
}

}  // namespace ayaka
