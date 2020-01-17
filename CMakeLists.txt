cmake_minimum_required(VERSION 3.5)

project(iterate_struct LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(modules/rapidjson/include)

add_executable(iterate_struct
    main.cpp
    iterate_struct.h
    value_printer.h
    ptree_converter.h
    json_doc_converter.h
    json_doc_io.h
    collect_paths_struct.h
    collect_paths_json_doc.h
    )
