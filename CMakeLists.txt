cmake_minimum_required(VERSION 2.8)
project(string_benchmark CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
    message(STATUS "Configuring default CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
endif()

find_package(Git REQUIRED)

include(ExternalProject)
message(STATUS "Configuring Celero")
set (CELERO_PREFIX ${CMAKE_BINARY_DIR}/celero)
set (CELERO_INSTALL_PREFIX ${CELERO_PREFIX}/install)
ExternalProject_Add(
    celero
    PREFIX ${CELERO_PREFIX}
    GIT_REPOSITORY https://github.com/DigitalInBlue/Celero.git
    GIT_TAG master
    GIT_SHALLOW 1
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${CELERO_INSTALL_PREFIX}
      -DCELERO_COMPILE_DYNAMIC_LIBRARIES:BOOL=OFF
      -DCELERO_ENABLE_EXPERIMENTS:BOOL=OFF
      -DCELERO_ENABLE_FOLDERS:BOOL=OFF
)
find_package(Boost 1.54.0)
message(STATUS "CELERO_PREFIX=${CELERO_PREFIX}")
message(STATUS "CELERO_INSTALL_PREFIX=${CELERO_INSTALL_PREFIX}")

find_package(Boost 1.54.0)
message(STATUS "Boost_FOUND=${Boost_FOUND}")
message(STATUS "Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}")

add_subdirectory(src)

# collect_info_files function
# Takes collect_info_files - list of non-source files to look for
# Returns INFO_FILES with all files found from the input list.
# Based on macro posted here
# http://lists.qt-project.org/pipermail/qt-creator/2012-August/001191.html
function(collect_info_files)
    list(APPEND _all_found)
    foreach(_it ${ARGN})
        if(NOT IS_DIRECTORY ${_it})
            get_filename_component(_path ${_it} ABSOLUTE)
            if(EXISTS ${_path})
                list(APPEND _all_found ${_it})
                if(NOT ${_it} MATCHES "^/\\\\..*$;~$")
                    set_source_files_properties(${_it} PROPERTIES HEADER_FILE_ONLY TRUE)
                endif()
            endif()
        endif()
    endforeach()
    set(INFO_FILES ${_all_found} PARENT_SCOPE)
endfunction()

# List all non-source code files (documents, configuration files, etc.)
set(other_files .gitignore .travis.yml appveyor.yml README.md)
# Collect all existing files and configure as non-sources.
collect_info_files(${other_files})
# Create dummy target/project for an IDE with documents attached
add_custom_target(docs SOURCES ${INFO_FILES})