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

#include "response.hpp"

#include "version.hpp"

namespace ayaka {

std::string Response::GetDate(const time_t* tloc) {
  time_t t = 0;
  if (tloc == nullptr) {
    t = time(&t);
  } else {
    t = *tloc;
  }
  tm tm{};
  gmtime_r(&t, &tm);
  char buf[32];
  strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tm);
  return {buf};
}

std::shared_ptr<Response> Response::Default() {
  auto resp = std::make_shared<Response>();
  resp->version_ = "HTTP/1.1";
  resp->status_ = std::move(HttpStatus::Ok());
  resp->headers_["Connection"] = "close";
  resp->headers_["Date"] = GetDate();
  resp->headers_["Server"] = "Ayaka/" AYAKA_VERSION;
  return resp;
}

}  // namespace ayaka
