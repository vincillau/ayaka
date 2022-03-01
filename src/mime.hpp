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

#ifndef AYAKA_SRC_MIME_HPP_
#define AYAKA_SRC_MIME_HPP_

#include <string>
#include <unordered_map>

namespace ayaka {

class Mime {
 public:
  Mime() = default;
  explicit Mime(std::string default_mime)
      : default_mime_(std::move(default_mime)){};

  Mime(const Mime&) = default;
  Mime& operator=(const Mime&) = default;
  Mime(Mime&&) = default;
  Mime& operator=(Mime&&) = default;
  ~Mime() = default;

  [[nodiscard]] auto& default_mime() const { return default_mime_; }
  void set_default_mime(std::string default_mime) {
    default_mime_ = std::move(default_mime);
  }

  [[nodiscard]] auto& ext_map() const { return ext_map_; }
  [[nodiscard]] auto& ext_map() { return ext_map_; }

  [[nodiscard]] const std::string& GetMime(const std::string& ext) const;

  /**
   * 从路径为 path 的文件加载 MIME 类型。path 必须是存在的。
   */
  void Load(const std::string& path);

 private:
  std::unordered_map<std::string, std::string> ext_map_;
  std::string default_mime_ = "application/octet-stream";
};

}  // namespace ayaka

#endif  // AYAKA_SRC_MIME_HPP_
