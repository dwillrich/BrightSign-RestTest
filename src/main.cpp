#include <iostream>
#include "SimpleUrlFetcher.h"
#include "Logger.h"
#include "UsersAndFriends.h"
#include <fstream>
#include "MiscUtils.h"
#include "UserAnalytics.h"

using namespace UsersAndFriends;

int main(int argc, char* argv[]) {
    Logger& logger = Logger::getInstance();
    std::string url = "http://test.brightsign.io:3000";

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

    // For debugging save the REST data to file
    std::string restDataFile = MiscUtils::writeStringToTmpFile(response);
    if(restDataFile != "" ) {
        logger.logInfo("Wrote REST data to : " + restDataFile);
    } else {
        logger.logError("Failed to write REST data to file");
        return 1;
    }

    std::vector<User> users;
    bool jsonParsed = buildUserVectorFromJsonString(response, users);
    logger.logInfo("Got the following user count: " + std::to_string(users.size()));
    if(!jsonParsed) {
        logger.logError("JSON Parsing Fail -> User Count: " + std::to_string(users.size()));
        return 1;
    } else {
        auto c1 = averageAgeOfUsersPerCity(users);
        auto c2 = averageFriendsPerCity(users);
        auto c3 = mostPopularUserPerCity(users);
        auto c4 = mostPopularName(users);
        auto c5 = mostPopularHobby(users);

        json outputJson;
        outputJson["C1"] = c1;
        outputJson["C2"] = c2;
        outputJson["C3"] = c3;
        outputJson["C4"] = c4;
        outputJson["C5"] = c5;

        // No pretty print here but jq can help 
        std::cout << outputJson.dump() << std::endl;

    }

    return 0;
}
