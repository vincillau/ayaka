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

#ifndef AYAKA_SRC_CONF_HPP_
#define AYAKA_SRC_CONF_HPP_

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "inet_addr.hpp"

namespace ayaka {

class ListenConf {
 public:
  ListenConf() = default;
  ListenConf(const ListenConf&) = default;
  ListenConf& operator=(const ListenConf&) = default;
  ListenConf(ListenConf&&) noexcept = default;
  ListenConf& operator=(ListenConf&&) noexcept = default;
  ~ListenConf() = default;

  [[nodiscard]] auto& family() const { return family_; }
  void set_family(std::string family) { family_ = std::move(family); }
  [[nodiscard]] auto& ip() const { return ip_; }
  void set_ip(std::string ip) { ip_ = std::move(ip); }
  [[nodiscard]] auto port() const { return port_; }
  void set_port(int port) { port_ = port; }

  [[nodiscard]] bool Valid() const;
  // 在调用此函数之前，需要确保 ListenConf 是有效的。
  [[nodiscard]] InetAddr ToInetAddr() const;

  void FromJson(const nlohmann::json& json);

  static constexpr auto kDefaultFamily = "ipv4";
  static constexpr auto kDefaultIp = "127.0.0.1";
  static constexpr auto kDefaultPort = 8080;
  static constexpr auto kReservedPort = 0;
  static constexpr auto kMaxPort = UINT16_MAX;

 private:
  std::string family_ = kDefaultFamily;
  std::string ip_ = kDefaultIp;
  int port_ = kDefaultPort;
};

class ServerConf {
 public:
  ServerConf() = default;
  ServerConf(const ServerConf&) = default;
  ServerConf& operator=(const ServerConf&) = default;
  ServerConf(ServerConf&&) noexcept = default;
  ServerConf& operator=(ServerConf&&) noexcept = default;
  ~ServerConf() = default;

  [[nodiscard]] auto& worker_threads() const { return worker_threads_; }
  void set_worker_threads(int worker_threads) {
    worker_threads_ = worker_threads;
  }
  [[nodiscard]] auto& listen() const { return listen_; }
  [[nodiscard]] auto& listen() { return listen_; }

  /* 工作线程数必须大于 0，并且小于等于 kMaxWorkerThreads。
   * 如果 listen_ 也必须有效。否则返回 false。
   */
  [[nodiscard]] bool Valid() const;

  void FromJson(const nlohmann::json& json);

  static constexpr int kDefaultWorkerThreads = 4;
  static constexpr int kMaxWorkerThreads = 255;

 private:
  ListenConf listen_;
  int worker_threads_ = kDefaultWorkerThreads;
};

class HttpConf {
 public:
  HttpConf();
  HttpConf(const HttpConf&) = default;
  HttpConf& operator=(const HttpConf&) = default;
  HttpConf(HttpConf&&) noexcept = default;
  HttpConf& operator=(HttpConf&&) noexcept = default;
  ~HttpConf() = default;

  [[nodiscard]] auto& mime() const { return mime_; }
  void set_mime(std::string mime) {
    mime_ = std::move(mime);
  }

  void FromJson(const nlohmann::json& json);

 private:
  std::string mime_;
};

class RouteConf {
 public:
  RouteConf() = default;
  RouteConf(const RouteConf&) = default;
  RouteConf& operator=(const RouteConf&) = default;
  RouteConf(RouteConf&&) noexcept = default;
  RouteConf& operator=(RouteConf&&) noexcept = default;
  ~RouteConf() = default;

  [[nodiscard]] auto& type() const { return type_; }
  void set_type(std::string type) { type_ = std::move(type); }
  [[nodiscard]] auto& url() const { return url_; }
  void set_url(std::string url) { url_ = std::move(url); }
  [[nodiscard]] auto& path() const { return path_; }
  void set_path(std::string path) { path_ = std::move(path); }
  [[nodiscard]] auto& root() const { return root_; }
  void set_root(std::string root) { root_ = std::move(root); }

  void FromJson(const nlohmann::json& json);

 private:
  std::string type_;
  std::string url_;
  std::string path_;
  std::string root_;
};

class Conf {
 public:
  Conf() = default;
  Conf(const Conf&) = default;
  Conf& operator=(const Conf&) = default;
  Conf(Conf&&) noexcept = default;
  Conf& operator=(Conf&&) noexcept = default;
  ~Conf() = default;

  [[nodiscard]] auto& server() const { return server_; }
  [[nodiscard]] auto& server() { return server_; }
  [[nodiscard]] auto& route() const { return route_; }
  [[nodiscard]] auto& route() { return route_; }
  [[nodiscard]] auto& http() const { return http_; }
  [[nodiscard]] auto& http() { return http_; }

  void Load(const std::string& conf_path);

 private:
  ServerConf server_;
  HttpConf http_;
  std::vector<RouteConf> route_;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_CONF_HPP_
