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

#ifndef AYAKA_SRC_SERVER_HPP_
#define AYAKA_SRC_SERVER_HPP_

#include <vector>

#include "conf.hpp"
#include "mime.hpp"
#include "router.hpp"
#include "worker.hpp"

namespace ayaka {

class Server {
 public:
  Server() = delete;

  /**
   * conf 必须是检查过的配置。
   */
  explicit Server(Conf conf);

  /**
   * Server 不能被拷贝或移动。
   */
  Server(const Server&) = delete;
  Server& operator=(const Server&) = delete;
  Server(Server&& other) = delete;
  Server& operator=(Server&& other) = delete;

  ~Server() = default;

  void Wait() const;

 private:
  void SetUpRouter();

  static void LiftFdLimit();

  Conf conf_;
  std::shared_ptr<Router> router_;
  std::shared_ptr<Mime> mime_;
  std::vector<std::shared_ptr<Worker>> workers_;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_SERVER_HPP_
