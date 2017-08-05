#pragma once

#include "abstract_logger.hpp"


namespace scgicxx
{
namespace logging
{

class console_logger : public abstract_logger
{
public:
    console_logger() = default;
    explicit console_logger(level_enum level);

    void log(const message &) override;
};

}
}