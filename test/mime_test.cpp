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

#include <filesystem>
#include <mime.hpp>

#include "test.hpp"

using ayaka::Mime;
using std::filesystem::path;

TEST(MimeTypeTest, Default1) {
  Mime mime;
  EXPECT_EQ(mime.default_mime(), "application/octet-stream");
  EXPECT_EQ(mime.GetMime(""), "application/octet-stream");
  EXPECT_EQ(mime.GetMime("txt"), "application/octet-stream");
  mime.set_default_mime("text/plain");
  EXPECT_EQ(mime.default_mime(), "text/plain");
  EXPECT_EQ(mime.GetMime(""), "text/plain");
  EXPECT_EQ(mime.GetMime("txt"), "text/plain");
}

TEST(MimeTypeTest, Default2) {
  Mime mime("text/plain");
  EXPECT_EQ(mime.default_mime(), "text/plain");
  EXPECT_EQ(mime.GetMime(""), "text/plain");
  EXPECT_EQ(mime.GetMime("txt"), "text/plain");
}

TEST(MimeTypeTest, Load) {
  auto mime_path =
      path(__FILE__).parent_path().parent_path() / "res/mime.types";
  Mime mime("text/plain");
  mime.Load(mime_path.string());
  EXPECT_EQ(mime.GetMime(""), "text/plain");
  EXPECT_EQ(mime.GetMime("txt"), "text/plain");
  EXPECT_EQ(mime.GetMime("html"), "text/html");
  EXPECT_EQ(mime.GetMime("htm"), "text/html");
  EXPECT_EQ(mime.GetMime("css"), "text/css");
  EXPECT_EQ(mime.GetMime("js"), "application/javascript");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ayaka::InitLogger();
  return RUN_ALL_TESTS();
}
