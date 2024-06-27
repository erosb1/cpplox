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
    Logger();
    explicit Logger(LogLevel level);
    Logger(const Logger& other) = delete;
    Logger(Logger&& other) noexcept = default;
    Logger& operator=(const Logger& other) = delete;
    Logger& operator=(Logger&& other) noexcept = default;
    ~Logger() = default;
    void Log(std::string msg);
    void SetOutputType(LogOutput output_type, std::string filename = "");
    void SetLogLevel(LogLevel log_level);
    std::string GetOutputString();
    [[nodiscard]] LogLevel GetLogLevel() const;
    template<typename T>
    Logger& operator<<(const T& value) {
        if (out_) {
            *out_ << value;
        }
        return *this;
    }
private:
    std::unique_ptr<std::ostream> out_;
    LogLevel level_;
};

#endif //LOGGER_H
