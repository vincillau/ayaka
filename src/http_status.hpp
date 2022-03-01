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

#ifndef AYAKA_SRC_HTTP_STATUS_HPP_
#define AYAKA_SRC_HTTP_STATUS_HPP_

#include <string>

namespace ayaka {

class HttpStatus {
 public:
  HttpStatus() = default;

  HttpStatus(std::string code, std::string msg)
      : code_(std::move(code)), msg_(std::move(msg)) {}

  HttpStatus(const HttpStatus&) = default;
  HttpStatus& operator=(const HttpStatus&) = default;
  HttpStatus(HttpStatus&&) noexcept = default;
  HttpStatus& operator=(HttpStatus&&) noexcept = default;
  ~HttpStatus() = default;

  static auto Ok() { return std::move(HttpStatus("200", "OK")); }
  static auto BadRequest() { return std::move(HttpStatus("400", "Bad Request")); }
  static auto NotFound() { return std::move(HttpStatus("404", "Not Found")); }
  static auto MethodNotAllowed() {
    return std::move(HttpStatus("405", "Method Not Allowed"));
  }

  [[nodiscard]] auto& code() const { return code_; }
  [[nodiscard]] auto& msg() const { return msg_; }

 private:
  std::string code_;
  std::string msg_;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_HTTP_STATUS_HPP_
