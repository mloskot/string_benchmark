cmake_minimum_required(VERSION 2.8)
project(string_benchmark CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
  message(STATUS "Configuring default CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
else()
  message(STATUS "Configuring CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
endif()

message(STATUS "Compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} (${CMAKE_CXX_COMPILER})")
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX14)
  message(STATUS "C++ Standard: C++14")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX11 OR MSVC_VERSION GREATER 1800)
  message(STATUS "C++ Standard: C++11")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  endif()
else()
    message(WARNING "Compiler ${CMAKE_CXX_COMPILER} has no C++ 11/14 support.")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /W4")
endif()

find_package(Boost 1.54.0)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_definitions("-DHAS_BOOST")
endif()
message(STATUS "Boost_FOUND=${Boost_FOUND}")
message(STATUS "Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}")

find_package(Git REQUIRED)
include(ExternalProject)

message(STATUS "Configuring Catch")
ExternalProject_Add(catch
  GIT_REPOSITORY https://github.com/philsquared/Catch.git
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON)
ExternalProject_Get_Property(catch SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)

message(STATUS "Configuring Celero")
ExternalProject_Add(celero
  GIT_REPOSITORY https://github.com/DigitalInBlue/Celero.git
  UPDATE_COMMAND ""
  INSTALL_DIR ${CMAKE_BINARY_DIR}
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_BINARY_DIR}
    -DCELERO_COMPILE_DYNAMIC_LIBRARIES:BOOL=OFF
    -DCELERO_ENABLE_EXPERIMENTS:BOOL=OFF
    -DCELERO_ENABLE_FOLDERS:BOOL=OFF)
ExternalProject_Get_Property(celero SOURCE_DIR)
ExternalProject_Get_Property(celero INSTALL_DIR)
add_library(libcelero UNKNOWN IMPORTED)
include_directories(${INSTALL_DIR}/include)
set_property(TARGET libcelero
  PROPERTY
  IMPORTED_LOCATION_DEBUG
  "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}celerod${CMAKE_STATIC_LIBRARY_SUFFIX}")
set_property(TARGET libcelero
  PROPERTY
  IMPORTED_LOCATION_RELEASE
  "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}celero${CMAKE_STATIC_LIBRARY_SUFFIX}")

if(MSVC)
  set(CELERO_EXTRA_LIBS PowrProf.lib)
endif()
add_dependencies(libcelero celero)

set(test test_fixture)
message(STATUS "Configuring test: ${test}")
add_executable(${test} ${test}.cpp)
add_dependencies(${test} catch libcelero)
add_custom_target(run_${test} ALL $<TARGET_FILE:${test}> DEPENDS ${test})

file(GLOB headers *.hpp)
file(GLOB sources fixture.cpp)
set(benchmarks
  benchmark_stod
  benchmark_stoi
  benchmark_starts_with
  benchmark_ends_with
  benchmark_istarts_with
  benchmark_iends_with
  benchmark_escape_xml)

foreach(benchmark ${benchmarks})
  set(btarget ${benchmark}) # benchmark char-based strings
  message(STATUS "Configuring benchmark: ${btarget}")
  add_executable(${btarget} ${benchmark}.cpp ${sources} ${headers})
  add_dependencies(${btarget} libcelero)
  target_link_libraries(${btarget} libcelero ${CELERO_EXTRA_LIBS})
  add_custom_target(run_${btarget} ALL
    COMMAND ${btarget} ARGS -t ${btarget}.csv
    DEPENDS run_${test} ${btarget})

  string(REPLACE "benchmark_" "benchmark_w" btarget ${benchmark}) # benchmark wchar_t-based strings
  message(STATUS "Configuring benchmark: ${btarget}")
  add_executable(${btarget} ${benchmark}.cpp ${sources} ${headers})
  set_property(TARGET ${btarget} PROPERTY COMPILE_DEFINITIONS STRING_BENCHMARK_WCHAR=1)
  add_dependencies(${btarget} libcelero)
  target_link_libraries(${btarget} libcelero ${CELERO_EXTRA_LIBS})
  add_custom_target(run_${btarget} ALL
    COMMAND ${btarget} ARGS -t ${btarget}.csv
    DEPENDS run_${test} ${btarget})
endforeach()
