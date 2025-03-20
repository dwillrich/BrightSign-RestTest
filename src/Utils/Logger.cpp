#include "Logger.h"
#include <syslog.h>

std::mutex Logger::logMutex;

#define LOG_TO_COUT   0
#define LOG_TO_CERR   0
#define LOG_TO_SYSLOG 1

#if LOG_TO_SYSLOG
bool sysLogOpen = false;
#endif

Logger::Logger() {
#if LOG_TO_SYSLOG
    if(!sysLogOpen) {
        sysLogOpen = true;
        openlog("fakerest-test", LOG_PID | LOG_CONS, LOG_USER);
    }
#endif
}

Logger::~Logger() {
#if LOG_TO_SYSLOG
    if(!sysLogOpen) {
        closelog();
    }
#endif
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::logInfo(const std::string& message) {
#if LOG_TO_COUT
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << "[INFO] " << message << std::endl;
#elif LOG_TO_CERR
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << "[INFO] " << message << std::endl;
#elif LOG_TO_SYSLOG
    syslog(LOG_INFO, "%s", message.c_str());
#else
    (void)message;
#endif
}

void Logger::logError(const std::string& message) {
#if LOG_TO_COUT
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << "[INFO] " << message << std::endl;
#elif LOG_TO_CERR
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << "[INFO] " << message << std::endl;
#elif LOG_TO_SYSLOG
    syslog(LOG_ERR, "%s", message.c_str());
#else
    (void)message;
#endif
}
