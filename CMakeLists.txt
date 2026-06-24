cmake_minimum_required(VERSION 3.14...3.28)

project(shaderplayground LANGUAGES CXX VERSION 0.1.0)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "デバッグ又はリリース" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "静的リンク" FORCE)

if(MSVC) 
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  add_compile_options(/utf-8)
endif()

set(DB_NAME "shaderplayground.db" CACHE STRING "データベースのパス" FORCE)

add_subdirectory(deps/glad)

option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)
add_subdirectory(deps/glfw)

if(UNIX)
  find_package(OpenGL REQUIRED)
endif()

add_subdirectory(deps/glm)
add_subdirectory(deps/imgui)
add_subdirectory(deps/luajit)
add_subdirectory(deps/sol2)
add_subdirectory(deps/sqlite)
add_subdirectory(deps/stb_image)

file(GLOB_RECURSE SHADERPLAYGROUND_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")
file(GLOB_RECURSE SHADERPLAYGROUND_INC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hh")

add_executable(shaderplayground main.cc ${SHADERPLAYGROUND_SRC} ${SHADERPLAYGROUND_INC})
set_target_properties(shaderplayground PROPERTIES CXX_STANDARD 20)

set(PLATFORM_LIBS "")

if(UNIX)
  if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    message(STATUS "【！】Solaris又はIllumosで静的リンクが不可能為、動的リンクで伺います。")
  else()
    target_link_options(shaderplayground PRIVATE -static-libstdc++ -static-libgcc)
  endif()

  if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    list(APPEND PLATFORM_LIBS
      -Wl,-Bstatic
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/illumos-x64/libX11.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/illumos-x64/libxcb.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/illumos-x64/libXau.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/illumos-x64/libXdmcp.a
      -Wl,-Bdynamic
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND PLATFORM_LIBS
      -Wl,-Bstatic
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/linux-x64/libX11.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/linux-x64/libxcb.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/linux-x64/libXau.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/linux-x64/libXdmcp.a
      -Wl,-Bdynamic
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    list(APPEND PLATFORM_LIBS
      -Wl,-Bstatic
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/freebsd-x64/libX11.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/freebsd-x64/libxcb.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/freebsd-x64/libXau.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/freebsd-x64/libXdmcp.a
      -Wl,-Bdynamic
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
    list(APPEND PLATFORM_LIBS
      -Wl,-Bstatic
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/openbsd-x64/libX11.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/openbsd-x64/libxcb.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/openbsd-x64/libXau.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/openbsd-x64/libXdmcp.a
      -Wl,-Bdynamic
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
    list(APPEND PLATFORM_LIBS
      -Wl,-Bstatic
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/netbsd-x64/libX11.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/netbsd-x64/libxcb.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/netbsd-x64/libXau.a
      ${CMAKE_SOURCE_DIR}/deps/xlibre/lib/netbsd-x64/libXdmcp.a
      -Wl,-Bdynamic
    )
  endif()

  list(APPEND PLATFORM_LIBS OpenGL::GL)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  set(DB_NAME "shaderplayground.db" CACHE STRING "データベースのパス" FORCE)
  set(PRODUCTION_BUILD OFF CACHE BOOL "リリース版？" FORCE)
  target_compile_definitions(shaderplayground PRIVATE
    DB_NAME="shaderplayground.db"
    DB_RELATIVE_TO_EXE
  )
else()
  set(PRODUCTION_BUILD ON CACHE BOOL "リリース版？" FORCE)
  if(WIN32)
    set(DB_NAME "$ENV{LOCALAPPDATA}/076Studio/ShaderPlayground/data/shaderplayground.db" CACHE STRING "データベースのパス" FORCE)
  elseif(UNIX)
    set(DB_NAME "$ENV{HOME}/.local/share/shaderplayground/shaderplayground.db" CACHE STRING "データベースのパス" FORCE)
  else()
    set(DB_NAME "shaderplayground.db" CACHE STRING "データベースのパス" FORCE)
  endif()
  target_compile_definitions(shaderplayground PRIVATE PRODUCTION_BUILD DB_NAME="${DB_NAME}")
endif()

message(STATUS "DB_NAME = ${DB_NAME}")

if(WIN32)
  set(INSTALL_BINDIR "C:/Program Files/076Studio/ShaderPlayground")
  set(INSTALL_DATADIR "$ENV{LOCALAPPDATA}/076Studio/ShaderPlayground")
else()
  include(GNUInstallDirs)
  set(INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR})
  set(INSTALL_DATADIR ${CMAKE_INSTALL_DATADIR}/shaderplayground/data)
endif()

install(TARGETS shaderplayground
  RUNTIME DESTINATION ${INSTALL_BINDIR}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  COMPONENT Runtime
)

install(DIRECTORY DESTINATION ${INSTALL_DATADIR}
  COMPONENT Runtime
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/shaderplayground.db")
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/shaderplayground.db"
    DESTINATION ${INSTALL_DATADIR}
    COMPONENT Runtime
  )
endif()

if(MSVC)
  target_compile_definitions(shaderplayground PUBLIC _CRT_SECURE_NO_WARNINGS)
elseif(MINGW)
  if(PRODUCTION_BUILD)
    target_link_options(shaderplayground PRIVATE -mwindows)
  endif()
elseif(APPLE OR UNIX)
  if(PRODUCTION_BUILD)
    target_link_options("${CMAKE_PROJECT_NAME}" PRIVATE -s)
  endif()
endif()

target_link_libraries(shaderplayground PRIVATE
  glad
  glfw
  imgui
  luajit
  sqlite3
  sol2
  ${PLATFORM_LIBS}
)

target_include_directories(shaderplayground PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(MSVC)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT shaderplayground)
endif()