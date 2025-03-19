#include <iostream>
#include "SimpleUrlFetcher.h"
#include <json/json.h>
#include "Logger.h"

int main(int argc, char* argv[]) {
    Logger& logger = Logger::getInstance();
    std::string url = "http://test.brightsign.io:3000";

    // TODO - Make this mandatory later and err out
    if (argc > 1) {
        url = argv[1];
    }

    logger.logInfo("Fetching data from: " + url);

    SimpleUrlFetcher fetcher;
    std::string response = fetcher.fetchData(url);

    if (response.empty()) {
        logger.logError("Failed to fetch data from URL.");
        return 1;
    }

    Json::CharReaderBuilder readerBuilder;
    Json::Value jsonData;
    std::string errors;
    std::istringstream jsonStream(response);

    if (Json::parseFromStream(readerBuilder, jsonStream, &jsonData, &errors)) {
        // Too long to bother print
        // logger.logInfo("Received JSON:\n" + jsonData.toStyledString());
        logger.logInfo("Received Valid JSON with size : " + std::to_string(response.length()));
    } else {
        logger.logError("Failed to parse JSON: " + errors);
    }

    return 0;
}
