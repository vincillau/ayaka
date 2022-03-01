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

#include "http_handler.hpp"

namespace ayaka {

void HttpHandler::Handle(const std::shared_ptr<Request> &req,
                         std::shared_ptr<Response> &resp) {
  if (req->method() == http_method::kGet) {
    DoGet(req, resp);
  } else if (req->method() == http_method::kPost) {
    DoPost(req, resp);
  } else if (req->method() == http_method::kPut) {
    DoPut(req, resp);
  } else if (req->method() == http_method::kDelete) {
    DoDelete(req, resp);
  } else if (req->method() == http_method::kHead) {
    DoHead(req, resp);
  } else if (req->method() == http_method::kOptions) {
    DoOptions(req, resp);
  } else if (req->method() == http_method::kConnect) {
    DoConnect(req, resp);
  } else if (req->method() == http_method::kTrace) {
    DoTrace(req, resp);
  } else if (req->method() == http_method::kPatch) {
    DoPatch(req, resp);
  } else {
    throw Http405Except(req->method(), req->url().src());
  }
}

}  // namespace ayaka
