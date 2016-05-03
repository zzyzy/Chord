cmake_minimum_required(VERSION 3.5)
project(Chord)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    include/Chord.h
    include/Node.h
    include/Utility.h
    src/Chord.cpp
    src/main.cpp
    src/Node.cpp
    src/Utility.cpp)

include_directories(include)

add_executable(Chord ${SOURCE_FILES})