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

#include "case.hpp"

namespace ayaka {

void StrLower(std::string& str) {
  for (auto& ch : str) {
    ch = static_cast<char>(tolower(ch));
  }
}

bool StrEqualIgnoreCase(const std::string& str1, const std::string& str2) {
  if (str1.size() != str2.size()) {
    return false;
  }
  auto len = str1.size();
  for (auto i = 0; i < len; ++i) {
    if (tolower(str1[i]) != tolower(str2[i])) {
      return false;
    }
  }
  return true;
}

}  // namespace ayaka
