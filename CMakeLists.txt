cmake_minimum_required(VERSION 3.10)
project(FakeRest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(CURL REQUIRED)
find_package(jsoncpp REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(fakerest ${SRC_FILES})
target_link_libraries(fakerest PRIVATE CURL::libcurl jsoncpp_lib)

add_custom_target(format
    COMMAND astyle --style=google --indent=spaces=4 --suffix=none ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.h
)

add_dependencies(fakerest format)

file(GLOB_RECURSE TEST_FILES test/*.cpp)
if(TEST_FILES)
    add_executable(fakerest-tests ${TEST_FILES})
    target_link_libraries(fakerest-tests PRIVATE CURL::libcurl jsoncpp_lib)
endif()
