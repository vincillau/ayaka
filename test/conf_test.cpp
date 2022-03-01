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

#include <conf.hpp>

#include "test.hpp"

using ayaka::InetAddr;
using ayaka::ListenConf;
using ayaka::ServerConf;

TEST(ListenConfTest, Default) {
  ListenConf conf;
  EXPECT_EQ(conf.family(), ListenConf::kDefaultFamily);
  EXPECT_EQ(conf.ip(), ListenConf::kDefaultIp);
  EXPECT_EQ(conf.port(), ListenConf::kDefaultPort);
}

TEST(ListenConf, Valid_1) {
  ListenConf conf;
  EXPECT_TRUE(conf.Valid());
  conf.set_family("ipv6");
  conf.set_ip("::");
  conf.set_port(80);
  EXPECT_TRUE(conf.Valid());
  conf.set_family("Ipv4");
  conf.set_ip("0.0.0.0");
  conf.set_port(65535);
  EXPECT_TRUE(conf.Valid());
  conf.set_family("ipv4");
  conf.set_ip("127.0.0.1");
  conf.set_port(0);
  EXPECT_FALSE(conf.Valid());
  conf.set_family("ipv4");
  conf.set_ip("127.0.0.1");
  conf.set_port(65536);
  EXPECT_FALSE(conf.Valid());
  conf.set_family("ipv4");
  conf.set_ip("127.0.0.256");
  conf.set_port(8080);
  EXPECT_FALSE(conf.Valid());
  conf.set_family("ipv4");
  conf.set_ip("12345");
  conf.set_port(8080);
  EXPECT_FALSE(conf.Valid());
}

TEST(ListenConf, Valid_2) {
  ListenConf conf;
  conf.set_family("abcd");
  conf.set_ip("127.0.0.1");
  conf.set_port(80);
  EXPECT_DEBUG_DEATH((void)conf.Valid(), ".*");
}

TEST(ListenConf, ToInetAddr) {
  ListenConf conf;
  conf.set_family("ipv4");
  conf.set_ip("0.0.0.0");
  conf.set_port(80);
  EXPECT_EQ(conf.ToInetAddr().family(), InetAddr::Family::kIpv4);
  EXPECT_EQ(conf.ToInetAddr().ip(), "0.0.0.0");
  EXPECT_EQ(conf.ToInetAddr().port(), 80);
  conf.set_family("Ipv6");
  conf.set_ip("::");
  conf.set_port(8080);
  EXPECT_EQ(conf.ToInetAddr().family(), InetAddr::Family::kIpv6);
  EXPECT_EQ(conf.ToInetAddr().ip(), "::");
  EXPECT_EQ(conf.ToInetAddr().port(), 8080);
}

TEST(ServerConfTest, Default) {
  ServerConf conf;
  EXPECT_EQ(conf.listen().family(), ListenConf::kDefaultFamily);
  EXPECT_EQ(conf.listen().ip(), ListenConf::kDefaultIp);
  EXPECT_EQ(conf.listen().port(), ListenConf::kDefaultPort);
  EXPECT_EQ(conf.worker_threads(), ServerConf::kDefaultWorkerThreads);
}

TEST(ServerConfTest, Setter) {
  ServerConf conf;
  conf.listen().set_family("ipv4");
  conf.listen().set_ip("127.0.0.1");
  conf.listen().set_port(8080);
  EXPECT_EQ(conf.listen().family(), "ipv4");
  EXPECT_EQ(conf.listen().ip(), "127.0.0.1");
  EXPECT_EQ(conf.listen().port(), 8080);
  conf.set_worker_threads(10);
  EXPECT_EQ(conf.worker_threads(), 10);
}

TEST(ServerConfTest, Valid) {
  ServerConf conf;
  EXPECT_TRUE(conf.Valid());
  conf.listen().set_family("ipv6");
  conf.listen().set_ip("::");
  conf.listen().set_port(80);
  EXPECT_TRUE(conf.Valid());
  conf.set_worker_threads(ServerConf::kMaxWorkerThreads + 1);
  EXPECT_FALSE(conf.Valid());
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ayaka::InitLogger();
  return RUN_ALL_TESTS();
}
