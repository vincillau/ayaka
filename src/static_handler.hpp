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

#ifndef AYAKA_SRC_STATIC_HANDLER_HPP_
#define AYAKA_SRC_STATIC_HANDLER_HPP_

#include <memory>
#include <string>

#include "http_handler.hpp"
#include "mime.hpp"

namespace ayaka {

class StaticPathHandler : public HttpHandler {
 public:
  StaticPathHandler() = default;

  /**
   * path 会被 lexically_normal 转换。
   */
  explicit StaticPathHandler(std::string path,
                             const std::shared_ptr<Mime>& mime);

  StaticPathHandler(const StaticPathHandler&) = default;
  StaticPathHandler& operator=(const StaticPathHandler&) = default;
  StaticPathHandler(StaticPathHandler&&) noexcept = default;
  StaticPathHandler& operator=(StaticPathHandler&&) noexcept = default;
  ~StaticPathHandler() override = default;

  void DoGet(const std::shared_ptr<Request>& req,
             std::shared_ptr<Response>& resp) override;

 private:
  std::filesystem::path path_;
  std::string mime_;
};

class StaticDirHandler : public HttpHandler {
 public:
  /**
   * 将 url 映射到本地目录 root。
   * url 和 root 会被 lexically_normal 转换。
   */
  StaticDirHandler(std::string url, std::string root,
                   std::shared_ptr<Mime> mime);

  void DoGet(const std::shared_ptr<Request>& req,
             std::shared_ptr<Response>& resp) override;

 private:
  std::filesystem::path root_;
  std::filesystem::path url_;
  std::shared_ptr<Mime> mime_;
};

}  // namespace ayaka

#endif  // AYAKA_SRC_STATIC_HANDLER_HPP_
