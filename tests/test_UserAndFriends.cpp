#include <gtest/gtest.h>
#include "UsersAndFriends.h"
#include <filesystem>

// // Test valid URLs
// TEST(UrlValidatorTest, ValidUrls) {
//     EXPECT_TRUE(UrlValidator::isValid("http://example.com"));
//     EXPECT_TRUE(UrlValidator::isValid("https://www.google.com"));
//     EXPECT_TRUE(UrlValidator::isValid("https://sub.domain.example.org"));
//     EXPECT_TRUE(UrlValidator::isValid("http://localhost:8080"));
//     EXPECT_TRUE(UrlValidator::isValid("https://example.com/path/to/page"));
// }

// // Negative cases
// TEST(UrlValidatorTest, InvalidUrls) {
//     EXPECT_FALSE(UrlValidator::isValid("ftp://example.com"));      // Invalid scheme
//     EXPECT_FALSE(UrlValidator::isValid("htt://badurl.com"));       // Typo in scheme
//     EXPECT_FALSE(UrlValidator::isValid("http:/missing-slash.com"));// Missing slash
//     EXPECT_FALSE(UrlValidator::isValid("://missing-scheme.com"));  // Missing scheme
//     EXPECT_FALSE(UrlValidator::isValid("example.com"));            // No scheme
//     EXPECT_FALSE(UrlValidator::isValid("http://"));                // Empty domain
//     EXPECT_FALSE(UrlValidator::isValid("potato"));                 // nonsense

// }

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
        if(!parsed) {
            badFiles.push_back(std::string(entry.path()));
        }
        users.clear();
    }

    for(auto f : badFiles) {
        std::cout << f << std::endl;
    }
}
