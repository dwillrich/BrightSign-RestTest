#include <gtest/gtest.h>
#include "UrlValidator.h"

TEST(UrlValidatorTest, ValidUrls) {
    EXPECT_TRUE(UrlValidator::isValid("http://example.com"));
    EXPECT_TRUE(UrlValidator::isValid("https://www.google.com"));
    EXPECT_TRUE(UrlValidator::isValid("https://sub.domain.example.org"));
    EXPECT_TRUE(UrlValidator::isValid("https://example.com/path/to/page"));
}

TEST(UrlValidatorTest, InvalidUrls) {
    EXPECT_FALSE(UrlValidator::isValid("ftp://example.com"));
    EXPECT_FALSE(UrlValidator::isValid("htt://badurl.com"));
    EXPECT_FALSE(UrlValidator::isValid("http:/missing-slash.com"));
    EXPECT_FALSE(UrlValidator::isValid("://missing-scheme.com"));
    EXPECT_FALSE(UrlValidator::isValid("example.com"));
    EXPECT_FALSE(UrlValidator::isValid("http://"));
    EXPECT_FALSE(UrlValidator::isValid("potato"));
    EXPECT_FALSE(UrlValidator::isValid("http://localhost:8080"));

}
