#include "SimpleUrlFetcher.h"
#include "UrlValidator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <string.h>
#include <assert.h>

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

// This really should return a file descriptor or something but just using for debugging so is fine for now
std::string SimpleUrlFetcher::mkTmp() const {
    char tempFile[] = "/tmp/fakeRestData-XXXXXX";
    int fd = mkstemp(tempFile);
    if (fd == -1) {
        return "";
    }
    close(fd);
    return std::string(tempFile);
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
        tempFilePath = mkTmp();
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
    if(writeToFile){
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeToFileCallback);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, outFile);
    }
    else {
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, static_cast<size_t (*)(void*, size_t, size_t, void*)>(writeCallback));
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &responseData);
    }

    CURLcode res = curl_easy_perform(m_curl);

    if (outFile) {
        fclose(outFile);
    }

    if (res != CURLE_OK) {
        m_logger->logError("CURL request failed: " + std::string(curl_easy_strerror(res)));
        return "";
    }
    else {
        m_logger->logInfo("Successfully fetched data from: " + url);
    }

    if(writeToFile) {
        m_logger->logInfo("Response written to: " + tempFilePath);
        return tempFilePath;
    }
    else {
        return responseData;
    }
}


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
