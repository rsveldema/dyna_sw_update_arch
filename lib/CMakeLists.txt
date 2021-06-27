cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(DynUpdate VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_library(DynUpdate STATIC UpdateableObject.cc IFunction.cc reflection.cc UpdateLoader.cc)

target_include_directories(DynUpdate PUBLIC
                           include
                           )            
                           
target_link_libraries(DynUpdate PUBLIC -ldl)
                           
set_target_properties(DynUpdate PROPERTIES PUBLIC_HEADER "dyn_update.h")
