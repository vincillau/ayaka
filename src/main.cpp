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

#include "server.hpp"

int main(void) {
  spdlog::flush_on(spdlog::level::trace);
  spdlog::set_level(spdlog::level::trace);
  spdlog::set_pattern("[%Y-%m-%d %T.%e] [%^%l%$] tid=%t %v");

  auto conf = ayaka::Conf();
  auto conf_path = std::filesystem::path(__FILE__);
  conf_path =
      std::move(conf_path.parent_path().parent_path() / "res/ayaka.json");
  conf.Load(conf_path.string());

  auto server = ayaka::Server(conf);
  server.Wait();

  return 0;
}
