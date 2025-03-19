#include <gtest/gtest.h>
#include "UrlValidator.h"

// Test valid URLs
TEST(UrlValidatorTest, ValidUrls) {
    EXPECT_TRUE(UrlValidator::isValid("http://example.com"));
    EXPECT_TRUE(UrlValidator::isValid("https://www.google.com"));
    EXPECT_TRUE(UrlValidator::isValid("https://sub.domain.example.org"));
    EXPECT_TRUE(UrlValidator::isValid("http://localhost:8080"));
    EXPECT_TRUE(UrlValidator::isValid("https://example.com/path/to/page"));
}

// Negative cases
TEST(UrlValidatorTest, InvalidUrls) {
    EXPECT_FALSE(UrlValidator::isValid("ftp://example.com"));      // Invalid scheme
    EXPECT_FALSE(UrlValidator::isValid("htt://badurl.com"));       // Typo in scheme
    EXPECT_FALSE(UrlValidator::isValid("http:/missing-slash.com"));// Missing slash
    EXPECT_FALSE(UrlValidator::isValid("://missing-scheme.com"));  // Missing scheme
    EXPECT_FALSE(UrlValidator::isValid("example.com"));            // No scheme
    EXPECT_FALSE(UrlValidator::isValid("http://"));                // Empty domain
    EXPECT_FALSE(UrlValidator::isValid("potato"));                 // nonsense

}

// Main function for running tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
