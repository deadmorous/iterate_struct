cmake_minimum_required(VERSION 3.5)

project(iterate_struct)

find_package(Boost)

include_directories (${Boost_INCLUDE_DIRS})
include_directories(modules/rapidjson/include)
include_directories(include)

add_subdirectory(example)

