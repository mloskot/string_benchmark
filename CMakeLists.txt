cmake_minimum_required(VERSION 2.8)
project(string_benchmark CXX)
include(ExternalProject)

find_package(Git REQUIRED)

message(STATUS "Configuring Catch")
set (CATCH_PREFIX ${CMAKE_BINARY_DIR}/catch)
ExternalProject_Add(
  catch
  PREFIX ${CATCH_PREFIX}
  GIT_REPOSITORY https://github.com/philsquared/Catch.git
  TIMEOUT 10
  UPDATE_COMMAND ${GIT_EXECUTABLE} pull
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND "")
ExternalProject_Get_Property(catch SOURCE_DIR)
set(CATCH_INCLUDE_DIR ${SOURCE_DIR}/include CACHE INTERNAL "Path to Catch include directory")
include_directories(${CATCH_INCLUDE_DIR})
message(STATUS "CATCH_INCLUDE_DIR=${CATCH_INCLUDE_DIR}")

message(STATUS "Configuring Celero")
set (CELERO_PREFIX ${CMAKE_BINARY_DIR}/celero)
set (CELERO_INSTALL_PREFIX ${CELERO_PREFIX}/install)
ExternalProject_Add(
    celero
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/DigitalInBlue/Celero.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${CELERO_INSTALL_PREFIX}
      -DCELERO_COMPILE_DYNAMIC_LIBRARIES:BOOL=OFF
      -DCELERO_ENABLE_EXPERIMENTS:BOOL=OFF
      -DCELERO_ENABLE_FOLDERS:BOOL=OFF)
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
  add_custom_target(run_${test} ALL $<TARGET_FILE:${test}> DEPENDS ${test})
endforeach()

# file(GLOB headers *.hpp)
# file(GLOB sources fixture.cpp)
# set(benchmarks benchmark_istarts_with)

# foreach(benchmark ${benchmarks})
#   message(STATUS "Configuring benchmark: ${benchmark}")
#   add_executable(${benchmark} ${benchmark}.cpp ${sources} ${headers})

#   if(MSVC)
#     target_link_libraries(${benchmark} debug ${CELERO_INSTALL_PREFIX}/lib/celerod.lib PowrProf.lib)
#     target_link_libraries(${benchmark} optimized ${CELERO_INSTALL_PREFIX}/lib/celero.lib PowrProf.lib)
#   elseif (UNIX)
#     target_link_libraries(${benchmark} ${CELERO_INSTALL_PREFIX}/lib/libcelero.lib)
#   endif()

#   add_dependencies(${benchmark} celero)
#   add_custom_target(run_${benchmark} ALL $<TARGET_FILE:${benchmark}> DEPENDS ${benchmark})
# endforeach()