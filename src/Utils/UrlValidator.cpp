#include "UrlValidator.h"
#include <regex>
#include <iostream>

bool UrlValidator::isValid(const std::string& url) {
    const std::regex pattern(R"(^https?://([a-zA-Z0-9-]+\.)+[a-zA-Z]{2,6}(:[0-9]{1,5})?(/.*)?$)");

    return std::regex_match(url, pattern);
}
