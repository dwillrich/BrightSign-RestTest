#ifndef SIMPLE_URL_FETCHER_H
#define SIMPLE_URL_FETCHER_H

#include <string>
#include <curl/curl.h>
#include <functional>
#include "Logger.h"

class SimpleUrlFetcher {
  public:
    SimpleUrlFetcher();
    ~SimpleUrlFetcher();
    std::string fetchData(const std::string& url, bool writeToFile = false);
    bool fetchDataByLine(const std::string& url, std::function<void(const std::string&)> lineCallback);
  private:
    std::string mkTmp() const;

    Logger* m_logger;
    CURL* m_curl;
};

#endif // SIMPLE_URL_FETCHER_H
