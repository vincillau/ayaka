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

#ifndef AYAKA_SRC_HTTP_METHOD_HPP_
#define AYAKA_SRC_HTTP_METHOD_HPP_

#include <string>

namespace ayaka::http_method {

static const std::string kGet = "GET";
static const std::string kPost = "POST";
static const std::string kPut = "PUT";
static const std::string kDelete = "DELETE";
static const std::string kHead = "HEAD";
static const std::string kOptions = "OPTIONS";
static const std::string kConnect = "CONNECT";
static const std::string kTrace = "TRACE";
static const std::string kPatch = "PATCH";
static const std::string kUnknown = "UNKNOWN";

}  // namespace ayaka::http_method

#endif  // AYAKA_SRC_HTTP_METHOD_HPP_
