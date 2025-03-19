#include "SimpleUrlFetcher.h"
#include "UrlValidator.h"

SimpleUrlFetcher::SimpleUrlFetcher() {
    curl = curl_easy_init();
}

SimpleUrlFetcher::~SimpleUrlFetcher() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

std::string SimpleUrlFetcher::fetchData(const std::string& url) {
    Logger& logger = Logger::getInstance();

    // Validate URL
    if (!UrlValidator::isValid(url)) {
        logger.logError("Invalid URL format: " + url);
        return "";
    }

    if (!curl) {
        logger.logError("CURL initialization failed.");
        return "";
    }

    std::string responseData;

    // Lambda seemed clever here but maybe revisit
    auto writeCallback = [](void* contents, size_t size, size_t nmemb, void* userData) -> size_t {
        size_t totalSize = size * nmemb;
        static_cast<std::string*>(userData)->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    };

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, static_cast<size_t (*)(void*, size_t, size_t, void*)>(writeCallback));
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        logger.logError("CURL request failed: " + std::string(curl_easy_strerror(res)));
        return "";
    }

    logger.logInfo("Successfully fetched data from: " + url);
    return responseData;
}
