
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
        )

target_include_directories(scgicxx PUBLIC ${ASIO_PATH})


if (WITH_EXAMPLES)
    message (STATUS "Including examples")
    add_subdirectory(src/examples)
endif()



# Installation

#install(DIRECTORY src/include/ DESTINATION include/ga)