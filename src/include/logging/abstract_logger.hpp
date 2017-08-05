#pragma once

#include "message.hpp"


namespace scgicxx
{
namespace logging
{

class abstract_logger
{
public:
    abstract_logger() : level(info)
    {
    }

    explicit abstract_logger(level_enum level);

    level_enum get_level() const;
    void set_level(level_enum level);
    virtual void log(const message &) = 0;

protected:
    level_enum level;
};

}
}