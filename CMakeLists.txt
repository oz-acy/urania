# CMakeLists.txt for project URANIA

cmake_minimum_required(VERSION 3.10)
set(CMAKE_VERBOSE_MAKEFILE 1)
project(urania CXX)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# 依存するライブラリ
find_package(themis REQUIRED)
find_package(polymnia REQUIRED)
#find_package(ZLIB REQUIRED)
#find_package(PNG REQUIRED)
#find_package(JPEG REQUIRED)


set(URANIA_SOURCES
  system.cpp
  msgloop.cpp
  sys-mbs2ws.cpp sys-ws2mbs.cpp
  getCLArgs.cpp getCLArgsW.cpp
  getLongPath.cpp
  ctrl.cpp ctrl_sb.cpp
  pdev.cpp pdev2.cpp pmdev.cpp pmdidx.cpp
  bwin.cpp
  dialog.cpp
  menu.cpp menudesc.cpp
  win.cpp scroll-h.cpp drop-h.cpp
  clkpanel.cpp
  cmndlg.cpp
  registry.cpp regread.cpp regwrite.cpp
)

set(URANIA_HEADERS
  decl.h
  system.h
  wbase.h
  bwin.h
  paintdev.h
  dialog.h
  menu.h
  window.h
  clkpanel.h
  cmndlg.h
  registry.h
)




#SET(CMAKE_CXX_FLAGS "-Wall -std=c++17 -O3 -DWINVER=0x500 -D_UNICODE -DUNICODE")
##SET(CMAKE_CXX_FLAGS "-Wall -std=c++14 -O3 -DWINVER=0x500 -D_UNICODE -DUNICODE")
#include_directories("${CMAKE_INSTALL_PREFIX}/include")


# Release版ライブラリ
add_library(urania STATIC ${URANIA_SOURCES})

set_target_properties(urania
  PROPERTIES PUBLIC_HEADER "${URANIA_HEADERS}"
)
target_link_libraries(urania polymnia themis)

target_compile_options(urania PRIVATE
  $<$<CXX_COMPILER_ID:Clang>:-Wall -O3>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -O3>
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /Ot>
)

target_compile_definitions(urania PRIVATE NDEBUG _UNICODE UNICODE WINVER=0x500)




install(TARGETS urania
  EXPORT urania-export
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION include
  PUBLIC_HEADER DESTINATION include/urania
)

install(EXPORT urania-export
  FILE urania-config.cmake
  DESTINATION share/cmake/urania/
#  EXPORT_LINK_INTERFACE_LIBRARIES
)




#eof
