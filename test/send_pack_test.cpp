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

#include <send_pack.hpp>

#include "test.hpp"

using ayaka::HttpStatus;
using ayaka::Response;
using ayaka::SendRespPack;

TEST(SendRespPackTest, Default) {
  SendRespPack pack;
  EXPECT_EQ(pack.uv_write(), nullptr);
  EXPECT_EQ(pack.resp(), nullptr);
  EXPECT_TRUE(pack.bufs().empty());
  EXPECT_FALSE(pack.on_finish());
}

TEST(SendRespPackTest, Constructor) {
  auto resp = std::make_shared<Response>();
  resp->set_version("HTTP/1.1");
  resp->set_status(HttpStatus::Ok());

  SendRespPack pack(resp);
  EXPECT_NE(pack.uv_write(), nullptr);
  EXPECT_EQ(pack.resp(), resp);
  EXPECT_FALSE(pack.on_finish());

  EXPECT_EQ(pack.bufs().size(), 7);
  EXPECT_EQ(pack.bufs()[0].len, strlen("HTTP/1.1"));
  EXPECT_MEMEQ(pack.bufs()[0].base, "HTTP/1.1", pack.bufs()[0].len);
  EXPECT_EQ(pack.bufs()[1].len, strlen(" "));
  EXPECT_MEMEQ(pack.bufs()[1].base, " ", pack.bufs()[1].len);
  EXPECT_EQ(pack.bufs()[2].len, strlen("200"));
  EXPECT_MEMEQ(pack.bufs()[2].base, "200", pack.bufs()[2].len);
  EXPECT_EQ(pack.bufs()[3].len, strlen(" "));
  EXPECT_MEMEQ(pack.bufs()[3].base, " ", pack.bufs()[3].len);
  EXPECT_EQ(pack.bufs()[4].len, strlen("OK"));
  EXPECT_MEMEQ(pack.bufs()[4].base, "OK", pack.bufs()[4].len);
  EXPECT_EQ(pack.bufs()[5].len, strlen("\r\n"));
  EXPECT_MEMEQ(pack.bufs()[5].base, "\r\n", pack.bufs()[5].len);
  EXPECT_EQ(pack.bufs()[6].len, strlen("\r\n"));
  EXPECT_MEMEQ(pack.bufs()[6].base, "\r\n", pack.bufs()[6].len);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ayaka::InitLogger();
  return RUN_ALL_TESTS();
}
