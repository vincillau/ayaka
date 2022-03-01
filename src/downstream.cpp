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

#include "downstream.hpp"

#include <iostream>
#include <memory>
#include <string>

namespace ayaka {

Downstream::Downstream(std::shared_ptr<Tcp> tcp, std::shared_ptr<Router> router)
    : tcp_(std::move(tcp)), router_(std::move(router)) {
  tcp_->set_on_recv([this](const char* buf, size_t len) { OnRecv(buf, len); });
}

void Downstream::OnRecv(const char* buf, size_t len) {
  try {
    HandleRecv(buf, len);
  } catch (const HttpExcept& except) {
    auto resp = Response::Default();
    except.SetUp(resp);
    Send(resp);
  }
  // 由 Tcp 负责释放 buf.base。
}

void Downstream::HandleRecv(const char* buf, size_t len) {
  const auto* base = buf;
  while (len > 0) {
    // 可能会抛出 Http400Except。
    auto nparsed = parser_.Exec(base, len);
    base += nparsed;
    len -= nparsed;
    if (parser_.state() == RequestParser::State::kDone) {
      auto req = parser_.req();
      auto resp = Response::Default();
      auto handler = router_->Route(req->url().path().string());
      handler->Handle(req, resp);
      AYAKA_LOG_INFO("{} {} {}", req->method(), req->url().src(),
                     resp->status().code());
      Send(resp);
    }
  }
}

void Downstream::Send(std::shared_ptr<Response> resp) {
  std::shared_ptr<SendPack> send_pack = std::make_shared<SendRespPack>(resp);
  tcp_->Send(send_pack, [this]() { tcp_->Close(); });
}

}  // namespace ayaka
