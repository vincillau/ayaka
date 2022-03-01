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

#ifndef AYAKA_SRC_TCP_HPP_
#define AYAKA_SRC_TCP_HPP_

#include <gtest/gtest.h>
#include <uv.h>

#include <functional>
#include <memory>
#include <unordered_map>

#include "error.hpp"
#include "inet_addr.hpp"
#include "loop.hpp"
#include "send_pack.hpp"

namespace ayaka {

/**
 * 因为实现原因，Tcp 对象只能在同一个线程中使用。
 */
class Tcp {
 public:
  using OnAcceptCb = std::function<void(const std::shared_ptr<Tcp> &)>;
  using OnCloseCb = std::function<void()>;
  using OnRecvCb = std::function<void(const char *, size_t)>;

  /**
   * 表示已关闭的 Tcp 对象，不应该被使用。
   */
  Tcp() = default;

  explicit Tcp(const std::shared_ptr<Loop> &loop);

  /**
   * Tcp 只可以移动，不可以拷贝。
   */
  Tcp(const Tcp &) = delete;
  Tcp &operator=(const Tcp &) = delete;

  Tcp(Tcp &&other) noexcept;
  Tcp &operator=(Tcp &&other) noexcept;

  ~Tcp();

  [[nodiscard]] auto &on_accept() const { return on_accept_; }
  void set_on_accept(OnAcceptCb on_accept) {
    on_accept_ = std::move(on_accept);
  }

  /**
   * on_close() 在 uv_tcp_ 关闭后调用，用于释放相关资源。
   */
  [[nodiscard]] auto &on_close() const { return on_close_; }
  void set_on_close(OnCloseCb on_close) { on_close_ = std::move(on_close); }

  [[nodiscard]] auto &on_recv() const { return on_recv_; }
  void set_on_recv(OnRecvCb on_recv) { on_recv_ = std::move(on_recv); }

  [[nodiscard]] auto closed() const { return closed_; }

  [[nodiscard]] auto Pending() const { return send_packs_.size(); }

  void Bind(const InetAddr &addr);
  void Listen();
  void Send(const std::shared_ptr<SendPack> &send_pack,
            std::function<void()> on_finish);
  /**
   * Close 可以重复调用，但是不会有任何效果。
   */
  void Close();

 private:
  void Open();
  void ReuseAddr();
  [[nodiscard]] std::shared_ptr<Tcp> Accept();
  void OnSendFinish(const std::shared_ptr<SendPack> &send_pack, int status);

  static void OnAlloc(uv_handle_t *handle, size_t suggested_size,
                      uv_buf_t *buf);
  static void OnClose(uv_handle_t *handle);
  static void OnConn(uv_stream_t *server, int status);
  static void OnRecv(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);

  static constexpr int kDefaultBacklog = 511;
  static constexpr int kDefaultBufSize = 1024;

  uv_tcp_t *uv_tcp_ = nullptr;
  std::shared_ptr<Loop> loop_;
  std::unordered_map<std::shared_ptr<SendPack>, std::function<void()>>
      send_packs_;
  OnAcceptCb on_accept_ = [](const std::shared_ptr<Tcp> &) {};
  OnCloseCb on_close_ = []() {};
  OnRecvCb on_recv_ = [](const char *, size_t) {};
  bool closed_ = true;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_TCP_HPP_
