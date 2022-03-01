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

#ifndef AYAKA_SRC_DOWNSTREAM_HPP_
#define AYAKA_SRC_DOWNSTREAM_HPP_

#include <memory>

#include "http_parser.hpp"
#include "logger.hpp"
#include "router.hpp"
#include "tcp.hpp"

namespace ayaka {

class Downstream {
 public:
  Downstream() = default;

  explicit Downstream(std::shared_ptr<Tcp> tcp, std::shared_ptr<Router> router);

  /**
   * Downstream 只能移动，不能拷贝。
   */
  Downstream(const Downstream &) = delete;
  Downstream &operator=(const Downstream &) = delete;
  Downstream(Downstream &&) noexcept = default;
  Downstream &operator=(Downstream &&) noexcept = default;

  ~Downstream() = default;

  [[nodiscard]] auto &tcp() const { return tcp_; }
  [[nodiscard]] auto &tcp() { return tcp_; }

  auto router_ptr() const { return router_.get(); }

 private:
  void Send(std::shared_ptr<Response> resp);
  void OnRecv(const char *buf, size_t len);
  void HandleRecv(const char *buf, size_t len);

  std::shared_ptr<Tcp> tcp_;
  std::shared_ptr<Router> router_;
  RequestParser parser_;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_DOWNSTREAM_HPP_
