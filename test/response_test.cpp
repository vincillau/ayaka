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

#include <regex>
#include <response.hpp>
#include <version.hpp>

#include "test.hpp"

using ayaka::Response;

TEST(ResponseTest, Default) {
  auto req = Response::Default();
  EXPECT_EQ(req->version(), "HTTP/1.1");
  EXPECT_EQ(req->status().code(), "200");
  EXPECT_EQ(req->status().msg(), "OK");
  EXPECT_EQ(req->headers().size(), 3);
  EXPECT_EQ(req->headers()["Connection"], "close");
  EXPECT_EQ(req->headers()["Server"], "Ayaka/" AYAKA_VERSION);
  EXPECT_FALSE(req->headers()["Date"].empty());  // 不检查 Date 的值。
}

TEST(ResponseTest, Date) {
  time_t t;
  std::string date;

  t = 1640995200;  // Sat, 01 Jan 2022 00:00:00 GMT
  date = Response::GetDate(&t);
  EXPECT_EQ(date, "Sat, 01 Jan 2022 00:00:00 GMT");

  t = 1643673600;  // Tue, 01 Feb 2022 00:00:00 GMT
  date = Response::GetDate(&t);
  EXPECT_EQ(date, "Tue, 01 Feb 2022 00:00:00 GMT");

  t = 1646092800;  // Tue, 01 Mar 2022 00:00:00 GMT
  date = Response::GetDate(&t);
  EXPECT_EQ(date, "Tue, 01 Mar 2022 00:00:00 GMT");

  t = 1648771200;  // Fri, 01 Apr 2022 00:00:00 GMT
  date = Response::GetDate(&t);
  EXPECT_EQ(date, "Fri, 01 Apr 2022 00:00:00 GMT");

  t = 1651363200;  // Sun, 01 May 2022 00:00:00 GMT
  date = Response::GetDate(&t);
  EXPECT_EQ(date, "Sun, 01 May 2022 00:00:00 GMT");

  t = 1654041600;  // Wed, 01 Jun 2022 00:00:00 GMT
  date = Response::GetDate(&t);
  EXPECT_EQ(date, "Wed, 01 Jun 2022 00:00:00 GMT");

  t = 1656633600;  // Fri, 01 Jul 2022 00:00:00 GMT
  date = Response::GetDate(&t);
  EXPECT_EQ(date, "Fri, 01 Jul 2022 00:00:00 GMT");

  t = 1659312000;  // Mon, 01 Aug 2022 00:00:00 GMT
  date = Response::GetDate(&t);
  EXPECT_EQ(date, "Mon, 01 Aug 2022 00:00:00 GMT");

  t = 1661990400;  // Thu, 01 Sep 2022 00:00:00 GMT
  date = Response::GetDate(&t);
  EXPECT_EQ(date, "Thu, 01 Sep 2022 00:00:00 GMT");

  t = 1664582400;  // Sat, 01 Oct 2022 00:00:00 GMT
  date = Response::GetDate(&t);
  EXPECT_EQ(date, "Sat, 01 Oct 2022 00:00:00 GMT");

  t = 1667260800;  // Tue, 01 Nov 2022 00:00:00 GMT
  date = Response::GetDate(&t);
  EXPECT_EQ(date, "Tue, 01 Nov 2022 00:00:00 GMT");

  t = 1669852800;  // Thu, 01 Dec 2022 00:00:00 GMT
  date = Response::GetDate(&t);
  EXPECT_EQ(date, "Thu, 01 Dec 2022 00:00:00 GMT");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ayaka::InitLogger();
  return RUN_ALL_TESTS();
}
