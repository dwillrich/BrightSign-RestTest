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

    std::vector<User> users;
    bool jsonParsed = buildUserVectorFromJsonString(response, users);
    logger.logInfo("Got the following user count: " + std::to_string(users.size()));
    if(!jsonParsed) {
        logger.logError("JSON Parsing Fail -> User Count: " + std::to_string(users.size()));
        return 1;
    } else {
        auto q1 = averageAgeOfUsersPerCity(users);
        std::cout << "---------Q1 : averageAgeOfUsersPerCity---------" << std::endl;
        for (std::map<std::string, double>::iterator it = q1.begin(); it != q1.end(); ++it) {
            std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
        }

        auto q2 = averageFriendsPerCity(users);
        std::cout << "---------Q2 : averageFriendsPerCity---------" << std::endl;
        for (std::map<std::string, double>::iterator it = q2.begin(); it != q2.end(); ++it) {
            std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
        }

        auto q3 = mostPopularUserPerCity(users);
        std::cout << "---------Q3 : mostPopularUserPerCity---------" << std::endl;
        for (std::map<std::string, User>::iterator it = q3.begin(); it != q3.end(); ++it) {
            std::cout << "Key: " << it->first << ", Value: " << *it->second.name << std::endl;
        }

        auto q4 = mostPopularName(users);
        std::cout << "---------Q4 : mostPopularName---------" << std::endl;
        std::cout << "- : " << q4 << std::endl;

        auto q5 = mostPopularHobby(users);
        std::cout << "---------Q5 : mostPopularHobby---------" << std::endl;
        std::cout << "- : " << q5 << std::endl;

    }

    return 0;
}
