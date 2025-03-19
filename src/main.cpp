#include <iostream>
#include <json/json.h>
#include <curl/curl.h>

#include "UrlValidator.h"

int main(int argc, char* argv[]) {
    std::string url = "http://test.brightsign.io:3000";

    if (argc == 2) {
        url = argv[1];
    } else {
        std::cerr << "Invalid args!" << std::endl;
        exit(1);
    }

    if(!UrlValidator::isValid(url)) {
        std::cerr << "Invalid URL!" << std::endl;
    }

    return 0;
}
