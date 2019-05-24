cmake_minimum_required(VERSION 3.5.1)
project(Stoneshire)

include_directories(include)

file(GLOB SOURCES "src/*.c")
add_executable(stoneshire ${SOURCES})