# Copyright (C) 2022 Vincil Lau.
#
# Ayaka is free software: you can redistribute it and/or modify it under the
# terms of the GNU Affero General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# Ayaka is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
# details.
#
# You should have received a copy of the GNU Affero General Public License along
# with Ayaka. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.20)

# 项目配置

project(ayaka VERSION 0.0.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Git 子模块

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/googletest)
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/json)
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/libuv)
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/spdlog)

# 构建 Ayaka 静态库

set(LOG_LEVEL LOG_LEVEL_TRACE)

configure_file(${PROJECT_SOURCE_DIR}/res/html/400.html.in
               ${PROJECT_SOURCE_DIR}/res/html/400.html)
configure_file(${PROJECT_SOURCE_DIR}/res/html/404.html.in
               ${PROJECT_SOURCE_DIR}/res/html/404.html)
configure_file(${PROJECT_SOURCE_DIR}/res/html/405.html.in
               ${PROJECT_SOURCE_DIR}/res/html/405.html)
configure_file(${PROJECT_SOURCE_DIR}/res/html/index.html.in
               ${PROJECT_SOURCE_DIR}/res/html/index.html)
configure_file(${PROJECT_SOURCE_DIR}/res/ayaka.json.in
               ${PROJECT_SOURCE_DIR}/res/ayaka.json)
configure_file(${PROJECT_SOURCE_DIR}/src/version.hpp.in
               ${PROJECT_SOURCE_DIR}/src/version.hpp)

file(STRINGS ${PROJECT_SOURCE_DIR}/res/html/400.html HTML_400 NEWLINE_CONSUME)
file(STRINGS ${PROJECT_SOURCE_DIR}/res/html/404.html HTML_404 NEWLINE_CONSUME)
file(STRINGS ${PROJECT_SOURCE_DIR}/res/html/405.html HTML_405 NEWLINE_CONSUME)

configure_file(${PROJECT_SOURCE_DIR}/src/http_except.cpp.in
               ${PROJECT_SOURCE_DIR}/src/http_except.cpp)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/third_party/googletest/googletest/include
  ${PROJECT_SOURCE_DIR}/third_party/googletest/googlemock/include
  ${PROJECT_SOURCE_DIR}/third_party/json/include
  ${PROJECT_SOURCE_DIR}/third_party/libuv/include
  ${PROJECT_SOURCE_DIR}/third_party/spdlog/include
  ${PROJECT_SOURCE_DIR}/third_party/spdlog/include/spdlog)

add_compile_definitions(LOG_LEVEL=${LOG_LEVEL})

set(AYAKA_LIB_SOURCES
    src/case.hpp
    src/case.cpp
    src/conf.hpp
    src/conf.cpp
    src/downstream.hpp
    src/downstream.cpp
    src/error.hpp
    src/http_except.hpp
    src/http_except.cpp
    src/http_handler.hpp
    src/http_handler.cpp
    src/http_method.hpp
    src/http_parser.hpp
    src/http_parser.cpp
    src/http_status.hpp
    src/inet_addr.hpp
    src/inet_addr.cpp
    src/location.hpp
    src/location.cpp
    src/logger.hpp
    src/loop.hpp
    src/loop.cpp
    src/mime.hpp
    src/mime.cpp
    src/request.hpp
    src/response.hpp
    src/response.cpp
    src/router.hpp
    src/router.cpp
    src/send_pack.hpp
    src/send_pack.cpp
    src/server.hpp
    src/server.cpp
    src/static_handler.hpp
    src/static_handler.cpp
    src/tcp.hpp
    src/tcp.cpp
    src/url.hpp
    src/url.cpp
    src/version.hpp
    src/worker.hpp
    src/worker.cpp)

add_library(ayaka_lib STATIC ${AYAKA_LIB_SOURCES})

# 构建 Ayaka 可执行文件

add_executable(ayaka src/main.cpp)
target_link_libraries(ayaka ayaka_lib pthread spdlog uv)

# 单元测试

enable_testing()

function(ayaka_test TEST_FILE)
  cmake_path(GET TEST_FILE STEM TEST_TARGET_NAME)
  add_executable(${TEST_TARGET_NAME} ${TEST_FILE}
                                     ${PROJECT_SOURCE_DIR}/test/test.hpp)
  target_link_libraries(${TEST_TARGET_NAME} ayaka_lib gtest pthread spdlog uv)
  add_test(NAME ${TEST_TARGET_NAME} COMMAND ${TEST_TARGET_NAME}
                                            --gtest_color=yes)
endfunction(ayaka_test)

ayaka_test(test/case_test.cpp)
ayaka_test(test/conf_test.cpp)
ayaka_test(test/inet_addr_test.cpp)
ayaka_test(test/http_except_test.cpp)
ayaka_test(test/http_parser_test.cpp)
ayaka_test(test/http_status_test.cpp)
ayaka_test(test/location_test.cpp)
ayaka_test(test/mime_test.cpp)
ayaka_test(test/response_test.cpp)
ayaka_test(test/send_pack_test.cpp)
ayaka_test(test/static_handler_test.cpp)
ayaka_test(test/tcp_test.cpp)
ayaka_test(test/url_test.cpp)
