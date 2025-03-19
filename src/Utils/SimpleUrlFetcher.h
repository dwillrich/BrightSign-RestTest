#ifndef SIMPLE_URL_FETCHER_H
#define SIMPLE_URL_FETCHER_H

#include <string>
#include <curl/curl.h>
#include "Logger.h"

class SimpleUrlFetcher {
  public:
    SimpleUrlFetcher();
    ~SimpleUrlFetcher();
    std::string fetchData(const std::string& url);

  private:
    CURL* curl;
};

#endif // SIMPLE_URL_FETCHER_H
