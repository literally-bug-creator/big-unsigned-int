cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file (GLOB_RECURSE SOURCES "source/*.cpp")
add_library(big_unsigned_int SHARED ${SOURCES})

target_include_directories(big_unsigned_int PUBLIC include)
