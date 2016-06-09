cmake_minimum_required(VERSION 2.8)
project(string-benchmark)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
    message(STATUS "Configuring default CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
endif()

find_package(Git REQUIRED)

include(ExternalProject)
set(HAYAI_DIR ${CMAKE_BINARY_DIR}/hayai)
message(STATUS "Configuring Hayai")
ExternalProject_Add(
    hayai
    PREFIX ${CMAKE_BINARY_DIR}/hayai
    GIT_REPOSITORY https://github.com/nickbruun/hayai.git
    GIT_TAG master
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON)
ExternalProject_Get_Property(hayai SOURCE_DIR)
set(HAYAI_INCLUDE_DIR ${SOURCE_DIR}/src CACHE INTERNAL "Path to Hayai headers")
message(STATUS "HAYAI_DIR=${HAYAI_DIR}")
message(STATUS "HAYAI_INCLUDE_DIR=${HAYAI_INCLUDE_DIR}")

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