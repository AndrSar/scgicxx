#include "../include/logging/logger.hpp"
#include "../include/logging/console_logger.hpp"

#include <array>
#include <thread>


namespace scgicxx
{
namespace logging
{

logger::logger(): is_stopped(true)
{
#if defined(_DEBUG) || defined(DEBUG)
    auto level = debug;
#else
    auto level = info;
#endif

    concrete_loggers.push_back(std::make_unique<console_logger>(level));
    run();
}


logger::logger(loggers_list_type &&loggers):
        is_stopped(true),
        concrete_loggers(std::move(loggers))
{
    run();
}


void logger::log(level_enum level, std::string &&source, std::string &&text)
{
    std::lock_guard<std::mutex> lock(mutex);
    log_messages_queue.emplace(level, std::move(source), std::move(text));
}


void logger::log_info(std::string &&source, std::string &&text)
{
    log(info, std::move(source), std::move(text));
}


void logger::log_error(std::string &&source, std::string &&text)
{
    log(error, std::move(source), std::move(text));
}


void logger::log_debug(std::string &&source, std::string &&text)
{
#if defined(_DEBUG) || defined(DEBUG)
    log(debug, std::move(source), std::move(text));
#endif
}


void logger::process_queue()
{
    std::array<message, 32ul> buffer;
    std::size_t n = 0;

    mutex.lock();

    while (n < buffer.size() && !log_messages_queue.empty())
    {
        buffer[n] = log_messages_queue.front();
        log_messages_queue.pop();
        ++n;
    }

    mutex.unlock();

    for (std::size_t i = 0; i < n; ++i)
    {
        const auto &log_message = buffer[i];
        for (auto &logger_ptr : concrete_loggers)
        {
            if (logger_ptr->get_level() >= log_message.level)
            {
                logger_ptr->log(log_message);
            }
        }
    }
}


void logger::run()
{
    is_stopped = false;
    std::thread thread([this](){
        while (!is_stopped)
        {
            process_queue();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });

    thread.detach();
}


void logger::stop()
{
    is_stopped.store(true);
}


logger::~logger()
{
    this->stop();
}

}
}