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

#ifndef AYAKA_SRC_HTTP_PARSER_HPP_
#define AYAKA_SRC_HTTP_PARSER_HPP_

#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "request.hpp"
#include "response.hpp"

namespace ayaka {

class RequestStartLineParser {
 public:
  enum class State {
    kMethod,
    kMethodSpace,
    kUrl,
    kUrlSpace,
    kVersion,
    kNewLine,
    kDone,
    kMoved
  };

  RequestStartLineParser() = default;

  /**
   * RequestStartLineParser 只能移动，不能拷贝。
   */
  RequestStartLineParser(const RequestStartLineParser&) = delete;
  RequestStartLineParser& operator=(const RequestStartLineParser&) = delete;

  RequestStartLineParser(RequestStartLineParser&& other) noexcept;
  RequestStartLineParser& operator=(RequestStartLineParser&& other) noexcept;
  ~RequestStartLineParser() = default;

  [[nodiscard]] auto& method() const { return method_; }
  [[nodiscard]] auto& url() const { return url_; }
  [[nodiscard]] auto& version() const { return version_; }
  [[nodiscard]] auto state() const { return state_; }

  void Step(char ch);
  void Reset();

 private:
  std::string method_;
  std::string url_;
  std::string version_;
  State state_ = State::kMethod;
};

/**
 * HeaderParser 会将 HTTP 请求头部的名称转换为小写。
 */
class HeaderParser {
 public:
  enum class State {
    kLineStart,
    kEmptyNewLine,
    kName,
    kNameSpace,
    kValue,
    kNewLine,
    kDone,
    kMoved
  };

  HeaderParser() = default;

  /**
   * HeaderParser 只能移动，不能拷贝。
   */
  HeaderParser(const HeaderParser&) = delete;
  HeaderParser& operator=(const HeaderParser&) = delete;

  HeaderParser(HeaderParser&& other) noexcept;
  HeaderParser& operator=(HeaderParser&& other) noexcept;
  ~HeaderParser() = default;

  void Step(char ch);
  void Reset();

  [[nodiscard]] auto state() const { return state_; }
  [[nodiscard]] auto&& headers() { return std::move(headers_); }

 private:
  std::unordered_map<std::string, std::string> headers_;
  std::string name_;
  std::string value_;
  State state_ = State::kLineStart;
};

class RequestParser {
 public:
  enum class State { kStartLine, kHeader, kDone, kMoved };

  RequestParser() = default;

  /**
   * RequestParser 只能移动，不能拷贝。
   */
  RequestParser(const RequestParser&) = delete;
  RequestParser& operator=(const RequestParser&) = delete;

  RequestParser(RequestParser&& other) noexcept;
  RequestParser& operator=(RequestParser&& other) noexcept;

  ~RequestParser() = default;

  [[nodiscard]] auto& req() const { return req_; }
  [[nodiscard]] auto& req() { return req_; }
  [[nodiscard]] auto state() const { return state_; }

  [[nodiscard]] size_t Exec(const char* data, size_t len);
  void Reset();

 private:
  void Step(char ch);

  RequestStartLineParser rslp_;
  HeaderParser hp_;
  std::shared_ptr<Request> req_ = std::make_shared<Request>();
  State state_ = State::kStartLine;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_HTTP_PARSER_HPP_
