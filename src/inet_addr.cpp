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

#include "inet_addr.hpp"

#include <cstring>

namespace ayaka {

bool InetAddr::Valid() const {
  if (port_ == kReservedPort) {
    return false;
  }
  sockaddr addr{};
  memset(&addr, 0, sizeof(addr));
  int family = (family_ == Family::kIpv4) ? AF_INET : AF_INET6;
  int res = inet_pton(family, ip_.c_str(), &addr);
  return res == 1;
}

void InetAddr::ToSockAddr(sockaddr* addr) const {
  memset(addr, 0, sizeof(sockaddr));
  if (family_ == Family::kIpv4) {
    auto* ipv4_addr = reinterpret_cast<sockaddr_in*>(addr);
    ipv4_addr->sin_family = AF_INET;
    ipv4_addr->sin_port = htons(port_);
    inet_pton(AF_INET, ip_.c_str(), &ipv4_addr->sin_addr);
  } else {
    auto* ipv6_addr = reinterpret_cast<sockaddr_in6*>(addr);
    ipv6_addr->sin6_family = AF_INET6;
    ipv6_addr->sin6_port = htons(port_);
    inet_pton(AF_INET6, ip_.c_str(), &ipv6_addr->sin6_addr);
  }
}

}  // namespace ayaka
