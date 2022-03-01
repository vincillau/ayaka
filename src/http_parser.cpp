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

#include "http_parser.hpp"

#include <cassert>

#include "case.hpp"
#include "error.hpp"
#include "http_except.hpp"

namespace ayaka {

RequestStartLineParser::RequestStartLineParser(
    RequestStartLineParser&& other) noexcept
    : method_(std::move(other.method_)),
      url_(std::move(other.url_)),
      version_(std::move(other.version_)),
      state_(other.state_) {
  other.state_ = State::kMoved;
}

RequestStartLineParser& RequestStartLineParser::operator=(
    RequestStartLineParser&& other) noexcept {
  if (this != &other) {
    state_ = other.state_;
    method_ = std::move(other.method_);
    url_ = std::move(other.url_);
    version_ = std::move(other.version_);
    other.state_ = State::kMoved;
  }
  return *this;
}

void RequestStartLineParser::Step(char ch) {
  switch (state_) {
    case State::kMethod:
      if (ch == ' ' || ch == '\t') {
        state_ = State::kMethodSpace;
      } else {
        method_.push_back(ch);
      }
      break;
    case State::kMethodSpace:
      if (!(ch == ' ' || ch == '\t')) {
        url_.push_back(ch);
        state_ = State::kUrl;
      }
      break;
    case State::kUrl:
      if (ch == ' ' || ch == '\t') {
        state_ = State::kUrlSpace;
      } else {
        url_.push_back(ch);
      }
      break;
    case State::kUrlSpace:
      if (!(ch == ' ' || ch == '\t')) {
        version_.push_back(ch);
        state_ = State::kVersion;
      }
      break;
    case State::kVersion:
      if (ch == '\r') {
        state_ = State::kNewLine;
      } else {
        version_.push_back(ch);
      }
      break;
    case State::kNewLine:
      if (ch == '\n') {
        state_ = State::kDone;
      } else {
        throw Http400Except();
      }
      break;
    case State::kDone:
      AYAKA_LOG_CRITICAL("should call Reset()");
    case State::kMoved:
      AYAKA_LOG_CRITICAL("use after move");
  }
}

void RequestStartLineParser::Reset() {
  AYAKA_TERM_IF(state_ == State::kMoved, "use after move");
  method_.clear();
  url_.clear();
  version_.clear();
  state_ = State::kMethod;
}

HeaderParser::HeaderParser(HeaderParser&& other) noexcept
    : name_(std::move(other.name_)),
      value_(std::move(other.value_)),
      state_(other.state_) {
  other.state_ = State::kMoved;
}

HeaderParser& HeaderParser::operator=(HeaderParser&& other) noexcept {
  if (this != &other) {
    state_ = other.state_;
    name_ = std::move(other.name_);
    value_ = std::move(other.value_);
    other.state_ = State::kMoved;
  }
  return *this;
}

void HeaderParser::Step(char ch) {
  switch (state_) {
    case State::kLineStart:
      if (ch == '\r') {
        state_ = State::kEmptyNewLine;
      } else {
        name_.push_back(ch);
        state_ = State::kName;
      }
      break;
    case State::kEmptyNewLine:
      if (ch == '\n') {
        state_ = State::kDone;
      } else {
        throw Http400Except();
      }
      break;
    case State::kName:
      if (ch == ':') {
        state_ = State::kNameSpace;
      } else {
        name_.push_back(ch);
      }
      break;
    case State::kNameSpace:
      if (!(ch == ' ' || ch == '\t')) {
        value_.push_back(ch);
        state_ = State::kValue;
      }
      break;
    case State::kValue:
      if (ch == '\r') {
        state_ = State::kNewLine;
      } else {
        value_.push_back(ch);
      }
      break;
    case State::kNewLine:
      if (ch == '\n') {
        StrLower(name_);
        headers_[std::move(name_)] = std::move(value_);
        name_.clear();
        value_.clear();
        state_ = State::kLineStart;
      } else {
        throw Http400Except();
      }
      break;
    case State::kDone:
      AYAKA_LOG_CRITICAL("should call Reset()");
    case State::kMoved:
      AYAKA_LOG_CRITICAL("use after move");
  }
}

void HeaderParser::Reset() {
  AYAKA_TERM_IF(state_ == State::kMoved, "use after move");
  name_.clear();
  value_.clear();
  headers_.clear();
  state_ = State::kLineStart;
}

RequestParser::RequestParser(RequestParser&& other) noexcept
    : rslp_(std::move(other.rslp_)),
      hp_(std::move(other.hp_)),
      state_(other.state_) {
  other.state_ = State::kMoved;
}

RequestParser& RequestParser::operator=(RequestParser&& other) noexcept {
  if (this != &other) {
    state_ = other.state_;
    rslp_ = std::move(other.rslp_);
    hp_ = std::move(other.hp_);
    other.state_ = State::kMoved;
  }
  return *this;
}

void RequestParser::Step(char ch) {
  switch (state_) {
    case State::kStartLine:
      rslp_.Step(ch);
      if (rslp_.state() == RequestStartLineParser::State::kDone) {
        req_->set_method(rslp_.method());
        req_->set_url(Url(rslp_.url()));
        req_->set_version(rslp_.version());
        state_ = State::kHeader;
        // 检查 URL 是否合法。
        if (!req_->url().valid()) {
          throw Http400Except();
        }
      }
      break;
    case State::kHeader:
      hp_.Step(ch);
      if (hp_.state() == HeaderParser::State::kDone) {
        req_->set_headers(hp_.headers());
        state_ = State::kDone;
        return;
      }
      break;
    case State::kDone:
      AYAKA_LOG_CRITICAL("should call Reset()");
    case State::kMoved:
      AYAKA_LOG_CRITICAL("use after move");
  }
}

size_t RequestParser::Exec(const char* data, size_t len) {
  for (auto i = 0; i < len; i++) {
    Step(data[i]);
    if (state_ == State::kDone) {
      if (!req_->url().valid()) {
        throw Http400Except();
      }
      return i + 1;
    }
  }
  return len;
}

void RequestParser::Reset() {
  rslp_.Reset();
  hp_.Reset();
  req_ = std::make_shared<Request>();
  state_ = State::kStartLine;
}

}  // namespace ayaka
