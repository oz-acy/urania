cmake_minimum_required(VERSION 3.10)
project(urania CXX)
set(CMAKE_VERBOSE_MAKEFILE 1)

enable_language(CXX)
#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# 依存するライブラリ

## 姑息的解決手段
if(MSVC)
  find_package(PNG REQUIRED)
  find_package(JPEG REQUIRED)
endif()

find_package(eunomia REQUIRED)


# デバッグバージョンのpostfix
set(CMAKE_DEBUG_POSTFIX "d")


# ソースファイルとヘッダファイル
set(URANIA_SOURCES
  system.cpp
    msgloop.cpp
    sys-mbs2ws.cpp sys-ws2mbs.cpp
    #getCLArgs.cpp
    getCLArgsW.cpp
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


# build target
add_library(urania STATIC ${URANIA_SOURCES})
set_target_properties(urania
  PROPERTIES PUBLIC_HEADER "${URANIA_HEADERS}"
)
target_link_libraries(urania PRIVATE eunomia::eunomia)
target_compile_definitions(urania PRIVATE NDEBUG _UNICODE UNICODE WINVER=0x500)
if(MSVC)
  set_target_properties(urania PROPERTIES PREFIX "lib")
  target_compile_options(urania PRIVATE /source-charset:utf-8)
endif()

#target_include_directories(urania PRIVATE
#  ${EUNOMIA_INCLUDE_DIRS}
#)


## Release版ライブラリ
#add_library(urania STATIC ${URANIA_SOURCES})

#set_target_properties(urania
#  PROPERTIES PUBLIC_HEADER "${URANIA_HEADERS}"
#)
#target_link_libraries(urania PRIVATE eunomia)

#target_compile_options(urania PRIVATE
#  $<$<CXX_COMPILER_ID:GNU>:-Wall -O3>
#  $<$<CXX_COMPILER_ID:Clang>:-Wall -O3>
#  $<$<CXX_COMPILER_ID:MSVC>:/W4 /Ot>
#)

#target_compile_definitions(urania PRIVATE NDEBUG _UNICODE UNICODE WINVER=0x500)


# DEBUG版ライブラリ
#add_library(urania-debug STATIC ${URANIA_SOURCES})
#target_link_libraries(urania-debug PRIVATE eunomia-debug)

#target_compile_options(urania-debug PRIVATE
#  $<$<CXX_COMPILER_ID:GNU>:-Wall -g3 -ggdb>
#  $<$<CXX_COMPILER_ID:Clang>:-Wall -O3>
#  $<$<CXX_COMPILER_ID:MSVC>:/W4>
#)

#target_compile_definitions(urania-debug PRIVATE _UNICODE UNICODE WINVER=0x0601)


# インストール設定
install(TARGETS urania
    #urania-debug
  EXPORT urania-export
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
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
