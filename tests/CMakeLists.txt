cmake_minimum_required(VERSION 3.10)
include(GoogleTest)

# set the project name and version
project(T1 VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(T1 test_init.cc)

target_include_directories(T1 PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

target_link_libraries(T1 PUBLIC DynUpdate gtest pthread)

gtest_add_tests(TARGET T1
                SOURCES test_init.cc)
