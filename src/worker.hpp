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

#ifndef AYAKA_SRC_WORKER_HPP_
#define AYAKA_SRC_WORKER_HPP_

#include <cstdint>
#include <memory>
#include <string>
#include <thread>
#include <unordered_set>

#include "downstream.hpp"
#include "inet_addr.hpp"
#include "logger.hpp"
#include "loop.hpp"
#include "tcp.hpp"

namespace ayaka {

class Worker {
 public:
  Worker() = delete;
  Worker(InetAddr addr, std::shared_ptr<Router> router);

  /**
   * Worker 不能被拷贝或移动。
   */
  Worker(const Worker&) = delete;
  Worker& operator=(const Worker&) = delete;
  Worker(Worker&&) noexcept = default;
  Worker& operator=(Worker&&) noexcept = delete;

  ~Worker() = default;

  auto loop() const { return loop_; }

  void Join() const;

 private:
  void Run(const InetAddr& addr);

  void OnListerAccept(std::shared_ptr<Tcp> tcp);

  std::unique_ptr<std::thread> thread_;
  std::shared_ptr<Loop> loop_;
  std::unique_ptr<Tcp> listener_;
  std::shared_ptr<Router> router_;
  std::unordered_set<std::shared_ptr<Downstream>> downstreams_;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_WORKER_HPP_
