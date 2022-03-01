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

#ifndef AYAKA_SRC_LOGGER_HPP_
#define AYAKA_SRC_LOGGER_HPP_

#include <fmt/fmt.h>
#include <spdlog/spdlog.h>

/**
 * 所有的日志宏函数都是线程安全的。
 */

#define LOG_LEVEL_TRACE 0
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_WARN 3
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_CRITICAL 5
#define LOG_LEVEL_OFF 6

#if (LOG_LEVEL <= LOG_LEVEL_TRACE)
#define AYAKA_LOG_TRACE(...) \
  spdlog::trace("{}:{} {}", __FILE__, __LINE__, fmt::format(__VA_ARGS__))
#else
#define AYAKA_LOG_TRACE(...)
#endif

#if (LOG_LEVEL <= LOG_LEVEL_DEBUG)
#define AYAKA_LOG_DEBUG(...) \
  spdlog::debug("{}:{} {}", __FILE__, __LINE__, fmt::format(__VA_ARGS__))
#else
#define AYAKA_LOG_DEBUG(...)
#endif

#if (LOG_LEVEL <= LOG_LEVEL_INFO)
#define AYAKA_LOG_INFO(...) spdlog::info(__VA_ARGS__)
#else
#define AYAKA_LOG_INFO(...)
#endif

#if (LOG_LEVEL <= LOG_LEVEL_WARN)
#define AYAKA_LOG_WARN(...) spdlog::warn(__VA_ARGS__)
#else
#define AYAKA_LOG_WARN(...)
#endif

#if (LOG_LEVEL <= LOG_LEVEL_ERROR)
#define AYAKA_LOG_ERROR(...) spdlog::error(__VA_ARGS__)
#else
#define AYAKA_LOG_ERROR(...)
#endif

#if (LOG_LEVEL <= LOG_LEVEL_CRITICAL)
#define AYAKA_LOG_CRITICAL(...)  \
  spdlog::critical(__VA_ARGS__); \
  std::terminate()
#else
#define AYAKA_LOG_CRITICAL(...)
#endif

#endif  // AYAKA_SRC_LOGGER_HPP_
