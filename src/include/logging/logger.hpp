#pragma once

#include "abstract_logger.hpp"
#include "message.hpp"

#include <vector>
#include <queue>
#include <mutex>
#include <atomic>
#include <memory>


namespace scgicxx
{
namespace logging
{

class logger
{
public:
    using loggers_list_type = std::vector<std::unique_ptr<abstract_logger>>;

public:
    logger();
    logger(loggers_list_type &&loggers);
    void log_info(std::string &&source, std::string &&text);
    void log_error(std::string &&source, std::string &&text);
    void log_debug(std::string &&source, std::string &&text);
    ~logger();


private:
    void process_queue();
    void run();
    void stop();
    void log(level_enum level, std::string &&source, std::string &&text);

private:
    loggers_list_type concrete_loggers;
    std::queue<message> log_messages_queue;
    std::mutex mutex;
    std::atomic_bool is_stopped;
};

}
}