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

#include "router.hpp"

#include <memory>

namespace ayaka {

std::shared_ptr<HttpHandler> Router::Route(const std::string& path) const {
  for (const auto& location : locations_) {
    if (location->Match(path)) {
      return location->handler();
    }
  }
  return std::move(std::make_shared<Http404Handler>());
}

}  // namespace ayaka
