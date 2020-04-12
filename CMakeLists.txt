cmake_minimum_required(VERSION 3.5)

project(rsl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE RSL_SOURCES
    "src/*.h*"
    "src/*.c*"
)

add_library(rsl SHARED ${RSL_SOURCES})

add_executable(rsltest main.cpp)
target_include_directories(rsltest PUBLIC src/)
target_link_libraries(rsltest rsl)
