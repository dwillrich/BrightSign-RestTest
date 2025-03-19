#ifndef URL_VALIDATOR_H
#define URL_VALIDATOR_H

#include <string>

class UrlValidator {
  public:
    static bool isValid(const std::string& url);
};

#endif // URL_VALIDATOR_H
