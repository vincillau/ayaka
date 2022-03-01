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

#include <http_status.hpp>

#include "test.hpp"

using ayaka::HttpStatus;

TEST(HttpStatusTest, Default) {
  HttpStatus status;
  EXPECT_EQ(status.code(), "");
  EXPECT_EQ(status.msg(), "");
}

TEST(HttpStatusTest, Constructor) {
  HttpStatus status("200", "OK");
  EXPECT_EQ(status.code(), "200");
  EXPECT_EQ(status.msg(), "OK");
}

TEST(HttpStatusTest, Predefined) {
  EXPECT_EQ(HttpStatus::Ok().code(), "200");
  EXPECT_EQ(HttpStatus::Ok().msg(), "OK");
  EXPECT_EQ(HttpStatus::BadRequest().code(), "400");
  EXPECT_EQ(HttpStatus::BadRequest().msg(), "Bad Request");
  EXPECT_EQ(HttpStatus::NotFound().code(), "404");
  EXPECT_EQ(HttpStatus::NotFound().msg(), "Not Found");
  EXPECT_EQ(HttpStatus::MethodNotAllowed().code(), "405");
  EXPECT_EQ(HttpStatus::MethodNotAllowed().msg(), "Method Not Allowed");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ayaka::InitLogger();
  return RUN_ALL_TESTS();
}
