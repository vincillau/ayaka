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

#include <http_except.hpp>

#include "test.hpp"

using ayaka::Http400Except;
using ayaka::Http404Except;
using ayaka::Http405Except;
using ayaka::Response;

TEST(Http400ExceptTest, Example) {
  Http400Except except;
  auto resp = Response::Default();
  except.SetUp(resp);
  EXPECT_EQ(resp->status().code(), "400");
  EXPECT_EQ(resp->status().msg(), "Bad Request");
}

TEST(Http404ExceptTest, Example) {
  Http404Except except("GET", "/");
  auto resp = Response::Default();
  except.SetUp(resp);
  EXPECT_EQ(resp->status().code(), "404");
  EXPECT_EQ(resp->status().msg(), "Not Found");
}

TEST(Http405ExceptTest, Example) {
  Http405Except except("TEST", "/");
  auto resp = Response::Default();
  except.SetUp(resp);
  EXPECT_EQ(resp->status().code(), "405");
  EXPECT_EQ(resp->status().msg(), "Method Not Allowed");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ayaka::InitLogger();
  return RUN_ALL_TESTS();
}