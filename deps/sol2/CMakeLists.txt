cmake_minimum_required(VERSION 3.14...3.28)
project(sol2)

add_library(sol2 INTERFACE)
target_include_directories(sol2 INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")