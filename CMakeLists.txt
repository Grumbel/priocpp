# prio - Property I/O for C++
# Copyright (C) 2020 Ingo Ruhnke <grumbel@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.6)
project(prio)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/external/cmake-modules/)

include(GNUInstallDirs)
include(MaximumWarnings)
include(ClangTidy)

find_package(PkgConfig REQUIRED)
pkg_search_module(JSONCPP REQUIRED jsoncpp IMPORTED_TARGET)

function(build_dependencies)
  set(BUILD_TESTS OFF)
  if(NOT TARGET sexp)
    add_subdirectory(external/sexp-cpp/ EXCLUDE_FROM_ALL)
  endif()
  if(NOT TARGET logmich)
    add_subdirectory(external/logmich/ EXCLUDE_FROM_ALL)
  endif()
endfunction()
build_dependencies()

file(GLOB PRIO_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  include/prio/*.hpp)
file(GLOB PRIO_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/reader/*.cpp)

add_library(prio STATIC ${PRIO_SOURCES})
set_target_properties(prio PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)
target_include_directories(prio INTERFACE include/)
target_include_directories(prio PRIVATE src/)
set_target_properties(prio PROPERTIES PUBLIC_HEADER "${PRIO_HEADERS}")
target_link_libraries(prio PUBLIC
  logmich
  sexp
  PkgConfig::JSONCPP)

if(BUILD_TESTS)
  if(NOT TARGET GTest)
    find_package(GTest REQUIRED)
  endif()

  file(GLOB TEST_PRIO_SOURCES test/*_test.cpp)
  add_executable(test_prio ${TEST_PRIO_SOURCES})
  set_target_properties(test_prio PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)
  target_compile_options(test_prio PRIVATE ${WARNINGS_CXX_FLAGS})
  target_include_directories(test_prio PRIVATE src/)
  target_link_libraries(test_prio
    prio
    GTest::GTest
    GTest::Main)

  enable_testing()
  add_test(NAME test_prio
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND test_prio)
endif()

install(TARGETS prio
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")

# EOF #