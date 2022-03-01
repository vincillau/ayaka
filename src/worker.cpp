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

#include "worker.hpp"

#include <utility>

namespace ayaka {

Worker::Worker(InetAddr addr, std::shared_ptr<Router> router)
    : router_(std::move(router)) {
  thread_ = std::make_unique<std::thread>(
      [this, addr = std::move(addr)]() { Run(addr); });
}

void Worker::Join() const { thread_->join(); }

void Worker::Run(const InetAddr& addr) {
  loop_ = std::make_shared<Loop>();
  listener_ = std::make_unique<Tcp>(loop_);

  listener_->set_on_accept(
      [this](std::shared_ptr<Tcp> tcp) { OnListerAccept(std::move(tcp)); });

  listener_->Bind(addr);
  listener_->Listen();
  loop_->Run();
}

void Worker::OnListerAccept(std::shared_ptr<Tcp> tcp) {
  auto downstream = std::make_shared<Downstream>(tcp, router_);
  auto res = downstreams_.insert(downstream);
  const auto& iter = *res.first;
  iter->tcp()->set_on_close([this, iter]() { downstreams_.erase(iter); });
}

}  // namespace ayaka
