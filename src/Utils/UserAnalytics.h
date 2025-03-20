#ifndef USERS_ANALYTICS_H
#define USERS_ANALYTICS_H

#include <string>
#include <vector>
#include "Logger.h"
#include "UsersAndFriends.h"

// Use same namespace for analytics
namespace UsersAndFriends {

// 1. average age of all users per city
std::map<std::string,double> averageAgeOfUsersPerCity(const std::vector<User>& users);

// 2. average number of friends per city
std::map<std::string,double> averageFriendsPerCity(const std::vector<User>& users);

// 3. the user with the most friends per city
std::map<std::string,User> mostPopularUserPerCity(const std::vector<User>& users);

// 4. the most common first name in all cities
// Note : Include friends I assume here
// Note : Says "in all" cities so going with one overall answer instead of one per city
std::string mostPopularName(const std::vector<User>& users);

// 5. the most common hobby of all friends of users in all cities
std::string mostPopularHobby(const std::vector<User>& users);

} // namespace UsersAndFriends

#endif // USERS_ANALYTICS_H
