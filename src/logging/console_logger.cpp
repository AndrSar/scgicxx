#include "../include/logging/console_logger.hpp"

#include <iostream>
#include <sstream>


namespace scgicxx
{
namespace logging
{

console_logger::console_logger(level_enum level) : abstract_logger(level)
{
}


void console_logger::log(const message &msg)
{
    std::ostringstream ostream;
    ostream  << '[' << msg.level_to_string() << "] " << msg.time_point_to_utc_string() << ' '
             << msg.text << std::endl;

    if (level == error)
    {
        std::cerr << ostream.str();
    }
    else
    {
        std::cout << ostream.str();
    }
}

}
}