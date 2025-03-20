#ifndef USERS_AND_FRIENDS_H
#define USERS_AND_FRIENDS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "Logger.h"

// Don't like doing this in header but fine for now
using json = nlohmann::json;

namespace UsersAndFriends {

struct Friend {
    std::optional<std::string> name;
    std::vector<std::string> hobbies;
};

struct User {
    std::optional<int> id;
    std::optional<std::string> name;
    std::optional<std::string> city;
    std::optional<int> age;
    std::vector<Friend> friends;
};

void from_json(const json& j, Friend& f);
void from_json(const json& j, User& p);

// Can probably just print JSON here but this is fine
void to_log(const Friend& f, Logger& logger);
void to_log(const Friend& f);
void to_log(const User& u, Logger& logger);
void to_log(const User& u);

};

#endif // USERS_AND_FRIENDS_H
