#include <gtest/gtest.h>
#include "UsersAndFriends.h"
#include <filesystem>
#include "SimpleUrlFetcher.h"
#include "MiscUtils.h"

// This is useful for retesting. The API test below will write all failed data to tmp
//  We can then use this test to reprocess the response sample to ensure error is fixed
TEST(UsersAndFriendsTest, ProcessAllSamplesInDir) {
    Logger& logger = Logger::getInstance();

    // This is not ideal but is easiest to do for debugging
    using namespace UsersAndFriends;
    // should be run from top level
    std::string rootDir = ".";
    std::string toolsDir = rootDir + "/tools";
    std::string samplesDir = rootDir + "/samples";

    // Ideally this doesn't need to happen amd they are already there
    // system((toolsDir + "/getSampleResponses.sh").c_str());

    std::vector<User> users;
    std::vector<std::string> badFiles;
    for (const auto & entry : std::filesystem::directory_iterator(samplesDir)) {

        // Check if the curl'd file is valid json with jw before bothering check
        std::string jqCheck = "cat " + std::string(entry.path()) + " | jq  > /dev/null";
        if(system(jqCheck.c_str()) != 0) {
            continue;
        }
        // std::cout << entry.path() << std::endl;
        bool parsed = buildUserVectorFromFile(entry.path(), users);
        EXPECT_TRUE(parsed);
        EXPECT_TRUE(users.size() > 1); // Seems safe to assume all valid json has more than one user
        if (!parsed || users.size() <= 1) {
            badFiles.push_back(std::string(entry.path()));
        }
        users.clear();
    }

    for(auto f : badFiles) {
        std::cout << f << std::endl;
    }
}

// This is less of a unit test but saves effort of saving files
// Doesn't do any inteligent checking of the parsed data but ensures we can handle everything the url hits us with
// Have ran with 1000 itterations, 10 is probably okay though
TEST(UsersAndFriendsTest, ProcessFromAPI) {
    Logger& logger = Logger::getInstance();
    std::string url = "http://test.brightsign.io:3000";

    using namespace UsersAndFriends;

    std::vector<User> users;
    std::vector<std::string> badFiles;

    for(int i = 0; i < 5; i++) {
        SimpleUrlFetcher fetcher;
        std::string response = fetcher.fetchData(url);

        if (response.empty()) {
            logger.logError("Failed to fetch data from URL.");
            continue;
        }

        bool parsed = buildUserVectorFromJsonString(response, users);
        EXPECT_TRUE(parsed);
        // Seemws safe to assume all valid json has more than one user
        // Null data is still valid in theory
        // EXPECT_TRUE(users.size() == 0);
        if(!parsed || users.size() <= 1) {
            // Write to file if we fail to parse
            badFiles.push_back(MiscUtils::writeStringToTmpFile(response));
        }
        users.clear();
    }
    for(auto f : badFiles) {
        std::cout << f << std::endl;
    }
}
