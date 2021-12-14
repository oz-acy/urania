# CMakeLists.txt
#
# by oZ/acy
# Copyright 2021 oZ/acy (名賀月晃嗣) <acy@hiemalis.org>
#
# Redistribution and use in source and binary forms, 
#     with or without modification, 
#   are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.19)
project(urania CXX)
set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)


option(BUILD_SHARED_LIBS
  "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)"
  OFF
)

# staticなライブラリをビルドする場合は、依存ライブラリもstaticにする。
if (NOT BUILD_SHARED_LIBS AND NOT MSVC)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .a;.dll.a;.lib)
endif()


# 依存するライブラリ
find_package(PNG)
find_package(JPEG)
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


#### liburania ########
add_library(urania ${URANIA_SOURCES})
set_target_properties(urania
  PROPERTIES PUBLIC_HEADER "${URANIA_HEADERS}"
)
target_link_libraries(urania PRIVATE eunomia::eunomia)
target_compile_definitions(urania PRIVATE NDEBUG _UNICODE UNICODE WINVER=0x0A00)
if(MSVC)
  set_target_properties(urania PROPERTIES PREFIX "lib")
  target_compile_options(urania PRIVATE /source-charset:utf-8)
endif()


# インストール設定
install(TARGETS urania
  EXPORT urania-export
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/urania
)

install(EXPORT urania-export
  FILE urania-config.cmake
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/urania/
  NAMESPACE urania::
  EXPORT_LINK_INTERFACE_LIBRARIES
)


#eof
