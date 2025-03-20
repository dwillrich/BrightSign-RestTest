#include "SimpleUrlFetcher.h"
#include "UrlValidator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "MiscUtils.h"

SimpleUrlFetcher::SimpleUrlFetcher() {
    m_curl = curl_easy_init();
    // Would rather this not be a pointer but for simplicity it is
    m_logger = &Logger::getInstance();
    assert(m_curl && m_logger);
}

SimpleUrlFetcher::~SimpleUrlFetcher() {
    if (m_curl) {
        curl_easy_cleanup(m_curl);
    }
}

std::string SimpleUrlFetcher::fetchData(const std::string& url, bool writeToFile) {
    // Validate URL
    if (!UrlValidator::isValid(url)) {
        m_logger->logError("Invalid URL format: " + url);
        return "";
    }

    if (!m_curl) {
        m_logger->logError("CURL initialization failed.");
        return "";
    }

    std::string responseData;
    std::string tempFilePath;
    FILE* outFile = nullptr;

    if (writeToFile) {
        tempFilePath = MiscUtils::mkTmp();
        if (tempFilePath.empty()) {
            m_logger->logError("Failed to create a temporary file.");
            return "";
        }

        outFile = fopen(tempFilePath.c_str(), "wb");
        if (!outFile) {
            m_logger->logError("Failed to open temporary file: " + tempFilePath);
            return "";
        }
    }

    // Lambda seemed clever here but maybe revisit
    auto writeCallback = [](void* contents, size_t size, size_t nmemb, void* userData) -> size_t {
        size_t totalSize = size * nmemb;
        static_cast<std::string*>(userData)->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    };

    auto writeToFileCallback = [](void* contents, size_t size, size_t nmemb, void* userData) -> size_t {
        FILE* fileStream = static_cast<FILE*>(userData);
        if (!fileStream) {
            return 0;
        }
        return fwrite(contents, size, nmemb, fileStream);
    };

    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_curl, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 5L); // 5 Second timeout is arbitrary but good enough

    if(writeToFile) {
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeToFileCallback);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, outFile);
    } else {
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, static_cast<size_t (*)(void*, size_t, size_t, void*)>(writeCallback));
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &responseData);
    }

    long http_code = 0;
    CURLcode res = curl_easy_perform(m_curl);
    curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (outFile) {
        fclose(outFile);
    }

    // This is a HACK but seems the API is returning 200 but sending a html page with the error ie. 400 Bad Request
    std::istringstream f(responseData);
    std::string firstLine;
    std::getline(f, firstLine);
    if(firstLine.find("400 Bad Request") != std::string::npos) {
        m_logger->logError("CURL override http_code : 400");
        http_code = 400;
    }
    // End hack zone

    if (res != CURLE_OK) {
        m_logger->logError("CURL request failed: " + std::string(curl_easy_strerror(res)));
        return "";
    } else if(http_code == 200) {
        m_logger->logInfo("Successfully fetched data from: " + url + " HTTP Code : " + std::to_string(http_code));
    } else {
        m_logger->logError("CURL got HTTP code : " + std::to_string(http_code));
        return "";
    }

    if(writeToFile) {
        m_logger->logInfo("Response written to: " + tempFilePath);
        return tempFilePath;
    } else {
        return responseData;
    }
}

// Not well tested probably wouldn't use
bool SimpleUrlFetcher::fetchDataByLine(const std::string& url, std::function<void(const std::string&)> lineCallback) {
    if (!m_curl) {
        m_logger->logError("CURL initialization failed.");
        return false;
    }

    auto lineCallbackLambda = [](void *contents, size_t size, size_t nmemb, void *userp) -> size_t {
        size_t total_size = size * nmemb;
        char *data = static_cast<char *>(contents);
        auto *callback = static_cast<std::function<void(const std::string &)> *>(userp);

        char *line = strtok(data, "\n");
        while (line != nullptr) {
            (*callback)(line);
            line = strtok(nullptr, "\n");
        }

        return total_size;
    };

    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, lineCallbackLambda);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &lineCallback);

    CURLcode res = curl_easy_perform(m_curl);

    if (res != CURLE_OK) {
        m_logger->logError("CURL request failed: " + std::string(curl_easy_strerror(res)));
        return false;
    }

    m_logger->logInfo("Successfully streamed data from: " + url);
    return true;
}
