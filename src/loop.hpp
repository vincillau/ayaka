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

#ifndef AYAKA_SRC_LOOP_HPP_
#define AYAKA_SRC_LOOP_HPP_

#include <uv.h>

#include <memory>

namespace ayaka {

class Loop {
 public:
  Loop();

  /**
   * Loop 不可以被拷贝或移动。
   */
  Loop(const Loop &) = delete;
  Loop &operator=(const Loop &) = delete;
  Loop(Loop &&) = delete;
  Loop &operator=(Loop &&) = delete;

  ~Loop();

  [[nodiscard]] auto uv_loop() const { return uv_loop_; }

  void Run() const;
  void Once() const;

 private:
  static void UvWalkClose(uv_handle_t *handle, void *arg);

  uv_loop_t *uv_loop_;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_LOOP_HPP_
