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

#include "loop.hpp"

#include "error.hpp"

namespace ayaka {

Loop::Loop() : uv_loop_(new uv_loop_t) {
  auto status = uv_loop_init(uv_loop_);
  AYAKA_TERM_IF(status < 0, "uv_loop_init() failed: {}", uv_strerror(status));
}

Loop::~Loop() {
  uv_walk(uv_loop_, UvWalkClose, nullptr);
  auto status = uv_run(uv_loop_, UV_RUN_DEFAULT);
  AYAKA_TERM_IF(status != 0, "uv_run() failed: {}", uv_strerror(status));
  status = uv_loop_close(uv_loop_);
  AYAKA_TERM_IF(status != 0, "uv_loop_close() failed: {}", uv_strerror(status));
  delete uv_loop_;
}

void Loop::Run() const {
  auto status = uv_run(uv_loop_, UV_RUN_DEFAULT);
  AYAKA_TERM_IF(status != 0, "uv_run() failed: {}", uv_strerror(status));
}

void Loop::Once() const { uv_run(uv_loop_, UV_RUN_ONCE); }

void Loop::UvWalkClose(uv_handle_t *handle, [[maybe_unused]] void *arg) {
  if (uv_is_closing(handle) != 0) {
    return;
  }
  uv_close(handle, nullptr);
}

}  // namespace ayaka
