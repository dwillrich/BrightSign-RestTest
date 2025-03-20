#ifndef USERS_AND_FRIENDS_H
#define USERS_AND_FRIENDS_H

#include <string>
#include <vector>

class Friend {
public:
    std::string name;
    std::vector<std::string> hobbies;

    // Function to load friend from a JSON object
    void fromJson(const Json::Value& j) {
        name = j["name"].asString();  // Get the friend's name

        // Parse the list of hobbies (assumed to be an array)
        for (const auto& hobby : j["hobbies"]) {
            hobbies.push_back(hobby.asString());
        }
    }
};
    
// Class for the main user (User class)
class User {
public:
    int id;
    std::string name;
    std::string city;
    int age;
    std::vector<Friend> friends;

    // Function to load user from a JSON object
    void fromJson(const Json::Value& j) {
        id = j["id"].asInt();                 // Get the id
        name = j["name"].asString();          // Get the name
        city = j["city"].asString();          // Get the city
        age = j["age"].asInt();               // Get the age

        // Parse the list of friends
        for (const auto& f : j["friends"]) {
            Friend friendObj;
            friendObj.fromJson(f);
            friends.push_back(friendObj);
        }
    }
};

#endif // USERS_AND_FRIENDS_H
