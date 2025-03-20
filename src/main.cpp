#include <iostream>
#include "SimpleUrlFetcher.h"
#include "Logger.h"
#include "UsersAndFriends.h"
#include <fstream>
#include "MiscUtils.h"

int main(int argc, char* argv[]) {
    Logger& logger = Logger::getInstance();
    std::string url = "http://test.brightsign.io:3000";

    // TODO - Make this mandatory later and err out
    if (argc > 1) {
        url = argv[1];
    }

    logger.logInfo("Fetching data from: " + url);

    SimpleUrlFetcher fetcher;

    // This doesn't work right now so comment out and loop through data line by line instead
    // All this and sometimes it isn't sent one item a line!!!!
    // auto lineCb = [](const std::string& line) {
    //     // std::cout << line << std::endl;
    // };
    // fetcher.fetchDataByLine(url, lineCb);
    // return 0;

    std::string response = fetcher.fetchData(url);

    if (response.empty()) {
        logger.logError("Failed to fetch data from URL.");
        return 1;
    }

    // For debugging save the REST data to file
    std::string restDataFile = MiscUtils::writeStringToTmpFile(response);
    if(restDataFile != "" ) {
        logger.logInfo("Wrote REST data to : " + restDataFile);
    } else {
        logger.logError("Failed to write REST data to file");
        return 1;
    }



    return 0;
}
