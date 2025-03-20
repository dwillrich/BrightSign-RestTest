cmake_minimum_required(VERSION 3.10)
project(FakeRest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(CURL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_package(nlohmann_json REQUIRED)
include_directories(${nlohmann_json_INCLUDE_DIRS})

file(GLOB FAKEREST_SRC_FILES src/main.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.h)
# file(GLOB_RECURSE HPP_FILES src/*.hpp)
file(GLOB_RECURSE UTILS_FILES src/Utils/*.cpp)
file(GLOB_RECURSE UTILS_HEADERS src/Utils/*.h)


add_custom_target(format
COMMAND astyle --style=google --indent=spaces=4 --suffix=none 
${CMAKE_SOURCE_DIR}/src/*.cpp
${CMAKE_SOURCE_DIR}/src/Utils/*.cpp
${CMAKE_SOURCE_DIR}/src/Utils/*.h 
${CMAKE_SOURCE_DIR}/tests/*.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/Utils) 

add_library(fakerest-utils STATIC ${UTILS_FILES} ${UTILS_HEADERS} ${nlohmann_json_INCLUDE_DIRS})

add_executable(fakerest src/main.cpp ${HEADER_FILES})
target_link_libraries(fakerest PRIVATE fakerest-utils CURL::libcurl nlohmann_json::nlohmann_json ${JSONCPP_LIBRARIES})

add_dependencies(fakerest format)

file(GLOB TEST_FILES tests/*.cpp)
if(TEST_FILES)
    add_executable(fakerest-tests ${TEST_FILES})
    target_link_libraries(fakerest-tests PRIVATE fakerest-utils GTest::GTest GTest::Main CURL::libcurl nlohmann_json::nlohmann_json ${JSONCPP_LIBRARIES})
    include_directories(${CMAKE_SOURCE_DIR}/src)
endif()
