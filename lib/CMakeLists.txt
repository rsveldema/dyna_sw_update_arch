cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(DynUpdate VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_library(DynUpdate dyn_update.cc)

target_include_directories(DynUpdate PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
                           