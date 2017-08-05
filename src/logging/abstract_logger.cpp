#include "../include/logging/abstract_logger.hpp"


namespace scgicxx
{
namespace logging
{

abstract_logger::abstract_logger(level_enum level) : level(level)
{
}


level_enum abstract_logger::get_level() const
{
    return level;
}


void abstract_logger::set_level(level_enum level)
{
    this->level = level;
}

}
}