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

#ifndef AYAKA_TEST_TEST_HPP_
#define AYAKA_TEST_TEST_HPP_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cctype>
#include <cstring>
#include <logger.hpp>
#include <sstream>
#include <string>

#define EXPECT_MEMEQ(mem1, mem2, len)                                         \
  EXPECT_EQ(memcmp(static_cast<const void*>(mem1),                            \
                   static_cast<const void*>(mem2), static_cast<size_t>(len)), \
            0)                                                                \
      << "length: " << (len) << std::endl                                     \
      << "mem1: " << ayaka::MemToStr((mem1), (len)) << std::endl              \
      << "mem2: " << ayaka::MemToStr((mem2), (len))

#define ASSERT_MEMEQ(mem1, mem2, len)                                         \
  ASSERT_EQ(memcmp(static_cast<const void*>(mem1),                            \
                   static_cast<const void*>(mem2), static_cast<size_t>(len)), \
            0)

namespace ayaka {

static inline void InitLogger() {
  spdlog::flush_on(spdlog::level::trace);
  spdlog::set_level(spdlog::level::trace);
  spdlog::set_pattern("[%Y-%m-%d %T.%e] [%^%l%$] tid=%t %v");
}

static inline std::string MemToStr(const char* mem, size_t len) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (size_t i = 0; i < len; i++) {
    if (std::isprint(mem[i]) != 0) {
      ss << mem[i];
      continue;
    }
    ss << "\\x" << static_cast<int>(mem[i]);
  }
  return std::move(ss.str());
}

}  // namespace ayaka

#endif  // AYAKA_TEST_TEST_HPP_
