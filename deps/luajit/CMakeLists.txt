cmake_minimum_required(VERSION 3.14...3.28)
project(luajit LANGUAGES C)

if(WIN32)
  set(LUAJIT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/windows-x64)
  # set(LUAJIT_LIB ${LUAJIT_LIB_DIR}/luajit.lib)
  set(LUAJIT_LIB ${LUAJIT_LIB_DIR}/lua51.lib)
else()
  if(UNIX AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LUAJIT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux-x64)
  elseif(UNIX AND CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    set(LUAJIT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/freebsd-x64)
  elseif(UNIX AND CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
    set(LUAJIT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/openbsd-x64)
  elseif(UNIX AND CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
    set(LUAJIT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/netbsd-x64)
  elseif(UNIX AND CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    set(LUAJIT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/illumos-x64)
  endif()
  set(LUAJIT_LIB ${LUAJIT_LIB_DIR}/luajit.a)
endif()

add_library(luajit STATIC IMPORTED GLOBAL)
set_target_properties(luajit PROPERTIES
  IMPORTED_LOCATION "${LUAJIT_LIB}"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(UNIX)
  target_link_libraries(luajit INTERFACE m dl)
endif()
