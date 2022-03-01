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

#ifndef AYAKA_SRC_LOCATION_HPP_
#define AYAKA_SRC_LOCATION_HPP_

#include <filesystem>
#include <memory>
#include <string>

#include "http_handler.hpp"

namespace ayaka {

class Location {
 public:
  Location() = default;
  Location(const Location &) = default;
  Location &operator=(const Location &) = default;
  Location(Location &&) noexcept = default;
  Location &operator=(Location &&) noexcept = default;
  virtual ~Location() = default;

  /**
   * path 参数必须经过 std::filesystem::path::lexically_normal 转换。
   */
  [[nodiscard]] virtual bool Match(const std::string &path) const = 0;

  [[nodiscard]] auto &handler() const { return handler_; }
  void set_handler(std::shared_ptr<HttpHandler> handler) {
    handler_ = std::move(handler);
  }

 protected:
  explicit Location(std::shared_ptr<HttpHandler> handler)
      : handler_(std::move(handler)) {}

 private:
  std::shared_ptr<HttpHandler> handler_;
};

class PathLocation : public Location {
 public:
  PathLocation() = default;

  /**
   * path 将被 std::filesystem::path::lexically_normal 转换。
   */
  PathLocation(std::string path, std::shared_ptr<HttpHandler> handler);

  PathLocation(const PathLocation &) = default;
  PathLocation &operator=(const PathLocation &) = default;
  PathLocation(PathLocation &&) noexcept = default;
  PathLocation &operator=(PathLocation &&) noexcept = default;
  ~PathLocation() override = default;

  /**
   * path 应该是被 std::filesystem::path::lexically_normal 转换过的。
   */
  [[nodiscard]] bool Match(const std::string &path) const override {
    return path == path_.string();
  }

 private:
  std::filesystem::path path_;
};

class DirLocation : public Location {
 public:
  DirLocation() = default;

  /**
   * dir 将被 std::filesystem::path::lexically_normal 转换。
   */
  DirLocation(std::string dir, std::shared_ptr<HttpHandler> handler);

  DirLocation(const DirLocation &) = default;
  DirLocation &operator=(const DirLocation &) = default;
  DirLocation(DirLocation &&) noexcept = default;
  DirLocation &operator=(DirLocation &&) noexcept = default;
  ~DirLocation() override = default;

  /**
   * path 应该是被 std::filesystem::path::lexically_normal 转换过的。
   */
  [[nodiscard]] bool Match(const std::string &path) const override {
    if (dir_.empty()) {
      return false;
    }
    return path.rfind(dir_.string(), 0) == 0;
  }

 private:
  std::filesystem::path dir_;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_LOCATION_HPP_
