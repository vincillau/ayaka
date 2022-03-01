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

#include "static_handler.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <filesystem>
#include <string>
#include <system_error>

namespace ayaka {

StaticPathHandler::StaticPathHandler(std::string path,
                                     const std::shared_ptr<Mime>& mime) {
  auto p = std::filesystem::path(std::move(path));
  path_ = std::move(p.lexically_normal());
  std::string ext = path_.extension().string();
  if (ext.empty()) {
    mime_ = mime->GetMime("");
  } else {
    mime_ = mime->GetMime(ext.substr(1));
  }
}

StaticDirHandler::StaticDirHandler(std::string url, std::string root,
                                   std::shared_ptr<Mime> mime)
    : mime_(std::move(mime)) {
  auto url_path = std::filesystem::path(std::move(url));
  url_ = std::move(url_path.lexically_normal());
  auto root_path = std::filesystem::path(std::move(root));
  root_ = std::move(root_path.lexically_normal());
}

void StaticPathHandler::DoGet(const std::shared_ptr<Request>& req,
                              std::shared_ptr<Response>& resp) {
  resp->headers()["Content-Type"] = mime_;
  try {
    auto file_size = std::filesystem::file_size(path_);
    resp->body() = std::make_shared<std::vector<char>>(file_size);
    auto fd = open(path_.c_str(), O_RDONLY);
    if (fd == -1) {
      throw Http404Except(req->method(), req->url().src());
    }
    auto read_size = read(fd, resp->body()->data(), file_size);
    if (read_size == -1) {
      throw Http404Except(req->method(), req->url().src());
    }
  } catch (const std::filesystem::filesystem_error& except) {
    throw Http404Except(req->method(), req->url().src());
  }
}

void StaticDirHandler::DoGet(const std::shared_ptr<Request>& req,
                             std::shared_ptr<Response>& resp) {
  try {
    auto relative =
        std::filesystem::path(req->url().path()).lexically_relative(url_);
    auto path = (root_ / relative).lexically_normal();
    if (std::filesystem::is_directory(path)) {
      path /= "index.html";
    }

    std::string ext = path.extension().string();
    if (ext.empty()) {
      resp->headers()["Content-Type"] = mime_->GetMime("");
    } else {
      resp->headers()["Content-Type"] = mime_->GetMime(ext.substr(1));
    }

    auto file_size = std::filesystem::file_size(path);
    resp->body() = std::make_shared<std::vector<char>>(file_size);
    auto fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
      throw Http404Except(req->method(), req->url().src());
    }
    auto read_size = read(fd, resp->body()->data(), file_size);
    if (read_size == -1) {
      throw Http404Except(req->method(), req->url().src());
    }
  } catch (const std::filesystem::filesystem_error& except) {
    throw Http404Except(req->method(), req->url().src());
  }
}

}  // namespace ayaka
