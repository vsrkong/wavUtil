cmake_minimum_required(VERSION 3.23)
project(wavTool)

include_directories(include)
aux_source_directory(src SRC_LIST)

set(CMAKE_CXX_STANDARD 11)

add_executable(wavTool main.cpp ${SRC_LIST})
