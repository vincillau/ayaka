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

#include <sys/socket.h>

#include <tcp.hpp>
#include <utility>

#include "test.hpp"

using namespace ayaka;

TEST(TcpTest, Default) {
  auto tcp = std::make_shared<Tcp>();
  EXPECT_TRUE(tcp->closed());
  EXPECT_TRUE(tcp->on_accept());
  EXPECT_TRUE(tcp->on_close());
  EXPECT_TRUE(tcp->on_recv());
}

TEST(TcpTest, Close) {
  auto loop = std::make_shared<Loop>();
  auto server = std::make_shared<Tcp>(loop);

  EXPECT_FALSE(server->closed());
  server->Close();
  EXPECT_FALSE(server->closed());

  while (!server->closed()) {
    loop->Once();
  }
}

TEST(TcpTest, Bind) {
  auto loop = std::make_shared<Loop>();
  auto ipv4_1 = std::make_shared<Tcp>(loop);
  auto ipv4_2 = std::make_shared<Tcp>(loop);
  auto ipv6_1 = std::make_shared<Tcp>(loop);
  auto ipv6_2 = std::make_shared<Tcp>(loop);

  /* 多个 Tcp 绑定同一个地址，测试端口复用。
   */
  InetAddr ipv4(InetAddr::Family::kIpv4, "0.0.0.0", 8080);
  ipv4_1->Bind(ipv4);
  ipv4_2->Bind(ipv4);
  InetAddr ipv6(InetAddr::Family::kIpv6, "::", 8080);
  ipv6_1->Bind(ipv4);
  ipv6_2->Bind(ipv4);

  ipv4_1->Close();
  ipv4_2->Close();
  ipv6_1->Close();
  ipv6_2->Close();
  while (!(ipv4_1->closed() && ipv4_2->closed() && ipv6_1->closed() &&
           ipv6_2->closed())) {
    loop->Once();
  }
}

TEST(TcpTest, Accept) {
  auto loop = std::make_shared<Loop>();
  auto server = std::make_shared<Tcp>(loop);
  InetAddr addr(InetAddr::Family::kIpv4, "127.0.0.1", 8080);
  server->Bind(addr);

  std::shared_ptr<Tcp> client;
  bool accept_flag = false;
  server->set_on_accept([&accept_flag, &client](std::shared_ptr<Tcp> tcp) {
    accept_flag = true;
    client = std::move(tcp);
  });
  server->Listen();

  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr sock_addr{};
  addr.ToSockAddr(&sock_addr);
  auto status = connect(client_fd, &sock_addr, sizeof(sockaddr));
  EXPECT_EQ(status, 0);
  while (!accept_flag) {
    loop->Once();
  }

  server->Close();
  client->Close();
  while (!(server->closed() && client->closed())) {
    loop->Once();
  }
}

TEST(TcpTest, Recv) {
  auto loop = std::make_shared<Loop>();
  auto server = std::make_shared<Tcp>(loop);
  InetAddr addr(InetAddr::Family::kIpv4, "127.0.0.1", 8080);
  server->Bind(addr);

  std::shared_ptr<Tcp> client;
  bool accept_flag = false;
  server->set_on_accept([&accept_flag, &client](std::shared_ptr<Tcp> tcp) {
    accept_flag = true;
    client = std::move(tcp);
    client->set_on_recv([](const char *data, size_t len) {
      EXPECT_EQ(len, 5);
      EXPECT_MEMEQ(data, "hello", len);
    });
  });
  server->Listen();

  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr sock_addr{};
  addr.ToSockAddr(&sock_addr);
  auto status = connect(client_fd, &sock_addr, sizeof(sockaddr));
  EXPECT_EQ(status, 0);
  auto nwrite = write(client_fd, "hello", 5);
  EXPECT_EQ(nwrite, 5);

  while (!accept_flag) {
    loop->Once();
  }

  server->Close();
  client->Close();
  while (!(server->closed() && client->closed())) {
    loop->Once();
  }
}

