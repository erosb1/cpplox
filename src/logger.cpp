#include <iostream>
#include <fstream>
#include <sstream>

#include "logger.h"

Logger::Logger()
    : level_(LogLevel::DEBUG)
{
    SetOutputType(LogOutput::STDOUT);
}

Logger::Logger(LogLevel level)
    : level_(level)
{
    SetOutputType(LogOutput::STDOUT);
}

void Logger::Log(std::string msg) {
    *(out_) << msg << std::endl;
}

void Logger::SetOutputType(LogOutput output_type, std::string filename) {
    switch (output_type) {
        case LogOutput::STDOUT:
            if (level_ == LogLevel::ERROR) {
                out_ = std::make_unique<std::ostream>(std::cerr.rdbuf());
            } else {
                out_ = std::make_unique<std::ostream>(std::cout.rdbuf());
            }
            break;
        case LogOutput::FILE:
            if (filename.empty()) {
                throw new std::invalid_argument("You have to provide a file name");
            }
            out_ = std::make_unique<std::ofstream>(filename);
            break;
        case LogOutput::STRING:
            out_ = std::make_unique<std::ostringstream>();
            break;
    }
}

void Logger::SetLogLevel(LogLevel log_level) {
    level_ = log_level;
}

std::string Logger::GetOutputString() {
    if (auto oss = dynamic_cast<std::ostringstream*>(out_.get())) {
        return oss->str();
    }
    throw new std::invalid_argument("Cannot get string when LogOutput != STRING");
}

LogLevel Logger::GetLogLevel() const {
    return level_;
}
