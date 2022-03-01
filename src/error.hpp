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

#ifndef AYAKA_SRC_ERROR_HPP_
#define AYAKA_SRC_ERROR_HPP_

#include <uv.h>

#include "logger.hpp"

/**
 * 如果 expr 为 true，则输入日志信息并终止程序。
 */
#define AYAKA_TERM_IF(expr, ...)                         \
  if (expr) {                                            \
    AYAKA_LOG_CRITICAL("AYAKA_TERM_IF: [{}], {}", #expr, \
                       fmt::format(__VA_ARGS__));        \
  }

namespace ayaka {

/**
 * 该函数线程安全。
 */
static inline std::string LastError() {
  char buf[256];
  strerror_r(errno, buf, 256);
  return {buf};
}

/**
 * 该函数线程安全。
 */
static inline std::string UvLastError(int status) {
  char buf[256];
  uv_strerror_r(status, buf, 256);
  return {buf};
}

}  // namespace ayaka

#endif  // AYAKA_SRC_ERROR_HPP_
