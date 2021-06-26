cmake_minimum_required(VERSION 3.10)
include(GoogleTest)

# set the project name and version
project(T1 VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(T1 main.cc test_access.cc)

target_include_directories(T1 PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

target_link_libraries(T1 PUBLIC DynUpdate gtest pthread)

gtest_discover_tests(T1 TEST_PREFIX new:)
add_test(NAME monolithic COMMAND tests)