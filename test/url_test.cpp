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

#include <url.hpp>

#include "test.hpp"

using ayaka::Url;

TEST(UrlTest, Default) {
  Url url;
  EXPECT_EQ(url.scheme(), "");
  EXPECT_EQ(url.host(), "");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path(), "/");
  EXPECT_EQ(url.query().Size(), 0);
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple1) {
  Url url("/");
  EXPECT_EQ(url.scheme(), "");
  EXPECT_EQ(url.host(), "");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path(), "/");
  EXPECT_TRUE(url.query().Empty());
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple2) {
  Url url("/index.html");
  EXPECT_EQ(url.scheme(), "");
  EXPECT_EQ(url.host(), "");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/index.html");
  EXPECT_TRUE(url.query().Empty());
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple3) {
  Url url("/foo/bar");
  EXPECT_EQ(url.scheme(), "");
  EXPECT_EQ(url.host(), "");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/foo/bar");
  EXPECT_TRUE(url.query().Empty());
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple4) {
  Url url("/foo/bar/");
  EXPECT_EQ(url.scheme(), "");
  EXPECT_EQ(url.host(), "");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/foo/bar/");
  EXPECT_TRUE(url.query().Empty());
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple5) {
  Url url("/foo/.././../bar");
  EXPECT_EQ(url.scheme(), "");
  EXPECT_EQ(url.host(), "");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/bar");
  EXPECT_TRUE(url.query().Empty());
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple6) {
  Url url("/foo/bar?a=1&b=2");
  EXPECT_EQ(url.scheme(), "");
  EXPECT_EQ(url.host(), "");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/foo/bar");
  EXPECT_EQ(url.query().Size(), 2);
  EXPECT_THAT(url.query().Get("a"), testing::ElementsAre("1"));
  EXPECT_THAT(url.query().Get("b"), testing::ElementsAre("2"));
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple7) {
  Url url("http://example.com");
  EXPECT_EQ(url.scheme(), "http");
  EXPECT_EQ(url.host(), "example.com");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/");
  EXPECT_TRUE(url.query().Empty());
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple8) {
  Url url("http://example.com/");
  EXPECT_EQ(url.scheme(), "http");
  EXPECT_EQ(url.host(), "example.com");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/");
  EXPECT_TRUE(url.query().Empty());
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple9) {
  Url url("https://www.example.com:443");
  EXPECT_EQ(url.scheme(), "https");
  EXPECT_EQ(url.host(), "www.example.com");
  EXPECT_EQ(url.port(), 443);
  EXPECT_EQ(url.path().string(), "/");
  EXPECT_TRUE(url.query().Empty());
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple10) {
  Url url("http://example.com/index.html");
  EXPECT_EQ(url.scheme(), "http");
  EXPECT_EQ(url.host(), "example.com");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/index.html");
  EXPECT_TRUE(url.query().Empty());
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple11) {
  Url url("http://example.com:8080/foo/bar");
  EXPECT_EQ(url.scheme(), "http");
  EXPECT_EQ(url.host(), "example.com");
  EXPECT_EQ(url.port(), 8080);
  EXPECT_EQ(url.path().string(), "/foo/bar");
  EXPECT_TRUE(url.query().Empty());
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple12) {
  Url url("http://example.com/..");
  EXPECT_EQ(url.scheme(), "http");
  EXPECT_EQ(url.host(), "example.com");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/");
  EXPECT_TRUE(url.query().Empty());
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple13) {
  Url url("http://example.com:8080/foo/bar?a=1&b=2");
  EXPECT_EQ(url.scheme(), "http");
  EXPECT_EQ(url.host(), "example.com");
  EXPECT_EQ(url.port(), 8080);
  EXPECT_EQ(url.path().string(), "/foo/bar");
  EXPECT_EQ(url.query().Size(), 2);
  EXPECT_THAT(url.query().Get("a"), testing::ElementsAre("1"));
  EXPECT_THAT(url.query().Get("b"), testing::ElementsAre("2"));
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple14) {
  Url url("/foo/bar?");
  EXPECT_EQ(url.scheme(), "");
  EXPECT_EQ(url.host(), "");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/foo/bar");
  EXPECT_TRUE(url.query().Empty());
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple15) {
  Url url("/foo/bar?a=1&b=2&");
  EXPECT_EQ(url.scheme(), "");
  EXPECT_EQ(url.host(), "");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/foo/bar");
  EXPECT_EQ(url.query().Size(), 2);
  EXPECT_THAT(url.query().Get("a"), testing::ElementsAre("1"));
  EXPECT_THAT(url.query().Get("b"), testing::ElementsAre("2"));
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple16) {
  Url url("/foo/bar?a=1&b=");
  EXPECT_EQ(url.scheme(), "");
  EXPECT_EQ(url.host(), "");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/foo/bar");
  EXPECT_EQ(url.query().Size(), 2);
  EXPECT_THAT(url.query().Get("a"), testing::ElementsAre("1"));
  EXPECT_THAT(url.query().Get("b"), testing::ElementsAre(""));
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple17) {
  Url url("/foo/bar?a=1&=2");
  EXPECT_EQ(url.scheme(), "");
  EXPECT_EQ(url.host(), "");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/foo/bar");
  EXPECT_EQ(url.query().Size(), 2);
  EXPECT_THAT(url.query().Get("a"), testing::ElementsAre("1"));
  EXPECT_THAT(url.query().Get(""), testing::ElementsAre("2"));
  EXPECT_TRUE(url.valid());
}

TEST(UrlTest, Simple18) {
  Url url("/foo/bar?a=1&a=2&b=3&a=4&b=5");
  EXPECT_EQ(url.scheme(), "");
  EXPECT_EQ(url.host(), "");
  EXPECT_EQ(url.port(), 0);
  EXPECT_EQ(url.path().string(), "/foo/bar");
  EXPECT_EQ(url.query().Size(), 5);
  EXPECT_THAT(url.query().Get("a"), testing::ElementsAre("1", "2", "4"));
  EXPECT_THAT(url.query().Get("b"), testing::ElementsAre("3", "5"));
  EXPECT_TRUE(url.valid());
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ayaka::InitLogger();
  return RUN_ALL_TESTS();
}
