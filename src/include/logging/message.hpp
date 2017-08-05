#pragma once

#include "level_enum.hpp"
#include <string>
#include <chrono>


namespace scgicxx
{
namespace logging
{

struct message
{
    std::chrono::system_clock::time_point time_point;
    level_enum level;
    std::string source;
    std::string text;

    message() = default;
    message(level_enum level, std::string &&source, std::string &&text);

    const std::string &level_to_string() const;
    std::string time_point_to_utc_string(const char *format = "%F %T UTC") const;
};

}
}