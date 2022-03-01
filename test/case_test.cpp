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

#include <case.hpp>

#include "test.hpp"

using ayaka::StrEqualIgnoreCase;

TEST(CaseTest, StrEqualIgnoreCase) {
  EXPECT_TRUE(StrEqualIgnoreCase("", ""));
  EXPECT_TRUE(StrEqualIgnoreCase("abc", "abc"));
  EXPECT_TRUE(StrEqualIgnoreCase("abc", "ABC"));
  EXPECT_TRUE(StrEqualIgnoreCase("aBc", "AbC"));
  EXPECT_TRUE(StrEqualIgnoreCase("a+-*/b1234c", "A+-*/b1234C"));
  EXPECT_TRUE(StrEqualIgnoreCase("AB简体中文cd", "ab简体中文CD"));
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  ayaka::InitLogger();
  return RUN_ALL_TESTS();
}
