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

#ifndef AYAKA_SRC_ROUTER_HPP_
#define AYAKA_SRC_ROUTER_HPP_

#include <memory>
#include <vector>

#include "http_except.hpp"
#include "location.hpp"
#include "logger.hpp"

namespace ayaka {

class Router {
 public:
  Router() = default;
  Router(const Router&) = default;
  Router& operator=(const Router&) = default;
  Router(Router&&) noexcept = default;
  Router& operator=(Router&&) noexcept = default;
  ~Router() = default;

  /**
   * 进行 URL 路由，返回匹配到的 HttpHandler 对象。如果没有匹配到，则抛出
   * Http404Except 异常。当有多个匹配的 Location
   * 时，则返回第一个（按照添加顺序）。
   */
  [[nodiscard]] std::shared_ptr<HttpHandler> Route(
      const std::string& path) const;

  void AddLocation(const std::shared_ptr<Location>& location) {
    locations_.push_back(location);
  }

 private:
  std::vector<std::shared_ptr<Location>> locations_;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_ROUTER_HPP_
