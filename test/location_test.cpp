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

#include <location.hpp>

#include "test.hpp"

using ayaka::DirLocation;
using ayaka::PathLocation;

TEST(PathLocationTest, Simple1) {
  auto location = std::make_shared<PathLocation>("", nullptr);
  EXPECT_FALSE(location->Match("/"));
  EXPECT_FALSE(location->Match("/index.html"));
  EXPECT_FALSE(location->Match("/foo/bar"));
}

TEST(PathLocationTest, Simple2) {
  auto location = std::make_shared<PathLocation>("/", nullptr);
  EXPECT_TRUE(location->Match("/"));
  EXPECT_FALSE(location->Match("/index.html"));
}

TEST(PathLocationTest, Simple3) {
  auto location = std::make_shared<PathLocation>("/foo/bar", nullptr);
  EXPECT_TRUE(location->Match("/foo/bar"));
  EXPECT_FALSE(location->Match("/foo/bar/"));
}

TEST(PathLocationTest, Simple4) {
  auto location = std::make_shared<PathLocation>("/foo/bar/", nullptr);
  EXPECT_FALSE(location->Match("/foo/bar"));
  EXPECT_TRUE(location->Match("/foo/bar/"));
}

TEST(PathLocationTest, Simple5) {
  auto location = std::make_shared<PathLocation>("/foo/bar/../bar", nullptr);
  EXPECT_TRUE(location->Match("/foo/bar"));
  EXPECT_FALSE(location->Match("/foo/bar/"));
}

TEST(PathLocationTest, Simple6) {
  auto location = std::make_shared<PathLocation>("/foo/bar/../bar/", nullptr);
  EXPECT_FALSE(location->Match("/foo/bar"));
  EXPECT_TRUE(location->Match("/foo/bar/"));
}

TEST(DirLocationTest, Simple1) {
  auto location = std::make_shared<DirLocation>("/foo/bar", nullptr);
  EXPECT_TRUE(location->Match("/foo/bar"));
  EXPECT_TRUE(location->Match("/foo/bar/"));
  EXPECT_TRUE(location->Match("/foo/bar/index.html"));
}

TEST(DirLocationTest, Simple2) {
  auto location = std::make_shared<DirLocation>("/foo/bar/", nullptr);
  EXPECT_FALSE(location->Match("/foo/bar"));
  EXPECT_TRUE(location->Match("/foo/bar/"));
  EXPECT_TRUE(location->Match("/foo/bar/index.html"));
}

TEST(DirLocationTest, Simple3) {
  auto location = std::make_shared<DirLocation>("/", nullptr);
  EXPECT_TRUE(location->Match("/"));
  EXPECT_TRUE(location->Match("/index.html"));
  EXPECT_TRUE(location->Match("/foo/bar"));
  EXPECT_TRUE(location->Match("/foo/bar/"));
  EXPECT_TRUE(location->Match("/foo/bar/index.html"));
}

TEST(DirLocationTest, Simple4) {
  auto location = std::make_shared<DirLocation>("/../", nullptr);
  EXPECT_TRUE(location->Match("/"));
  EXPECT_TRUE(location->Match("/index.html"));
  EXPECT_TRUE(location->Match("/foo/bar"));
  EXPECT_TRUE(location->Match("/foo/bar/"));
  EXPECT_TRUE(location->Match("/foo/bar/index.html"));
}

TEST(DirLocationTest, Simple5) {
  auto location = std::make_shared<DirLocation>("", nullptr);
  EXPECT_FALSE(location->Match("/"));
  EXPECT_FALSE(location->Match("/index.html"));
  EXPECT_FALSE(location->Match("/foo/bar"));
  EXPECT_FALSE(location->Match("/foo/bar/"));
  EXPECT_FALSE(location->Match("/foo/bar/index.html"));
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ayaka::InitLogger();
  return RUN_ALL_TESTS();
}