TEST(TcpTest, Send) {
  auto loop = std::make_shared<Loop>();
  auto server = std::make_shared<Tcp>(loop);
  InetAddr addr(InetAddr::Family::kIpv4, "127.0.0.1", 8080);
  server->Bind(addr);

  std::shared_ptr<Tcp> client;
  bool accept_flag = false;
  server->set_on_accept([&accept_flag, &client](std::shared_ptr<Tcp> tcp) {
    accept_flag = true;
    client = std::move(tcp);
  });
  server->Listen();

  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr sock_addr{};
  addr.ToSockAddr(&sock_addr);
  auto status = connect(client_fd, &sock_addr, sizeof(sockaddr));
  EXPECT_EQ(status, 0);

  while (!accept_flag) {
    loop->Once();
  }

  auto resp = std::make_shared<Response>();
  resp->set_status(std::move(HttpStatus::Ok()));
  resp->set_version("HTTP/1.1");
  std::shared_ptr<SendPack> send_pack = std::make_shared<SendRespPack>(resp);
  client->Send(send_pack, {});

  while (client->Pending() > 0) {
    loop->Once();
  }

  char buf[1024] = {};
  auto nread = read(client_fd, buf, 1024);
  EXPECT_EQ(nread, strlen("HTTP/1.1 200 OK\r\n\r\n"));
  EXPECT_MEMEQ(buf, "HTTP/1.1 200 OK\r\n\r\n", nread);

  server->Close();
  client->Close();
  while (!(server->closed() && client->closed())) {
    loop->Once();
  }
}

TEST(TcpTest, Hup1) {
  auto loop = std::make_shared<Loop>();
  auto server = std::make_shared<Tcp>(loop);
  InetAddr addr(InetAddr::Family::kIpv4, "127.0.0.1", 8080);
  server->Bind(addr);

  std::shared_ptr<Tcp> client;
  bool accept_flag = false;
  server->set_on_accept([&accept_flag, &client](std::shared_ptr<Tcp> tcp) {
    accept_flag = true;
    client = std::move(tcp);
  });
  server->Listen();

  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr sock_addr{};
  addr.ToSockAddr(&sock_addr);
  auto status = connect(client_fd, &sock_addr, sizeof(sockaddr));
  EXPECT_EQ(status, 0);

  while (!accept_flag) {
    loop->Once();
  }

  // 连接后立即关闭。
  status = close(client_fd);
  EXPECT_EQ(status, 0);

  server->Close();
  client->Close();
  while (!(server->closed() && client->closed())) {
    loop->Once();
  }
}

TEST(TcpTest, Hup2) {
  auto loop = std::make_shared<Loop>();
  auto server = std::make_shared<Tcp>(loop);
  InetAddr addr(InetAddr::Family::kIpv4, "127.0.0.1", 8080);
  server->Bind(addr);

  bool accept_flag = false;
  std::shared_ptr<Tcp> client;
  server->set_on_accept([&accept_flag, &client](std::shared_ptr<Tcp> tcp) {
    accept_flag = true;
    client = std::move(tcp);
    client->set_on_recv([](const char *data, size_t len) {
      EXPECT_EQ(len, 5);
      EXPECT_MEMEQ(data, "hello", len);
    });
  });
  server->Listen();

  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr sock_addr{};
  addr.ToSockAddr(&sock_addr);
  auto status = connect(client_fd, &sock_addr, sizeof(sockaddr));
  EXPECT_EQ(status, 0);

  while (!accept_flag) {
    loop->Once();
  }

  // 发送数据后再关闭。
  auto nwrite = write(client_fd, "hello", 5);
  EXPECT_EQ(nwrite, 5);
  status = close(client_fd);
  EXPECT_EQ(status, 0);

  while (!client->closed()) {
    loop->Once();
  }

  server->Close();
  while (!(server->closed() && client->closed())) {
    loop->Once();
  }
}

TEST(TcpTest, Hup3) {
  auto loop = std::make_shared<Loop>();
  auto server = std::make_shared<Tcp>(loop);
  InetAddr addr(InetAddr::Family::kIpv4, "127.0.0.1", 8080);
  server->Bind(addr);

  std::shared_ptr<Tcp> client;
  bool accept_flag = false;
  server->set_on_accept([&accept_flag, &client](std::shared_ptr<Tcp> tcp) {
    accept_flag = true;
    client = std::move(tcp);
  });
  server->Listen();

  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr sock_addr{};
  addr.ToSockAddr(&sock_addr);
  auto status = connect(client_fd, &sock_addr, sizeof(sockaddr));
  EXPECT_EQ(status, 0);

  while (!accept_flag) {
    loop->Once();
  }

  // 连接后立即关闭。
  status = close(client_fd);
  EXPECT_EQ(status, 0);

  // 尝试发送数据。
  auto resp = std::make_shared<Response>();
  resp->set_status(std::move(HttpStatus::Ok()));
  resp->set_version("HTTP/1.1");
  std::shared_ptr<SendPack> send_pack = std::make_shared<SendRespPack>(resp);
  client->Send(send_pack, {});

  while (!client->closed()) {
    loop->Once();
  }

  server->Close();
  while (!(server->closed())) {
    loop->Once();
  }
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ayaka::InitLogger();
  return RUN_ALL_TESTS();
}
