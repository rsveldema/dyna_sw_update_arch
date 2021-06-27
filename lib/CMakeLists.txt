cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(DynUpdate VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_library(DynUpdate STATIC dyn_update.cc IFunction.cc reflection.cc)

target_include_directories(DynUpdate PUBLIC
                           include
                           )                           
                           
set_target_properties(DynUpdate PROPERTIES PUBLIC_HEADER "dyn_update.h")
