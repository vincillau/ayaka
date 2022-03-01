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

#ifndef AYAKA_SRC_INET_ADDR_HPP_
#define AYAKA_SRC_INET_ADDR_HPP_

#include <arpa/inet.h>
#include <net/if.h>

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

namespace ayaka {

class InetAddr {
 public:
  enum class Family { kIpv4, kIpv6 };

  InetAddr() = default;

  InetAddr(Family family, std::string ip, uint16_t port)
      : family_(family), ip_(std::move(ip)), port_(port) {}

  InetAddr(const InetAddr& addr) = default;
  InetAddr& operator=(const InetAddr& addr) = default;
  InetAddr(InetAddr&& addr) noexcept = default;
  InetAddr& operator=(InetAddr&& addr) noexcept = default;
  ~InetAddr() = default;

  [[nodiscard]] auto family() const { return family_; }
  void set_family(Family family) { family_ = family; }
  [[nodiscard]] auto& ip() const { return ip_; }
  void set_ip(std::string ip) { ip_ = std::move(ip); }
  [[nodiscard]] auto port() const { return port_; }
  void set_port(uint16_t port) { port_ = port; }

  /**
   * 判断地址是否有效。端口为 0 的地址无效。
   */
  [[nodiscard]] bool Valid() const;

  void ToSockAddr(sockaddr* addr) const;

  static constexpr uint16_t kReservedPort = 0;

 private:
  Family family_ = Family::kIpv4;
  std::string ip_;
  uint16_t port_ = kReservedPort;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_INET_ADDR_HPP_
