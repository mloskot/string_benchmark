cmake_minimum_required(VERSION 2.8)
project(string_benchmark CXX)
include(ExternalProject)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
    message(STATUS "Configuring default CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX14)
        message(STATUS "C++ Standard: C++14")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    elseif(COMPILER_SUPPORTS_CXX11)
        message(STATUS "C++ Standard: C++11")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif(COMPILER_SUPPORTS_CXX0X)
        message(STATUS "C++ Standard: C++0x")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
        message(WARNING "Compiler ${CMAKE_CXX_COMPILER} has no C++ 0x/11/14 support.")
    endif()
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

find_package(Git REQUIRED)

message(STATUS "Configuring Catch")
ExternalProject_Add(
  catch
  PREFIX ${CMAKE_BINARY_DIR}/catch
  GIT_REPOSITORY https://github.com/philsquared/Catch.git
  TIMEOUT 10
  UPDATE_COMMAND ${GIT_EXECUTABLE} pull
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON)
ExternalProject_Get_Property(catch SOURCE_DIR)
set(CATCH_INCLUDE_DIR ${SOURCE_DIR}/include CACHE INTERNAL "Path to Catch include directory")
include_directories(${CATCH_INCLUDE_DIR})
message(STATUS "CATCH_INCLUDE_DIR=${CATCH_INCLUDE_DIR}")

message(STATUS "Configuring Celero")
set (CELERO_PREFIX ${CMAKE_BINARY_DIR}/celero)
set (CELERO_INSTALL_PREFIX ${CELERO_PREFIX}/install)
ExternalProject_Add(
    celero
    PREFIX ${CELERO_PREFIX}
    GIT_REPOSITORY https://github.com/DigitalInBlue/Celero.git
    GIT_TAG master
    GIT_SHALLOW 1
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    BINARY_DIR ${CELERO_PREFIX}/build
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${CELERO_INSTALL_PREFIX}
      -DCELERO_COMPILE_DYNAMIC_LIBRARIES:BOOL=OFF
      -DCELERO_ENABLE_EXPERIMENTS:BOOL=OFF
      -DCELERO_ENABLE_FOLDERS:BOOL=OFF
    BUILD_COMMAND "${CMAKE_COMMAND}" --build ${CELERO_PREFIX}/build --config Release
)
message(STATUS "CELERO_PREFIX=${CELERO_PREFIX}")
message(STATUS "CELERO_INSTALL_PREFIX=${CELERO_INSTALL_PREFIX}")
include_directories(${CELERO_INSTALL_PREFIX}/include)

find_package(Boost 1.54.0)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_definitions("-DHAS_BOOST")
endif()
message(STATUS "Boost_FOUND=${Boost_FOUND}")
message(STATUS "Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}")

enable_testing()
set(tests test_fixture)
foreach(test ${tests})
  message(STATUS "Configuring test: ${test}")
  add_executable(${test} ${test}.cpp)
  add_dependencies(${test} catch)
  add_test(NAME ${test} COMMAND ${test})
endforeach()

file(GLOB headers *.hpp)
file(GLOB sources fixture.cpp)
set(benchmarks benchmark_istarts_with)

foreach(benchmark ${benchmarks})
  message(STATUS "Configuring benchmark: ${benchmark}")
  add_executable(${benchmark} ${benchmark}.cpp ${sources} ${headers})

  if(MSVC)
    target_link_libraries(${benchmark} debug ${CELERO_INSTALL_PREFIX}/lib/celerod.lib PowrProf.lib)
    target_link_libraries(${benchmark} optimized ${CELERO_INSTALL_PREFIX}/lib/celero.lib PowrProf.lib)
  elseif (UNIX)
    target_link_libraries(${benchmark} ${CELERO_INSTALL_PREFIX}/lib/libcelero.lib)
  endif()

  add_dependencies(${benchmark} celero)
  add_custom_target(run_${benchmark} ALL $<TARGET_FILE:${benchmark}> DEPENDS ${benchmark})
endforeach()
