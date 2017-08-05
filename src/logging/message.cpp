#include "../include/logging/message.hpp"

#include <ctime>
#include <array>


namespace scgicxx
{
namespace logging
{

message::message(level_enum level, std::string &&source, std::string &&text) : level(level)
{
    time_point = std::chrono::system_clock::now();
    this->text = std::move(text);
    this->source = std::move(source);
}


const std::string &message::level_to_string() const
{
    static const std::string arr[4] = {
            "UNKNOWN",
            "ERROR",
            "INFO",
            "DEBUG"
    };

    return arr[level];
}


std::string message::time_point_to_utc_string(const char *format) const
{
    const std::time_t now = std::chrono::system_clock::to_time_t(time_point);
#ifdef _MSC_VER
    std::tm t;
    gmtime_s(&t, &now);
#else
    const std::tm t = &gmtime(&now);
#endif
    std::array<char, 24ul> str;
    std::size_t bytes_written = std::strftime(str.data(), str.size(), format, &t);
    return std::string(str.data(), bytes_written);
}

}
}