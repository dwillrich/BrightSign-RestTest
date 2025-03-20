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

// Just need sum and count of entries don't need to add up live
inline double average(int sum, int count) {
    return count > 0 ? static_cast<double>(sum) / count : 0.0;
}

// // Transform data from array of users and organize by city
// // Should run in O(n)
// // For now do not do in place and take easy route using more memory
// std::unordered_map<std::string, std::vector<User>> getCityUserMap(const std::vector<User>& users) {
//     std::unordered_map<std::string, std::vector<User>> retVal;
//     for(auto user : users) {
//         retVal[user.city.value_or("Unknown")].push_back(user);
//     }
//     // Let RVO get rid of this copy
//     return retVal;
// }

// 1. average age of all users per city
// Should run in O(n)
// For now do not do in place and take easy route using more memory
std::unordered_map<std::string,double> averageAgeOfUsersPerCity(const std::vector<User>& users) {
    Logger& logger = Logger::getInstance();
    std::unordered_map<std::string,double> retVal;

    // get rid of call to getCityUserMap and use sum and count
    std::unordered_map<std::string, std::pair<int, int>> cityAgeInfoPairs;

    for (auto user : users) {
        if (user.age) {
            // sum
            cityAgeInfoPairs[user.city.value_or("Unknown")].first += *user.age;
            // count
            cityAgeInfoPairs[user.city.value_or("Unknown")].second += 1;
        } else {
            logger.logError("Missing Age For User! Skipping - " + user.name.value_or("Anon"));
        }
    }

    for (auto [city, infoPair] : cityAgeInfoPairs) {
        retVal[city] = average(infoPair.first, infoPair.second);
    }

    // auto cityMap = getCityUserMap(users);
    // std::vector<int> agesInCity;
    // for(auto city : cityMap) {
    //     for(auto user : city.second) {
    //         if(user.age) {
    //             agesInCity.push_back(*user.age);
    //         } else {
    //             logger.logError("Missing Age For User! Skipping - " + (user.name ? *user.name : "Anon"));
    //         }
    //     }
    //     retVal[city.first] = average_vector(agesInCity);
    //     agesInCity.clear();
    // }

    // Let RVO get rid of this copy
    return retVal;
}

// 2. average number of friends per city
// Should run in O(n)
std::unordered_map<std::string,double> averageFriendsPerCity(const std::vector<User>& users) {
    std::unordered_map<std::string,double> retVal;

    // get rid of call to getCityUserMap and use sum and count
    // same concept as above
    std::unordered_map<std::string, std::pair<int, int>> cityFriendInfo;

    for (auto user : users) {
        cityFriendInfo[user.city.value_or("Unknown")].first += user.friends.size();
        cityFriendInfo[user.city.value_or("Unknown")].second += 1;
    }

    for (auto [city, info] : cityFriendInfo) {
        retVal[city] = average(info.first, info.second);
    }

    // auto cityMap = getCityUserMap(users);
    // std::vector<int> friendCountInCity;
    // for(auto city : cityMap) {
    //     for(auto user : city.second) {
    //         friendCountInCity.push_back(user.friends.size());
    //     }
    //     retVal[city.first] = average_vector(friendCountInCity);
    //     friendCountInCity.clear();
    // }
    // Let RVO get rid of this copy
    return retVal;
}

// 3. the user with the most friends per city
// Should run in O(n)
// Note : this says user not user name so despite only returning the users name return whole object
std::unordered_map<std::string,User> mostPopularUserPerCity(const std::vector<User>& users) {
    if(users.size() < 1) {
        return std::unordered_map<std::string,User>();
    }
    std::unordered_map<std::string,User> retVal;

    for (auto user : users) {
        // value_or is cool, didn't know optional had that
        std::string city = user.city.value_or("Unknown");
        if (retVal.find(city) == retVal.end() || user.friends.size() > retVal[city].friends.size()) {
            retVal[city] = user;
        }
    }

    // auto cityMap = getCityUserMap(users);
    // for(auto city : cityMap) {
    //     // Safe to assume all cities have atleast one user
    //     retVal[city.first] = city.second[0];
    //     for(auto user : city.second) {
    //         if(user.friends.size() > retVal[city.first].friends.size()) {
    //             retVal[city.first] = user;
    //         }
    //     }
    // }

    // Let RVO get rid of this copy
    return retVal;
}

// 4. the most common first name in all cities
// Should run in O(n)
// Note : Include friends
std::string mostPopularName(const std::vector<User>& users) {
    if(users.size() < 1) {
        return "";
    }

    std::unordered_map<std::string, int> nameCountMap;
    std::string popularName;
    int leadNameCount = 0;

    for(auto user : users) {
        // For users
        if(nameCountMap.find(*user.name) != nameCountMap.end()) {
            nameCountMap[*user.name]++;
            if(nameCountMap[*user.name] > leadNameCount) {
                popularName = *user.name;
                leadNameCount = nameCountMap[*user.name];
            }
        } else {
            // First one
            nameCountMap[*user.name] = 1;
            if(nameCountMap[*user.name] > leadNameCount) {
                popularName = *user.name;
                leadNameCount = nameCountMap[*user.name];
            }
        }

        // For their friends
        for(auto f : user.friends) {
            if(nameCountMap.find(*f.name) != nameCountMap.end()) {
                nameCountMap[*f.name]++;
                if(nameCountMap[*f.name] > leadNameCount) {
                    popularName = *f.name;
                    leadNameCount = nameCountMap[*f.name];
                }
            } else {
                // First one
                nameCountMap[*f.name] = 1;
                if(nameCountMap[*f.name] > leadNameCount) {
                    popularName = *f.name;
                    leadNameCount = nameCountMap[*f.name];
                }
            }
        }
    }
    return popularName;
}

// 5. the most common hobby of all friends of users in all cities
// Should run in O(n) (n = hobbies + friends)
std::string mostPopularHobby(const std::vector<User>& users) {
    if(users.size() < 1) {
        return "";
    }

    std::unordered_map<std::string, int> hobbyCountMap;
    std::string popularHobby;
    int leadHobbyCount = 0;

    for(auto user : users) {
        // For users they don't have hobbies
        // For their friends
        for(auto f : user.friends) {
            for(auto h : f.hobbies) {
                if(hobbyCountMap.find(h) != hobbyCountMap.end()) {
                    hobbyCountMap[h]++;
                    if(hobbyCountMap[h] > leadHobbyCount) {
                        leadHobbyCount = hobbyCountMap[h];
                        popularHobby = h;
                    }
                } else {
                    // First one
                    hobbyCountMap[h] = 1;
                }
            }
        }
    }

    return popularHobby;
}
} // namespace UsersAndFriends
