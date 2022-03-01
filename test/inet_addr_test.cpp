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

#include <inet_addr.hpp>

#include "test.hpp"

using ayaka::InetAddr;

TEST(InetAddrTest, Default) {
  InetAddr addr;
  EXPECT_EQ(addr.family(), InetAddr::Family::kIpv4);
  EXPECT_EQ(addr.ip(), "");
  EXPECT_EQ(addr.port(), InetAddr::kReservedPort);
}

TEST(InetAddrTest, Constructor) {
  InetAddr addr(InetAddr::Family::kIpv4, "127.0.0.1", 80);
  EXPECT_EQ(addr.family(), InetAddr::Family::kIpv4);
  EXPECT_EQ(addr.ip(), "127.0.0.1");
  EXPECT_EQ(addr.port(), 80);
}

TEST(InetAddrTest, Vaild1) {
  InetAddr addr;
  EXPECT_FALSE(addr.Valid());
}

TEST(InetAddrTest, Vaild2) {
  InetAddr addr;
  addr.set_family(InetAddr::Family::kIpv4);
  addr.set_ip("127.0.0.1");
  addr.set_port(80);
  EXPECT_TRUE(addr.Valid());
}

TEST(InetAddrTest, Vaild3) {
  InetAddr addr;
  addr.set_family(InetAddr::Family::kIpv6);
  addr.set_ip("::");
  addr.set_port(65535);
  EXPECT_TRUE(addr.Valid());
}

TEST(InetAddrTest, Vaild4) {
  InetAddr addr;
  addr.set_family(InetAddr::Family::kIpv4);
  addr.set_ip("0.0.0.0");
  addr.set_port(0);
  EXPECT_FALSE(addr.Valid());
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ayaka::InitLogger();
  return RUN_ALL_TESTS();
}
