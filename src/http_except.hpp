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

#ifndef AYAKA_SRC_HTTP_EXCEPT_HPP_
#define AYAKA_SRC_HTTP_EXCEPT_HPP_

#include <exception>
#include <memory>
#include <stdexcept>

#include "logger.hpp"
#include "response.hpp"

namespace ayaka {

class HttpExcept : std::exception {
 public:
  HttpExcept() = default;

  HttpExcept(const HttpExcept&) = default;
  HttpExcept& operator=(const HttpExcept&) = default;
  HttpExcept(HttpExcept&&) noexcept = default;
  HttpExcept& operator=(HttpExcept&&) noexcept = default;
  ~HttpExcept() override = default;

  /**
   * 应该传递 Response::Default 构造的 Response 对象。
   */
  virtual void SetUp(std::shared_ptr<Response>) const = 0;

 private:
  std::string code_;
  std::string msg_;
};

class Http400Except : public HttpExcept {
 public:
  Http400Except() { AYAKA_LOG_INFO("400 Bad Request"); }

  Http400Except(const Http400Except&) = default;
  Http400Except& operator=(const Http400Except&) = default;
  Http400Except(Http400Except&&) noexcept = default;
  Http400Except& operator=(Http400Except&&) noexcept = default;
  ~Http400Except() override = default;

  void SetUp(std::shared_ptr<Response> resp) const override;
};

class Http404Except : public HttpExcept {
 public:
  Http404Except(const std::string& method, const std::string& url) {
    AYAKA_LOG_INFO("{} {} 404", method, url);
  }
  Http404Except(const Http404Except&) = default;
  Http404Except& operator=(const Http404Except&) = default;
  Http404Except(Http404Except&&) noexcept = default;
  Http404Except& operator=(Http404Except&&) noexcept = default;
  ~Http404Except() override = default;

  void SetUp(std::shared_ptr<Response> resp) const override;
};

class Http405Except : public HttpExcept {
 public:
  Http405Except(const std::string& method, const std::string& url) {
    AYAKA_LOG_INFO("{} {} 405", method, url);
  }
  Http405Except(const Http405Except&) = default;
  Http405Except& operator=(const Http405Except&) = default;
  Http405Except(Http405Except&&) noexcept = default;
  Http405Except& operator=(Http405Except&&) noexcept = default;
  ~Http405Except() override = default;

  void SetUp(std::shared_ptr<Response> resp) const override;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_HTTP_EXCEPT_HPP_
