cmake_minimum_required(VERSION 3.15)

project(JPGtoSVG)

set(CMAKE_CXX_STANDARD 14)

# Get Submodules
find_package(Git QUIET)
# If the user is using git fetch the submodules
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Option to disable fetching submodules on build
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Getting submodules")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update failed with ${GIT_SUBMOD_RESULT}")
        endif()
    endif()
endif()

# Check if submodules exists
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/base64")
    message(FATAL_ERROR "Required submodule cpp-base64 was not found")
endif()

include_directories(base64)

add_executable(JPGtoSVG
        base64/base64.cpp
        base64/base64.h
        convert.cpp)
