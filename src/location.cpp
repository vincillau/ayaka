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

#include "location.hpp"

namespace ayaka {

PathLocation::PathLocation(std::string path,
                           std::shared_ptr<HttpHandler> handler)
    : Location(std::move(handler)) {
  auto p = std::filesystem::path(std::move(path));
  path_ = std::move(p.lexically_normal());
}

DirLocation::DirLocation(std::string dir, std::shared_ptr<HttpHandler> handler)
    : Location(std::move(handler)) {
  auto d = std::filesystem::path(std::move(dir));
  dir_ = std::move(d.lexically_normal());
}

}  // namespace ayaka
