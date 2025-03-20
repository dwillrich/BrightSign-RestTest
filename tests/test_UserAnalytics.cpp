#include <gtest/gtest.h>
#include "UserAnalytics.h"

using namespace UsersAndFriends;

TEST(AverageAgeOfUsersPerCity, UserAnalyticsTest) {
    std::vector<User> users = {
        {1, "Alice", "Boston", 25, {}},
        {2, "Bob", "Boston", 35, {}},
        {3, "Charlie", "Austin", 30, {}}
    };

    auto result = averageAgeOfUsersPerCity(users);
    EXPECT_DOUBLE_EQ(result["Boston"], 30.0);
    EXPECT_DOUBLE_EQ(result["Austin"], 30.0);
}

TEST(AverageFriendsPerCity, UserAnalyticsTest) {
    std::vector<User> users = {
        {1, "Alice", "Boston", 25, {{"Bob", {"Reading"}}, {"Charlie", {"Gaming"}}}},
        {2, "Bob", "Boston", 35, {{"Alice", {"Cooking"}}}},
        {3, "Charlie", "Austin", 30, {{"Dave", {"Swimming"}}, {"Eve", {"Running"}}}}
    };

    auto result = averageFriendsPerCity(users);
    EXPECT_DOUBLE_EQ(result["Boston"], 1.5);
    EXPECT_DOUBLE_EQ(result["Austin"], 2.0);
}

TEST(MostPopularUserPerCity, UserAnalyticsTest) {
    std::vector<User> users = {
        {1, "Alice", "Boston", 25, {{"Bob", {"Reading"}}, {"Charlie", {"Gaming"}}, {"Dave", {"Gaming"}}}},
        {2, "Bob", "Boston", 35, {{"Alice", {"Cooking"}}}},
        {3, "Charlie", "Austin", 30, {{"Dave", {"Swimming"}}, {"Eve", {"Running"}}}}
    };

    auto result = mostPopularUserPerCity(users);
    EXPECT_EQ(result["Boston"].name, "Alice");
    EXPECT_EQ(result["Austin"].name, "Charlie");
}

TEST(MostPopularName, UserAnalyticsTest) {
    std::vector<User> users = {
        {1, "Alice", "Boston", 25, {{"Bob", {"Reading"}}, {"Charlie", {"Gaming"}}, {"Dave", {"Gaming"}}}},
        {2, "Bob", "Boston", 35, {{"Alice", {"Cooking"}}}},
        {3, "Charlie", "Austin", 30, {{"Alice", {"Cooking"}}, {"Dave", {"Swimming"}}, {"Eve", {"Running"}}}}
    };

    auto result = mostPopularName(users);
    EXPECT_EQ(result, "Alice");
}

TEST(MostPopularHobby, UserAnalyticsTest) {
    std::vector<User> users = {
        {1, "Alice", "Boston", 25, {{"Bob", {"Reading"}}, {"Charlie", {"Gaming"}}}},
        {2, "Bob", "Boston", 35, {{"Alice", {"Cooking"}}}},
        {3, "Charlie", "Austin", 30, {{"Dave", {"Swimming"}}, {"Eve", {"Running"}}, {"Bob", {"Running"}}}}
    };

    auto result = mostPopularHobby(users);
    EXPECT_EQ(result, "Running");
}
