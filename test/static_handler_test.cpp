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

#include <static_handler.hpp>

#include "test.hpp"

using ayaka::Mime;
using ayaka::Request;
using ayaka::Response;
using ayaka::StaticDirHandler;
using ayaka::StaticPathHandler;
using ayaka::Url;
using std::filesystem::path;

TEST(StaticPathHandlerTest, Example) {
  auto mime = std::make_shared<Mime>();
  mime->ext_map()["html"] = "text/html";

  auto html_path =
      path(__FILE__).parent_path().parent_path() / "res/html/index.html";
  auto handler = StaticPathHandler(html_path, mime);
  auto req = std::make_shared<Request>();
  req->set_method(ayaka::http_method::kGet);
  req->set_url(Url("/index.html"));
  req->set_version("HTTP/1.1");

  auto resp = Response::Default();
  handler.Handle(req, resp);
  EXPECT_EQ(resp->version(), "HTTP/1.1");
  EXPECT_EQ(resp->status().code(), "200");
  EXPECT_EQ(resp->status().msg(), "OK");
  EXPECT_EQ(resp->headers().size(), 4);
  EXPECT_EQ(resp->headers()["Content-Type"], "text/html");
}

TEST(StaticDirHandlerTest, Example) {
  auto mime = std::make_shared<Mime>();
  mime->ext_map()["html"] = "text/html";

  auto root = path(__FILE__).parent_path().parent_path();
  auto handler = StaticDirHandler("/", root.string(), mime);
  auto req = std::make_shared<Request>();
  req->set_method(ayaka::http_method::kGet);
  req->set_url(Url("/res/html/index.html"));
  req->set_version("HTTP/1.1");

  auto resp = Response::Default();
  handler.Handle(req, resp);
  EXPECT_EQ(resp->version(), "HTTP/1.1");
  EXPECT_EQ(resp->status().code(), "200");
  EXPECT_EQ(resp->status().msg(), "OK");
  EXPECT_EQ(resp->headers().size(), 4);
  EXPECT_EQ(resp->headers()["Content-Type"], "text/html");
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ayaka::InitLogger();
  return RUN_ALL_TESTS();
}
