cmake_minimum_required(VERSION 3.0)
project(string_benchmark CXX)

option(ENABLE_BENCHMARK_RUN "Run benchmarks after successful build" ON)

include(CTest)

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
  add_definitions("-DVC_EXTRALEAN")
  add_definitions("-DWIN32_LEAN_AND_MEAN")
  add_definitions("-DNOMINMAX")
endif()

find_package(Boost 1.72.0 REQUIRED)
  include_directories(${Boost_INCLUDE_DIRS})
  add_definitions("-DHAS_BOOST")
message(STATUS "Boost_FOUND=${Boost_FOUND}")
message(STATUS "Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}")

find_package(Git REQUIRED)
include(ExternalProject)

message(STATUS "Configuring Celero")
ExternalProject_Add(celero_project
  GIT_REPOSITORY https://github.com/DigitalInBlue/Celero.git
  GIT_TAG "v2.6.0"
  GIT_SHALLOW 1
  UPDATE_COMMAND ""
  INSTALL_DIR ${CMAKE_BINARY_DIR}
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_BINARY_DIR}
    -DCELERO_COMPILE_DYNAMIC_LIBRARIES:BOOL=OFF
    -DCELERO_ENABLE_EXPERIMENTS:BOOL=OFF
    -DCELERO_ENABLE_FOLDERS:BOOL=OFF)
ExternalProject_Get_Property(celero_project SOURCE_DIR)
ExternalProject_Get_Property(celero_project INSTALL_DIR)
set(CELERO_INSTALL_DIR ${INSTALL_DIR})
message(STATUS "CELERO_INSTALL_DIR=${CELERO_INSTALL_DIR}")

include_directories(${CELERO_INSTALL_DIR}/include)
add_library(libcelero STATIC IMPORTED GLOBAL)
set_property(TARGET libcelero PROPERTY
  IMPORTED_LOCATION_DEBUG
  "${CELERO_INSTALL_DIR}/lib/static/${CMAKE_STATIC_LIBRARY_PREFIX}celerod${CMAKE_STATIC_LIBRARY_SUFFIX}")
set_property(TARGET libcelero PROPERTY
  IMPORTED_LOCATION_RELEASE
  "${CELERO_INSTALL_DIR}/lib/static/${CMAKE_STATIC_LIBRARY_PREFIX}celero${CMAKE_STATIC_LIBRARY_SUFFIX}")
add_dependencies(libcelero celero_project)

if(MSVC)
  set(CELERO_EXTRA_LIBS PowrProf.lib)
endif()

message(STATUS "Configuring fmtlib")
ExternalProject_Add(fmt
  PREFIX ${CMAKE_BINARY_DIR}/fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG "4.x"
  GIT_SHALLOW 1
  TIMEOUT 10
  UPDATE_COMMAND ${GIT_EXECUTABLE} pull
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON)
ExternalProject_Get_Property(fmt SOURCE_DIR)
set(FMT_INCLUDE_DIR ${SOURCE_DIR} CACHE INTERNAL "Path to include folder for Catch")
message(STATUS "FMT_INCLUDE_DIR=${FMT_INCLUDE_DIR}")
include_directories(${FMT_INCLUDE_DIR})
add_definitions("-DFMT_HEADER_ONLY")

message(STATUS "Configuring tests - ${BUILD_TESTING}")
if (BUILD_TESTING)
  message(STATUS "Configuring Catch")
  ExternalProject_Add(catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_SHALLOW 1
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON)
  ExternalProject_Get_Property(catch SOURCE_DIR)
  set(CATCH_INCLUDE_DIR ${SOURCE_DIR}/single_include CACHE INTERNAL "Path to include folder for Catch")
  message(STATUS "CATCH_INCLUDE_DIR=${CATCH_INCLUDE_DIR}")
  include_directories(${CATCH_INCLUDE_DIR})

  set(tests test_fixture)
  foreach(test ${tests})
    message(STATUS "Configuring test: ${test}")
    add_executable(${test} ${test}.cpp)
    add_dependencies(${test} catch fmt libcelero)
    add_test(run_${test} ${test})
  endforeach()
else()

endif()

file(GLOB headers *.hpp)
file(GLOB sources fixture.cpp)
set(benchmarks
  benchmark_stod
  benchmark_stoi
  benchmark_starts_with
  benchmark_ends_with
  benchmark_istarts_with
  benchmark_iends_with
  benchmark_escape_xml
  benchmark_fmtlib
  benchmark_boost_geometry_wkt)

message(STATUS "Configuring benchmarks run - ${ENABLE_BENCHMARK_RUN}")
if (ENABLE_BENCHMARK_RUN)
  set(run_all ALL)
else()
  set(run_all)
endif()

foreach(benchmark ${benchmarks})
  set(btarget ${benchmark}) # benchmark char-based strings
  message(STATUS "Configuring benchmark: ${btarget}")
  add_executable(${btarget} ${benchmark}.cpp ${sources} ${headers})
  add_dependencies(${btarget} fmt libcelero)
  target_link_libraries(${btarget} libcelero ${CELERO_EXTRA_LIBS})
  set(any_test_target)
  if (ENABLE_TESTS)
    set(any_test_target run_${btarget})
  endif()
  add_custom_target(run_${btarget} ${run_all}
    COMMAND ${btarget} ARGS -t ${btarget}.csv
    DEPENDS ${any_test_target} ${btarget})

  string(REPLACE "benchmark_" "benchmark_w" btarget ${benchmark}) # benchmark wchar_t-based strings
  message(STATUS "Configuring benchmark: ${btarget}")
  add_executable(${btarget} ${benchmark}.cpp ${sources} ${headers})
  set_property(TARGET ${btarget} PROPERTY COMPILE_DEFINITIONS STRING_BENCHMARK_WCHAR=1)
  add_dependencies(${btarget} fmt libcelero)
  target_link_libraries(${btarget} libcelero ${CELERO_EXTRA_LIBS})
  set(any_test_target)
  if (ENABLE_TESTS)
    set(any_test_target run_${btarget})
  endif()
  add_custom_target(run_${btarget} ${run_all}
    COMMAND ${btarget} ARGS -t ${btarget}.csv
    DEPENDS  ${any_test_target} ${btarget})
endforeach()
