#include "UsersAndFriends.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include "Logger.h"

namespace UsersAndFriends {

double average_vector(const std::vector<int>& vec) {
    if (vec.empty()) {
        return 0.0; // Return 0 if the vector is empty
    }
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    return static_cast<double>(sum) / vec.size();
}


// Transform data from array of users and organize by city
// Should run in O(n)
// For now do not do in place and take easy route using more memory
std::map<std::string, std::vector<User>> getCityUserMap(const std::vector<User>& users) {
    std::map<std::string, std::vector<User>> retVal;
    for(auto user : users) {
        std::string cityName = (user.city ? *user.city : "Unknown");
        retVal[cityName].push_back(user);
    }
    return retVal;
}

// 1. average age of all users per city
// Should run in O(2n)?????????
// For now do not do in place and take easy route using more memory
std::map<std::string,double> averageAgeOfUsersPerCity(const std::vector<User>& users) {
    Logger& logger = Logger::getInstance();
    std::map<std::string,double> retVal;
    auto cityMap = getCityUserMap(users);
    std::vector<int> agesInCity;
    for(auto city : cityMap) {
        for(auto user : city.second) {
            if(user.age) {
                agesInCity.push_back(*user.age);
            } else {
                logger.logError("Missing Age For User! Skipping - " + (user.name ? *user.name : "Anon"));
            }
        }
        retVal[city.first] = average_vector(agesInCity);
        agesInCity.clear();
    }
    return retVal;
}

// 2. average number of friends per city
std::map<std::string,double> averageFriendsPerCity(const std::vector<User>& users) {
    std::map<std::string,double> retVal;
    auto cityMap = getCityUserMap(users);
    std::vector<int> friendCountInCity;
    for(auto city : cityMap) {
        for(auto user : city.second) {
            friendCountInCity.push_back(user.friends.size());
        }
        retVal[city.first] = average_vector(friendCountInCity);
        friendCountInCity.clear();
    }
    return retVal;
}

// 3. the user with the most friends per city
User mostPopularUserPerCity(const std::vector<User>& users) {
    return User();
}

// 4. the most common first name in all cities
// Note : Include friends
std::string mostPopularName(const std::vector<User>& users) {
    return "Derek";
}

// 5. the most common hobby of all friends of users in all cities
std::string mostPopularHobby(const std::vector<User>& users) {
    return "Coding";
}
} // namespace UsersAndFriends
