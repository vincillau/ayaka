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

#include "url.hpp"

#include <stdexcept>

namespace ayaka {

namespace {

const std::unordered_map<char, int> kHexMap = {
    {'0', 0},  {'1', 1},  {'2', 2},  {'3', 3},  {'4', 4},  {'5', 5},
    {'6', 6},  {'7', 7},  {'8', 8},  {'9', 9},  {'A', 10}, {'B', 11},
    {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}, {'a', 10}, {'b', 11},
    {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15},
};

}  // namespace

std::vector<std::string> Query::Get(const std::string& key) const {
  std::vector<std::string> result;
  try {
    auto indexs = keys_.at(key);
    for (auto index : indexs) {
      result.push_back(values_[index]);
    }
  } catch (const std::out_of_range& e) {
  }
  return std::move(result);
}

void Query::Add(std::string key, std::string value) {
  values_.push_back(std::move(value));
  auto index = values_.size() - 1;
  keys_[std::move(key)].push_back(index);
}

std::string Url::Decode(const std::string& str) {
  auto len = str.size();
  auto i = 0;
  std::string result;
  while (i < len) {
    if (str[i] != '%' || i + 2 >= len) {
      result.push_back(str[i]);
      i++;
      continue;
    }

    auto c1 = str[i + 1];
    auto c2 = str[i + 2];
    if ((isxdigit(c1) == 0) || (isxdigit(c2) == 0)) {
      result.push_back(str[i]);
      i++;
      continue;
    }

    auto v1 = kHexMap.at(c1);
    auto v2 = kHexMap.at(c2);
    auto v = v1 * 16 + v2;
    result.push_back(static_cast<char>(v));
    i += 3;
  }
  return result;
}

void Url::Parse(const std::string& url) {
  auto path_start = ParseSchemeHostPort(url);
  if (path_start == std::string::npos) {
    return;
  }
  auto path_query = std::move(url.substr(path_start));
  ParsePathQuery(std::move(path_query));
}

std::string::size_type Url::ParseSchemeHostPort(const std::string& url) {
  auto scheme_end = url.find("://");
  if (scheme_end == std::string::npos) {
    // 没有 scheme, host 和 port。
    return 0;
  }
  scheme_ = std::move(url.substr(0, scheme_end));
  auto path_start = url.find('/', scheme_end + 3);
  if (path_start == std::string::npos) {
    // 只有 scheme 和 host，可能有 port。
    auto host_port = url.substr(scheme_end + 3);
    ParseHostPort(std::move(host_port));
  }
  // 有 scheme， host，和 path，可能有 port。
  auto host_port = url.substr(scheme_end + 3, path_start - (scheme_end + 3));
  ParseHostPort(std::move(host_port));
  return path_start;
}

void Url::ParseHostPort(std::string host_port) {
  auto host_end = host_port.find(':');
  if (host_end == std::string::npos) {
    // 没有 port。
    host_ = std::move(host_port);
    if (host_.empty()) {
      // host_port 为空字符串。
      valid_ = false;
    }
    return;
  }
  host_ = std::move(host_port.substr(0, host_end));
  if (host_.empty()) {
    // host 为空。
    valid_ = false;
    return;
  }
  auto port = std::move(host_port.substr(host_end + 1));
  if (port.empty()) {
    // port 为空。
    valid_ = false;
    return;
  }
  try {
    auto port_int = std::stoi(port);
    if (port_int < 0 || port_int > UINT16_MAX) {
      // port 超出范围。
      valid_ = false;
      return;
    }
    port_ = port_int;
  } catch (std::invalid_argument& e) {
    // port 不是数字。
    valid_ = false;
  } catch (std::out_of_range& e) {
    // port 超出范围。
    valid_ = false;
  }
}

void Url::ParsePathQuery(std::string path_query) {
  auto path_end = path_query.find('?');
  if (path_end == std::string::npos) {
    // 没有 query。
    auto path = std::filesystem::path(std::move(path_query));
    path_ = std::move(Decode(path.lexically_normal()));
    return;
  }
  auto path = std::filesystem::path(std::move(path_query.substr(0, path_end)));
  path_ = std::move(Decode(path.lexically_normal()));
  ParseQuery(path_query.substr(path_end + 1));
}

void Url::ParseQuery(const std::string& query_str) {
  std::string key;
  std::string value;
  bool parse_key = true;

  for (const char ch : query_str) {
    if (parse_key) {
      if (ch != '=') {
        key.push_back(ch);
        continue;
      }
      parse_key = false;
      continue;
    }
    if (ch != '&') {
      value.push_back(ch);
      continue;
    }
    query_.Add(std::move(Decode(key)), std::move(Decode(value)));
    key.clear();
    value.clear();
    parse_key = true;
  }

  if (parse_key && key.empty()) {
    // query_str 以 & 结尾。
    return;
  }
  query_.Add(std::move(Decode(key)), std::move(Decode(value)));
}

}  // namespace ayaka
