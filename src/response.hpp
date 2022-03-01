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

#ifndef AYAKA_SRC_RESPONSE_HPP_
#define AYAKA_SRC_RESPONSE_HPP_

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "http_status.hpp"

namespace ayaka {

class Response {
 public:
  using Body = std::shared_ptr<std::vector<char>>;

  /**
   * 创建默认的响应对象：
   * 1. 将 HTTP 版本设置为 HTTP/1.1
   * 2. 将响应状态码设置为 200 OK
   * 3. Connection 设置为 close
   * 4. Date 设置为当前时间
   * 5. Server 设置为 Ayaka/{AYAKA_VERSION}
   * 6. 不初始化 body
   */
  static std::shared_ptr<Response> Default();

  Response() = default;
  Response(const Response&) = default;
  Response& operator=(const Response&) = default;
  Response(Response&&) noexcept = default;
  Response& operator=(Response&&) noexcept = default;
  ~Response() = default;

  [[nodiscard]] auto& version() const { return version_; }
  void set_version(std::string version) { version_ = std::move(version); }
  [[nodiscard]] auto& status() const { return status_; }
  void set_status(HttpStatus status) { status_ = std::move(status); }
  [[nodiscard]] auto& headers() const { return headers_; }
  [[nodiscard]] auto& headers() { return headers_; }
  void set_headers(std::unordered_map<std::string, std::string> headers) {
    headers_ = std::move(headers);
  }
  [[nodiscard]] auto& body() const { return body_; }
  [[nodiscard]] auto& body() { return body_; }
  void set_body(Body body) { body_ = std::move(body); }

  static std::string GetDate(const time_t* tloc = nullptr);

 private:
  std::string version_;
  HttpStatus status_;
  std::unordered_map<std::string, std::string> headers_;
  // 发送 HTTP 响应时，根据 body 的长度自动设置 Content-Length。
  Body body_;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_RESPONSE_HPP_
