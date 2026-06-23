cmake_minimum_required(VERSION 3.14...3.28)
project(sqlite3 LANGUAGES C)

add_library(sqlite3 STATIC sqlite3.c sqlite3.h)

target_include_directories(sqlite3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(sqlite3
  PUBLIC
    SQLITE_ENABLE_RTREE
    SQLITE_ENABLE_FTS5
    SQLITE_ENABLE_JSON1
    SQLITE_ENABLE_DBSTAT_VTAB
)

target_compile_definitions(sqlite3 PRIVATE
  SQLITE_THREADSAFE=1
  SQLITE_OMIT_LOAD_EXTENSION
)

set_target_properties(sqlite3 PROPERTIES
  POSITION_INDEPENDENT_CODE ON
  C_STANDARD 99
  C_STANDARD_REQUIRED ON
)

source_group("Header Files" FILES sqlite3.h)
source_group("Source Files" FILES sqlite3.c)