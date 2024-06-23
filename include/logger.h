#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum class LogOutput {
    STDOUT,
    FILE,
    STRING,
};

enum class LogLevel {
    DEBUG,
    ERROR,
};

class Logger {
public:
    Logger(LogLevel level);
    void Log(std::string msg);
    void SetOutputType(LogOutput output_type, std::string filename = "");
    std::string GetOutputString();
    [[nodiscard]] LogLevel GetLogLevel() const;
private:
    std::unique_ptr<std::ostream> out_;
    LogLevel level_;
};

#endif //LOGGER_H
