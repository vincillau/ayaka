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

#ifndef AYAKA_SRC_HTTP_HANDLER_HPP_
#define AYAKA_SRC_HTTP_HANDLER_HPP_

#include <memory>

#include "http_except.hpp"
#include "request.hpp"
#include "response.hpp"

namespace ayaka {

class HttpHandler {
 public:
  HttpHandler() = default;
  HttpHandler(const HttpHandler&) = default;
  HttpHandler& operator=(const HttpHandler&) = default;
  HttpHandler(HttpHandler&&) noexcept = default;
  HttpHandler& operator=(HttpHandler&&) noexcept = default;
  virtual ~HttpHandler() = default;

  virtual void Handle(const std::shared_ptr<Request>& req,
                      std::shared_ptr<Response>& resp);

  virtual void DoGet(const std::shared_ptr<Request>& req,
                     [[maybe_unused]] std::shared_ptr<Response>& resp) {
    throw Http405Except(req->method(), req->url().src());
  }

  virtual void DoPost(const std::shared_ptr<Request>& req,
                      [[maybe_unused]] std::shared_ptr<Response>& resp) {
    throw Http405Except(req->method(), req->url().src());
  }

  virtual void DoPut(const std::shared_ptr<Request>& req,
                     [[maybe_unused]] std::shared_ptr<Response>& resp) {
    throw Http405Except(req->method(), req->url().src());
  }

  virtual void DoDelete(const std::shared_ptr<Request>& req,
                        [[maybe_unused]] std::shared_ptr<Response>& resp) {
    throw Http405Except(req->method(), req->url().src());
  }

  virtual void DoHead(const std::shared_ptr<Request>& req,
                      [[maybe_unused]] std::shared_ptr<Response>& resp) {
    throw Http405Except(req->method(), req->url().src());
  }

  virtual void DoOptions(const std::shared_ptr<Request>& req,
                         [[maybe_unused]] std::shared_ptr<Response>& resp) {
    throw Http405Except(req->method(), req->url().src());
  }

  virtual void DoConnect(const std::shared_ptr<Request>& req,
                         [[maybe_unused]] std::shared_ptr<Response>& resp) {
    throw Http405Except(req->method(), req->url().src());
  }

  virtual void DoTrace(const std::shared_ptr<Request>& req,
                       [[maybe_unused]] std::shared_ptr<Response>& resp) {
    throw Http405Except(req->method(), req->url().src());
  }

  virtual void DoPatch(const std::shared_ptr<Request>& req,
                       [[maybe_unused]] std::shared_ptr<Response>& resp) {
    throw Http405Except(req->method(), req->url().src());
  }
};

class Http404Handler : public HttpHandler {
 public:
  Http404Handler() = default;
  Http404Handler(const Http404Handler&) = default;
  Http404Handler& operator=(const Http404Handler&) = default;
  Http404Handler(Http404Handler&&) noexcept = default;
  Http404Handler& operator=(Http404Handler&&) noexcept = default;
  ~Http404Handler() override = default;

  void Handle(const std::shared_ptr<Request>& req,
              [[maybe_unused]] std::shared_ptr<Response>& resp) override {
    throw Http404Except(req->method(), req->url().src());
  }
};

}  // namespace ayaka

#endif  // AYAKA_SRC_HTTP_HANDLER_HPP_
