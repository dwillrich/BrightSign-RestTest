#include "UsersAndFriends.h"
#include "Logger.h"
#include <iostream>
#include <fstream>

namespace UsersAndFriends {

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

bool buildUserVectorFromFile(std::string fileName, std::vector<User>& users) {
    Logger& logger = Logger::getInstance();
    std::ifstream file(fileName);
    if (!file) {
        logger.logError("Error opening file: " + fileName);
        return false;
    }

    json j;
    try {
        file >> j;

        // Annoyingly daya is sometimes an array sometimes not
        if (j.is_array()) {
            users = j.get<std::vector<User>>();
        } else {
            logger.logInfo("Invalid JSON format. Expected an array. Dealing with it anyway");

            // Already piped in one entry so process that outside the loop
            auto person = j.get<User>();
            users.push_back(person);

            // Check if the line is valid json first
            // Stop processing if invalid JSON is found
            while (j.accept(file)) {
                file >> j;
                auto user = j.get<User>();
                users.push_back(user);
            }
        }
    } catch (const json::parse_error& e) {
        logger.logError("This should never happen with API as implemented but catch anyway!");
        logger.logError(e.what());
        if (file.is_open()) {
            file.close();
        }
        return false;
    }

    if (file.is_open()) {
        file.close();
    }

    return true;
}

}
