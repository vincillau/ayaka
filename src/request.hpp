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

#ifndef AYAKA_SRC_REQUEST_HPP_
#define AYAKA_SRC_REQUEST_HPP_

#include <memory>
#include <string>

#include "http_method.hpp"
#include "url.hpp"

namespace ayaka {

class Request {
 public:
  Request() : method_(http_method::kUnknown) {}
  Request(const Request&) = default;
  Request& operator=(const Request&) = default;
  Request(Request&&) noexcept = default;
  Request& operator=(Request&&) noexcept = default;
  ~Request() = default;

  [[nodiscard]] auto& method() const { return method_; }
  void set_method(std::string method) { method_ = std::move(method); }
  [[nodiscard]] auto& url() const { return url_; }
  [[nodiscard]] auto& url() { return url_; }
  void set_url(Url url) { url_ = std::move(url); }
  [[nodiscard]] auto& version() const { return version_; }
  void set_version(std::string version) { version_ = std::move(version); }
  [[nodiscard]] auto& headers() { return headers_; }
  void set_headers(std::unordered_map<std::string, std::string> headers) {
    headers_ = std::move(headers);
  }

 private:
  std::string method_;
  Url url_;
  std::string version_;
  std::unordered_map<std::string, std::string> headers_;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_REQUEST_HPP_
