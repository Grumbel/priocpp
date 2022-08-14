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
project(prio VERSION 0.0.0)

include(mk/cmake/TinyCMMC.cmake)

find_package(fmt REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(JSONCPP REQUIRED jsoncpp IMPORTED_TARGET)

function(build_dependencies)
  set(BUILD_TESTS OFF)

  find_package(sexp QUIET)
  if(NOT TARGET sexp::sexp)
    add_subdirectory(external/sexp-cpp/ EXCLUDE_FROM_ALL)
  endif()

  find_package(logmich QUIET)
  if(NOT TARGET logmich::logmich)
    add_subdirectory(external/logmich/ EXCLUDE_FROM_ALL)
  endif()
endfunction()
build_dependencies()

file(GLOB PRIO_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  include/prio/*.hpp)
file(GLOB PRIO_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/*.cpp)

add_library(prio STATIC ${PRIO_SOURCES})
set_target_properties(prio PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)
target_compile_options(prio PRIVATE ${WARNINGS_CXX_FLAGS})
target_include_directories(prio INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_include_directories(prio PRIVATE src/ include/prio/)
set_target_properties(prio PROPERTIES PUBLIC_HEADER "${PRIO_HEADERS}")
target_link_libraries(prio PUBLIC
  logmich::logmich
  sexp::sexp
  fmt::fmt
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
    prio::prio
    GTest::GTest
    GTest::Main)

  enable_testing()
  add_test(NAME test_prio
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND test_prio)
endif()

if(BUILD_EXTRA)
  file(GLOB PRIO_EXTRA_SOURCES_CXX RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    extra/*.cpp)
  foreach(EXTRA_SOURCE ${PRIO_EXTRA_SOURCES_CXX})
    string(REGEX REPLACE ".*/\([^/]*\).cpp" "\\1" EXTRA_EXE ${EXTRA_SOURCE})
    add_executable(${EXTRA_EXE} ${EXTRA_SOURCE})
    set_target_properties(${EXTRA_EXE} PROPERTIES
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF)
    set_target_properties(${EXTRA_EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "extra/")
    target_compile_options(${EXTRA_EXE} PRIVATE ${WARNINGS_CXX_FLAGS})
    target_link_libraries(${EXTRA_EXE} prio)
  endforeach(EXTRA_SOURCE)

  if(BUILD_TESTS)
    add_test(NAME priotool
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      COMMAND ${CMAKE_CURRENT_BINARY_DIR}/extra/priotool test/data/data.sexp)
  endif()
endif()

tinycmmc_export_and_install_library(prio)

# EOF #
