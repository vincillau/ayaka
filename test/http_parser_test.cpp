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
#include <http_parser.hpp>

#include "test.hpp"

using ayaka::HeaderParser;
using ayaka::Http400Except;
using ayaka::RequestParser;
using ayaka::RequestStartLineParser;

TEST(RequestStartLineParserTest, Simple1) {
  RequestStartLineParser parser;
  std::string str = "GET / HTTP/1.1\r\n";
  for (char ch : str) {
    parser.Step(ch);
  }

  EXPECT_EQ(parser.method(), "GET");
  EXPECT_EQ(parser.url(), "/");
  EXPECT_EQ(parser.version(), "HTTP/1.1");
  EXPECT_EQ(parser.state(), RequestStartLineParser::State::kDone);
}

TEST(RequestStartLineParserTest, Simple2) {
  RequestStartLineParser parser;
  std::string str = "POST /foo/bar HTTP/2.0\r\n";
  for (char ch : str) {
    parser.Step(ch);
  }

  EXPECT_EQ(parser.method(), "POST");
  EXPECT_EQ(parser.url(), "/foo/bar");
  EXPECT_EQ(parser.version(), "HTTP/2.0");
  EXPECT_EQ(parser.state(), RequestStartLineParser::State::kDone);
}

TEST(RequestStartLineParserTest, Simple3) {
  RequestStartLineParser parser;
  std::string str = "POST /foo/bar HTTP/2.0\r\r";
  EXPECT_THROW(
      for (char ch
           : str) { parser.Step(ch); },
      Http400Except);
}

TEST(HeaderParserTest, Simple1) {
  HeaderParser parser;
  std::string str = "\r\n";
  for (char ch : str) {
    parser.Step(ch);
  }

  EXPECT_TRUE(parser.headers().empty());
  EXPECT_EQ(parser.state(), HeaderParser::State::kDone);
}

TEST(HeaderParserTest, Simple2) {
  HeaderParser parser;
  std::string str = "Host: example.com\r\n\r\n";
  for (char ch : str) {
    parser.Step(ch);
  }

  EXPECT_EQ(parser.headers().size(), 1);
  EXPECT_EQ(parser.headers()["host"], "example.com");
  EXPECT_EQ(parser.state(), HeaderParser::State::kDone);
}

TEST(HeaderParserTest, Simple3) {
  HeaderParser parser;
  std::string str = "Host:example.com\r\n\r\n";
  for (char ch : str) {
    parser.Step(ch);
  }

  EXPECT_EQ(parser.headers().size(), 1);
  EXPECT_EQ(parser.headers()["host"], "example.com");
  EXPECT_EQ(parser.state(), HeaderParser::State::kDone);
}

TEST(HeaderParserTest, Simple4) {
  HeaderParser parser;
  std::string str = "Host: example.com\r\nContent-Length: 0\r\n\r\n";
  for (char ch : str) {
    parser.Step(ch);
  }

  EXPECT_EQ(parser.headers().size(), 2);
  EXPECT_EQ(parser.headers()["host"], "example.com");
  EXPECT_EQ(parser.headers()["content-length"], "0");
  EXPECT_EQ(parser.state(), HeaderParser::State::kDone);
}

TEST(HeaderParserTest, Simple5) {
  HeaderParser parser;
  std::string str = "Host: example.com\r\r\r\n";
  EXPECT_THROW(
      for (char ch
           : str) { parser.Step(ch); },
      Http400Except);
}

TEST(HeaderParserTest, Simple6) {
  HeaderParser parser;
  std::string str = "Host: example.com\r\n\r\r";
  EXPECT_THROW(
      for (char ch
           : str) { parser.Step(ch); },
      Http400Except);
}

TEST(RequestParserTest, Simple1) {
  RequestParser parser;
  std::string str = "GET / HTTP/1.1\r\n\r\n";
  auto nparsed = parser.Exec(str.c_str(), str.size());
  EXPECT_EQ(nparsed, str.size());
  EXPECT_EQ(parser.state(), RequestParser::State::kDone);

  auto req = parser.req();
  EXPECT_EQ(req->method(), ayaka::http_method::kGet);
  EXPECT_EQ(req->url().scheme(), "");
  EXPECT_EQ(req->url().host(), "");
  EXPECT_EQ(req->url().port(), 0);
  EXPECT_EQ(req->url().path().string(), "/");
  EXPECT_TRUE(req->url().query().Empty());
  EXPECT_EQ(req->version(), "HTTP/1.1");
  EXPECT_TRUE(req->headers().empty());
}

TEST(RequestParserTest, Simple2) {
  RequestParser parser;
  std::string str =
      "POST https://example.com:443/foo/bar?a=1&b=2 HTTP/1.1\r\nHost: "
      "example.com\r\nContent-Type: text/plain\r\n\r\n";
  auto nparsed = parser.Exec(str.c_str(), str.size());
  EXPECT_EQ(nparsed, str.size());
  EXPECT_EQ(parser.state(), RequestParser::State::kDone);

  auto req = parser.req();
  EXPECT_EQ(req->method(), ayaka::http_method::kPost);
  EXPECT_EQ(req->url().scheme(), "https");
  EXPECT_EQ(req->url().host(), "example.com");
  EXPECT_EQ(req->url().port(), 443);
  EXPECT_EQ(req->url().path().string(), "/foo/bar");
  EXPECT_EQ(req->url().query().Size(), 2);
  EXPECT_EQ(req->version(), "HTTP/1.1");
  EXPECT_EQ(req->headers().size(), 2);
  EXPECT_EQ(req->headers()["host"], "example.com");
  EXPECT_EQ(req->headers()["content-type"], "text/plain");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ayaka::InitLogger();
  return RUN_ALL_TESTS();
}
