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

#include "conf.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>

#include "case.hpp"
#include "logger.hpp"

namespace ayaka {

bool ListenConf::Valid() const {
  if (port_ > kMaxPort) {
    return false;
  }

  InetAddr::Family family = InetAddr::Family::kIpv4;
  if (StrEqualIgnoreCase(family_, "ipv4")) {
    family = InetAddr::Family::kIpv4;
  } else if (StrEqualIgnoreCase(family_, "ipv6")) {
    family = InetAddr::Family::kIpv6;
  } else {
    // 在调用此函数之前，已经确保 family_ 是有效的。
    AYAKA_LOG_CRITICAL("invalid listen family: {}", family_);
  }

  InetAddr addr(family, ip_, port_);
  // InetAddr::Valid 负责检查端口是否为 0。
  return addr.Valid();
}

InetAddr ListenConf::ToInetAddr() const {
  InetAddr::Family family = InetAddr::Family::kIpv4;
  if (StrEqualIgnoreCase(family_, "ipv4")) {
    family = InetAddr::Family::kIpv4;
  } else if (StrEqualIgnoreCase(family_, "ipv6")) {
    family = InetAddr::Family::kIpv6;
  } else {
    AYAKA_LOG_CRITICAL("invalid listen family: {}", family_);
  }
  return std::move(InetAddr(family, ip_, port_));
}

void ListenConf::FromJson(const nlohmann::json& json) {
  if (json.find("family") != json.end()) {
    if (!json.at("family").is_string()) {
      AYAKA_LOG_CRITICAL("\"family\" must be a string");
    }
    family_ = json["family"];
  }
  if (json.find("ip") != json.end()) {
    if (!json.at("ip").is_string()) {
      AYAKA_LOG_CRITICAL("\"ip\" must be a string");
    }
    ip_ = json["ip"];
  }
  if (json.find("port") != json.end()) {
    if (!json.at("port").is_number_integer()) {
      AYAKA_LOG_CRITICAL("\"port\" must be an integer");
    }
    port_ = json["port"];
  }
}

bool ServerConf::Valid() const {
  if (worker_threads_ <= 0 || worker_threads_ > kMaxWorkerThreads) {
    return false;
  }
  return listen_.Valid();
}

void ServerConf::FromJson(const nlohmann::json& json) {
  if (json.find("listen") != json.end()) {
    if (!json.at("listen").is_object()) {
      AYAKA_LOG_CRITICAL("\"listen\" must be an object");
    }
    listen_.FromJson(json.at("listen"));
  }
  if (json.find("worker_threads") != json.end()) {
    if (!json.at("worker_threads").is_number_integer()) {
      AYAKA_LOG_CRITICAL("\"worker_threads\" must be an integer");
    }
    worker_threads_ = json.at("worker_threads");
  }
}

HttpConf::HttpConf() {
  std::filesystem::path mime_path(__FILE__);
  mime_path = mime_path.parent_path().parent_path() / "res/mime.types";
  mime_ = mime_path.string();
}

void HttpConf::FromJson(const nlohmann::json& json) {
  if (json.find("mime") != json.end()) {
    if (!json.at("mime").is_string()) {
      AYAKA_LOG_CRITICAL("\"mime\" must be a string");
    }
    mime_ = json["mime"];
  }
}

void RouteConf::FromJson(const nlohmann::json& json) {
  if (json.find("type") != json.end()) {
    if (!json.at("type").is_string()) {
      AYAKA_LOG_CRITICAL("\"type\" must be a string");
    }
    type_ = json["type"];
  }
  if (json.find("url") != json.end()) {
    if (!json.at("url").is_string()) {
      AYAKA_LOG_CRITICAL("\"url\" must be a string");
    }
    url_ = json["url"];
  }
  if (json.find("path") != json.end()) {
    if (!json.at("path").is_string()) {
      AYAKA_LOG_CRITICAL("\"path\" must be a string");
    }
    path_ = json["path"];
  }
  if (json.find("root") != json.end()) {
    if (!json.at("root").is_string()) {
      AYAKA_LOG_CRITICAL("\"root\" must be a string");
    }
    root_ = json["root"];
  }
}

void Conf::Load(const std::string& conf_path) {
  std::ifstream conf_file(conf_path);
  if (!conf_file.is_open()) {
    AYAKA_LOG_CRITICAL("failed to open conf file: {}", conf_path);
    return;
  }
  std::string json_str;
  std::string line;
  while (std::getline(conf_file, line)) {
    json_str += line;
  }

  nlohmann::json json;
  try {
    json = nlohmann::json::parse(json_str);
  } catch (const nlohmann::json::parse_error& e) {
    AYAKA_LOG_CRITICAL("failed to parse conf file: {}", e.what());
    return;
  }

  if (json.find("server") != json.end()) {
    if (!json.at("server").is_object()) {
      AYAKA_LOG_CRITICAL("\"server\" must be an object");
    }
    server_.FromJson(json.at("server"));
  }

  if (json.find("http") != json.end()) {
    if (!json.at("http").is_object()) {
      AYAKA_LOG_CRITICAL("\"http\" must be an object");
    }
    http_.FromJson(json.at("http"));
  }

  if (json.find("route") != json.end()) {
    if (!json.at("route").is_array()) {
      AYAKA_LOG_CRITICAL("\"route\" must be an array");
    }
    for (const auto& route : json.at("route")) {
      if (!route.is_object()) {
        AYAKA_LOG_CRITICAL("\"route\" must be an array of objects");
      }
      RouteConf route_conf;
      route_conf.FromJson(route);
      route_.push_back(std::move(route_conf));
    }
  }
}

}  // namespace ayaka
