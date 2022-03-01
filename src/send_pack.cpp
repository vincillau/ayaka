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

#include "send_pack.hpp"

#include <unordered_map>
#include <utility>

namespace ayaka {

namespace {

thread_local std::unordered_map<uv_write_t*, SendPack*> gSendPackMap;

}  // namespace

SendPack::SendPack(SendPack&& other) noexcept
    : uv_write_(other.uv_write_),
      bufs_(std::move(other.bufs_)),
      on_finish_(std::move(other.on_finish_)) {
  other.uv_write_ = nullptr;
}

SendPack& SendPack::operator=(SendPack&& other) noexcept {
  if (this != &other) {
    delete uv_write_;
    uv_write_ = other.uv_write_;
    other.uv_write_ = nullptr;
    bufs_ = std::move(other.bufs_);
    on_finish_ = std::move(other.on_finish_);
  }
  return *this;
}

void SendPack::UvOnWrite(uv_write_t* req, int status) {
  auto* send_pack = gSendPackMap.at(req);
  send_pack->on_finish_(status);
}

SendRespPack::SendRespPack() { gSendPackMap[uv_write_] = this; }

SendRespPack::SendRespPack(std::shared_ptr<Response> resp)
    : resp_(std::move(resp)) {
  uv_write_ = new uv_write_t;
  gSendPackMap[uv_write_] = this;
  SetUpBufs();
}

SendRespPack::~SendRespPack() { gSendPackMap.erase(uv_write_); }

void SendRespPack::SetUpBufs() {
  AddVersionBufs();
  AddStatusCodeBufs();
  AddStatusMsgBufs();
  AddHeadersBufs();

  // HTTP headers 后要加一个空行。
  uv_buf_t buf;
  buf.base = const_cast<char*>("\r\n");
  buf.len = 2;
  bufs_.push_back(buf);

  AddBodyBufs();
}

void SendRespPack::AddVersionBufs() {
  uv_buf_t buf;
  buf.base = const_cast<char*>(resp_->version().c_str());
  buf.len = resp_->version().size();
  bufs_.push_back(buf);
  buf.base = const_cast<char*>(" ");
  buf.len = 1;
  bufs_.push_back(buf);
}

void SendRespPack::AddStatusCodeBufs() {
  uv_buf_t buf;
  buf.base = const_cast<char*>(resp_->status().code().c_str());
  buf.len = resp_->status().code().size();
  bufs_.push_back(buf);
  buf.base = const_cast<char*>(" ");
  buf.len = 1;
  bufs_.push_back(buf);
}

void SendRespPack::AddStatusMsgBufs() {
  uv_buf_t buf;
  buf.base = const_cast<char*>(resp_->status().msg().c_str());
  buf.len = resp_->status().msg().size();
  bufs_.push_back(buf);
  buf.base = const_cast<char*>("\r\n");
  buf.len = 2;
  bufs_.push_back(buf);
}

void SendRespPack::AddHeadersBufs() {
  for (const auto& header : resp_->headers()) {
    uv_buf_t buf;
    buf.base = const_cast<char*>(header.first.c_str());
    buf.len = header.first.size();
    bufs_.push_back(buf);
    buf.base = const_cast<char*>(": ");
    buf.len = 2;
    bufs_.push_back(buf);
    buf.base = const_cast<char*>(header.second.c_str());
    buf.len = header.second.size();
    bufs_.push_back(buf);
    buf.base = const_cast<char*>("\r\n");
    buf.len = 2;
    bufs_.push_back(buf);
  }
}

void SendRespPack::AddBodyBufs() {
  if (!resp_->body() || resp_->body()->empty()) {
    return;
  }

  uv_buf_t buf;
  buf.base = const_cast<char*>(resp_->body()->data());
  buf.len = resp_->body()->size();
  bufs_.push_back(buf);
}

}  // namespace ayaka
