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

#ifndef AYAKA_SRC_SEND_PACK_HPP_
#define AYAKA_SRC_SEND_PACK_HPP_

#include <uv.h>

#include <functional>

#include "response.hpp"

namespace ayaka {

/**
 * 因为实现原因，SendPack 对象只能在同一个线程中使用。
 */
class SendPack {
 public:
  using OnFinishCb = std::function<void(int)>;

  /**
   * 默认构造的 SendPack 表示一种无效的状态，不应该被使用。
   */
  SendPack() = default;

  /**
   * SendPack 只能被移动，不能被拷贝。
   */
  SendPack(const SendPack &) = delete;
  SendPack &operator=(const SendPack &) = delete;

  SendPack(SendPack &&other) noexcept;
  SendPack &operator=(SendPack &&other) noexcept;

  virtual ~SendPack() { delete uv_write_; }

  [[nodiscard]] auto uv_write() const { return uv_write_; }
  [[nodiscard]] auto &bufs() const { return bufs_; }
  [[nodiscard]] auto &on_finish() const { return on_finish_; }
  void set_on_finish(OnFinishCb on_finish) {
    on_finish_ = std::move(on_finish);
  }

  static void UvOnWrite(uv_write_t *req, int status);

 protected:
  uv_write_t *uv_write_ = nullptr;
  std::vector<uv_buf_t> bufs_;

 private:
  OnFinishCb on_finish_;
};

class SendRespPack : public SendPack {
 public:
  SendRespPack();

  explicit SendRespPack(std::shared_ptr<Response> resp);

  /**
   * SendRespPack 只能被移动，不能被拷贝。
   */
  SendRespPack(const SendRespPack &) = delete;
  SendRespPack &operator=(const SendRespPack &) = delete;

  SendRespPack(SendRespPack &&) noexcept = default;
  SendRespPack &operator=(SendRespPack &&) noexcept = default;

  ~SendRespPack() override;

  [[nodiscard]] auto resp() const { return resp_; }

 private:
  void SetUpBufs();
  void AddVersionBufs();
  void AddStatusCodeBufs();
  void AddStatusMsgBufs();
  void AddHeadersBufs();
  void AddBodyBufs();

  std::shared_ptr<Response> resp_;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_SEND_PACK_HPP_
