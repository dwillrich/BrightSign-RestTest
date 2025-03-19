#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <mutex>

class Logger {
  public:
    static Logger& getInstance();

    void logInfo(const std::string& message);
    void logError(const std::string& message);

  private:
    Logger() = default;
    ~Logger() = default;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static std::mutex logMutex;
};

#endif // LOGGER_H
