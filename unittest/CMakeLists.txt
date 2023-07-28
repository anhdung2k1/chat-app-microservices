cmake_minimum_required(VERSION 3.16.3)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests gtest.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
