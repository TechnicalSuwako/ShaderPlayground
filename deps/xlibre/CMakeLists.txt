cmake_minimum_required(VERSION 3.14...3.28)
project(xlibre LANGUAGES C)

if(UNIX)
  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(XLIBRE_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux-x64)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    set(XLIBRE_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/freebsd-x64)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
    set(XLIBRE_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/openbsd-x64)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
    set(LCMAKE_CURRENT_SOURCE_DIR}/lib/netbsd-x64)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    set(XLIBRE_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/illumos-x64)
  endif()
  set(XLIBRE_LIB
    ${XLIBRE_LIB_DIR}/libX11.a
    ${XLIBRE_LIB_DIR}/libX11-xcb.a
    ${XLIBRE_LIB_DIR}/libxcb.a
  )
endif()

add_library(xlibre STATIC IMPORTED GLOBAL)
set_target_properties(xlibre PROPERTIES
  IMPORTED_LOCATION "${XLIBRE_LIB}"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(UNIX)
  target_link_libraries(xlibre INTERFACE m dl)
endif()
