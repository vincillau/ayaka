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

#include <sys/resource.h>
#include <unistd.h>

#include <cstring>
#include <memory>
#include <utility>

#include "case.hpp"
#include "error.hpp"
#include "inet_addr.hpp"
#include "location.hpp"
#include "logger.hpp"
#include "static_handler.hpp"

namespace ayaka {

Server::Server(Conf conf)
    : conf_(std::move(conf)), router_(std::make_shared<Router>()) {
  mime_ = std::make_shared<Mime>();
  mime_->Load(conf_.http().mime());

  LiftFdLimit();
  SetUpRouter();

  auto listen_addr = conf_.server().listen().ToInetAddr();
  auto worker_threads = conf_.server().worker_threads();

  for (int i = 0; i < worker_threads; ++i) {
    workers_.push_back(std::make_shared<Worker>(listen_addr, router_));
  }
}

void Server::Wait() const {
  for (const auto &worker : workers_) {
    worker->Join();
  }
}

void Server::SetUpRouter() {
  const auto &route = conf_.route();
  for (const auto &location : route) {
    const auto &type = location.type();
    if (StrEqualIgnoreCase(type, "path")) {
      auto handler =
          std::make_shared<StaticPathHandler>(location.path(), mime_);
      auto path_location =
          std::make_shared<PathLocation>(location.url(), std::move(handler));
      router_->AddLocation(path_location);
    } else if (StrEqualIgnoreCase(type, "dir")) {
      auto handler = std::make_shared<StaticDirHandler>(location.url(),
                                                        location.root(), mime_);
      auto dir_location =
          std::make_shared<DirLocation>(location.url(), std::move(handler));
      router_->AddLocation(dir_location);
    } else {
      AYAKA_LOG_CRITICAL("unknown location type: {}", type);
    }
  }
}

void Server::LiftFdLimit() {
  struct rlimit rl;
  if (getrlimit(RLIMIT_NOFILE, &rl) == -1) {
    AYAKA_LOG_CRITICAL("getrlimit failed");
  }

  rl.rlim_cur = rl.rlim_max;
  if (setrlimit(RLIMIT_NOFILE, &rl) == -1) {
    AYAKA_LOG_CRITICAL("setrlimit failed");
  }
}

}  // namespace ayaka
