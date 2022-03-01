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

#ifndef AYAKA_SRC_URL_HPP_
#define AYAKA_SRC_URL_HPP_

#include <filesystem>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace ayaka {

class Query {
 public:
  Query() = default;
  Query(const Query&) = default;
  Query& operator=(const Query&) = default;
  Query(Query&&) noexcept = default;
  Query& operator=(Query&&) noexcept = default;
  ~Query() = default;

  [[nodiscard]] auto& keys() const { return keys_; }
  [[nodiscard]] auto& values() const { return values_; }

  [[nodiscard]] auto Size() const { return values_.size(); }
  [[nodiscard]] auto Empty() const { return values_.empty(); }

  [[nodiscard]] std::vector<std::string> Get(const std::string& key) const;

  void Add(std::string key, std::string value);

 private:
  std::unordered_map<std::string, std::vector<size_t>> keys_;
  std::vector<std::string> values_;
};

class Url {
 public:
  Url() = default;

  /**
   * 支持解析 scheme://host[:port]/path[?query] 或 /path[?query] 形式的
   * URL。path 将会被 std::filesystem::path::lexically_normal 转换。
   */
  explicit Url(const std::string& url) : src_(url) { Parse(url); }

  Url(const Url&) = default;
  Url& operator=(const Url&) = default;
  Url(Url&&) noexcept = default;
  Url& operator=(Url&&) noexcept = default;
  ~Url() = default;

  [[nodiscard]] auto& src() const { return src_; }
  [[nodiscard]] auto& scheme() const { return scheme_; }
  void set_scheme(std::string scheme) { scheme_ = std::move(scheme); }
  [[nodiscard]] auto& host() const { return host_; }
  void set_host(std::string host) { host_ = std::move(host); }
  [[nodiscard]] auto& port() const { return port_; }
  void set_port(uint16_t port) { port_ = port; }
  [[nodiscard]] auto& path() const { return path_; }
  void set_path(std::string path) { path_ = std::move(path); }
  [[nodiscard]] auto& query() const { return query_; }
  [[nodiscard]] auto& query() { return query_; }
  void set_query(Query query) { query_ = std::move(query); }
  [[nodiscard]] auto valid() const { return valid_; }

  /**
   * port 为此值时表示 URL 中没有指定 port。
   */
  static constexpr uint16_t kReservedPort = 0;

 private:
  void Parse(const std::string& url);

  static std::string Decode(const std::string& str);

  /**
   * 解析 scheme, host 和 port，返回 path 的起始位置。如果没有 path 或者 URL
   * 格式错误，返回 std::string::npos。
   *
   * 通常，URL 中的没有 port 字段，所以传参使用 std::string 而不是 const
   * std::string&，这样可以将实参直接移动到 host_。可以接受一个空字符串。
   */
  std::string::size_type ParseSchemeHostPort(const std::string& url);

  /**
   * 解析主机名和端口号（如果有）。如果端口号不是大于 0 的 uint16_t，则将 valid_
   * 设置为 false。
   */
  void ParseHostPort(std::string host_port);

  /**
   * 此函数一定不会设置 valid_。
   *
   * 因为 path_query 是从 src_ 截取的，所以不用 const std::string&。
   */
  void ParsePathQuery(std::string path_query);

  /**
   * 此函数一定不会设置 valid_。
   *
   * 因为 query_str 是从 path_query 截取的，所以不用 const std::string&。
   */
  void ParseQuery(const std::string& query_str);

  std::string src_;
  std::string scheme_;
  std::string host_;
  std::filesystem::path path_ = "/";
  Query query_;
  uint16_t port_ = kReservedPort;
  bool valid_ = true;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_URL_HPP_
