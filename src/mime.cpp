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

#include "mime.hpp"

#include <fstream>

#include "logger.hpp"

namespace ayaka {

const std::string& Mime::GetMime(const std::string& ext) const {
  if (ext.empty()) {
    return default_mime_;
  }
  try {
    return ext_map_.at(ext);
  } catch (const std::out_of_range& e) {
    return default_mime_;
  }
}

void Mime::Load(const std::string& path) {
  std::ifstream mime_file(path);
  if (!mime_file.is_open()) {
    AYAKA_LOG_CRITICAL("failed to open mime type file: {}", path);
  }

  std::unordered_map<std::string, std::string> ext_map;
  for (;;) {
    std::string mime;
    mime_file >> mime;
    if (mime_file.peek() == EOF) {
      break;
    }
    if (mime.empty()) {
      continue;
    }

    for (;;) {
      std::string ext;
      mime_file >> ext;
      if (mime_file.peek() == EOF || ext.empty()) {
        AYAKA_LOG_CRITICAL("failed to read mime type file: {}", path);
      }
      if (ext.back() == ';') {
        ext.pop_back();
        ext_map[std::move(ext)] = mime;
        break;
      }
      ext_map[std::move(ext)] = mime;
    }
  }

  ext_map_ = std::move(ext_map);
  mime_file.close();
}

}  // namespace ayaka
