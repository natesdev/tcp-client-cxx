#pragma once
#include <string>
#include <source_location>

class Logger
{
private:
    std::string className;

public:
    explicit Logger(const std::string &className);

    void info(const std::string &message,
              const std::source_location &loc = std::source_location::current());

    void error(const std::string &message,
               const std::source_location &loc = std::source_location::current());

    void warn(const std::string &message,
              const std::source_location &loc = std::source_location::current());

    void debug(const std::string &message,
               const std::source_location &loc = std::source_location::current());
};
