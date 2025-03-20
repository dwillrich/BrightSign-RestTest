#include "UsersAndFriends.h"
#include "Logger.h"
#include <iostream>
#include <fstream>

namespace UsersAndFriends {


void to_json(json& j, const Friend& f) {
    if(f.name) {
        j["name"] = *f.name;
    }
    j["hobbies"] = f.hobbies;
}

// void to_json(json& j, const std::vector<Friend>& fs) {
//     json ij = json(fs);
//     j = ij;
// }

void to_json(json& j, const User& u) {
    if(u.id) {
        j["id"] = *u.id;
    }
    if(u.name) {
        j["name"] = u.name;
    }
    if(u.city) {
        j["city"] = *u.city;
    }
    if(u.age) {
        j["age"] = *u.age;
    }

    j["friends"] = u.friends;

}

void from_json(const json& j, Friend& f) {
    if(j.contains("name")) {
        f.name = j.at("name").get<std::string>();
    } else {
        f.name = "Anonymous";
    }

    if(j.contains("hobbies")) {
        f.hobbies= j.at("hobbies").get<std::vector<std::string>>();
    } else {
        // What do you do all day?
        // j.hobbies = [];
    }
}

void from_json(const json& j, User& u) {
    if(j.contains("id")) {
        u.id = j.at("id").get<int>();
    }

    if(j.contains("name")) {
        u.name = j.at("name").get<std::string>();
    }

    if(j.contains("city")) {
        u.city = j.at("city").get<std::string>();
    }

    if(j.contains("age")) {
        u.age = j.at("age").get<int>();
    }

    if(j.contains("friends")) {
        u.friends = j.at("friends").get<std::vector<Friend>>();
    }
}

void to_log(const Friend& f, Logger& logger) {
    std::string hobbiesStr = "";
    for (size_t i = 0; i < f.hobbies.size(); ++i) {
        hobbiesStr += (f.hobbies)[i];
        if (i < f.hobbies.size() - 1) {
            hobbiesStr += ", ";
        }
    }
    logger.logInfo("  - " + (f.name ? *f.name : "N/A") + " (" + hobbiesStr + ")");
}

void to_log(const Friend& f) {
    to_log(f, Logger::getInstance());
}

void to_log(const User& u, Logger& logger) {
    logger.logInfo("--------------------------------------------");
    logger.logInfo("ID: " + (u.id ? std::to_string(*u.id) : "N/A"));
    logger.logInfo("Name: " + (u.name ? *u.name : "N/A"));
    logger.logInfo("City: " + (u.city ? *u.city : "N/A"));
    logger.logInfo("Age: " + (u.age ? std::to_string(*u.age) : "N/A"));
    logger.logInfo("Friends:");

    // No need for check here as using normal vector
    for (const auto& f : u.friends) {
        to_log(f, logger);
    }
}

void to_log(const User& u) {
    to_log(u, Logger::getInstance());
}

bool is_whitespace_only_ws(std::istream& is) {
    is >> std::ws;
    return is.peek() == EOF;
}

bool buildUserVectorFromIstream(std::istream& iStream, std::vector<User>& users) {
    Logger& logger = Logger::getInstance();
    json j;
    try {
        iStream >> j;

        // Annoyingly data is sometimes an array sometimes not
        if (j.is_array()) {
            // This is happy path but should be how the API just gives us the data.
            // In production anything but this path would be tossed as invalid.
            users = j.get<std::vector<User>>();
        } else {
            // This happens too often just shut the log up its like 50% of the time
            // logger.logInfo("Invalid JSON format. Expected an array. Dealing with it anyway");

            // Already piped in one entry so process that outside the loop
            auto user = j.get<User>();
            users.push_back(user);

            // Stop processing if we hit only white space
            while (!is_whitespace_only_ws(iStream)) {
                iStream >> j;
                try {
                    auto user = j.get<User>();
                    users.push_back(user);
                } catch (const json::parse_error& e) {
                    // Not valid User? What do we do here
                    logger.logError(e.what());
                }
            }
        }
    } catch (const json::parse_error& e) {
        logger.logError("This should never happen with API as implemented but catch anyway!");
        logger.logError(e.what());
        return false;
    }

    return true;
}

bool buildUserVectorFromJsonString(const std::string& jsonStr, std::vector<User>& users) {
    std::istringstream jsonStream(jsonStr);
    return buildUserVectorFromIstream(jsonStream, users);
}

bool buildUserVectorFromFile(const std::string& fileName, std::vector<User>& users) {
    Logger& logger = Logger::getInstance();
    std::ifstream file(fileName);
    if (!file) {
        logger.logError("Error opening file: " + fileName);
        return false;
    }

    // File is empty nothing to do
    if(file.peek() == std::ifstream::traits_type::eof()) {
        return true;
    }

    bool retVal = buildUserVectorFromIstream(file, users);

    if (file.is_open()) {
        file.close();
    }
    return retVal;
}

}
