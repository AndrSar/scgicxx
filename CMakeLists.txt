
#          Copyright Andrey Lifanov 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.2)

cmake_policy(SET CMP0048 NEW)

project(scgicxx VERSION 0.1.0 LANGUAGES CXX)

message (STATUS "C++ compiler is ${CMAKE_CXX_COMPILER}")

find_path(ASIO_PATH NAMES asio.hpp)
message (STATUS "ASIO C++ library headers path: ${ASIO_PATH}")


if (NOT MSVC)
    add_compile_options(-std=c++14)
endif()


add_library(scgicxx
        src/include/server.hpp
        src/server.cpp
        src/include/detail/detail.hpp
        src/include/detail/array_view.hpp
        src/include/detail/string_view.hpp
        src/include/scgi_protocol_parser.hpp
        src/scgi_protocol_parser.cpp
        src/detail/detail.cpp
        src/include/http_request.hpp
        src/http_request.cpp
        src/include/http_response.hpp
        src/http_response.cpp
        src/include/server_pool.hpp
        src/server_pool.cpp
        src/include/logging/logger.hpp
        src/logging/logger.cpp
        src/include/logging/abstract_logger.hpp
        src/logging/abstract_logger.cpp
        src/include/logging/console_logger.hpp
        src/logging/console_logger.cpp
        src/include/logging/message.hpp
        src/logging/message.cpp
        src/include/logging/level_enum.hpp
        )

target_include_directories(scgicxx PUBLIC ${ASIO_PATH})

if (MSVC)
    target_compile_definitions(scgicxx PUBLIC -D_WIN32_WINNT=0x0601)
endif()


if (WITH_EXAMPLES)
    message (STATUS "Including examples")
    add_subdirectory(src/examples)
endif()


if (WITH_TESTS)
    enable_testing()
    add_subdirectory(src/test)
endif()

# Installation

#install(DIRECTORY src/include/ DESTINATION include/ga)