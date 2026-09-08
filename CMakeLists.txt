# This CMake config generates a Makefile with the following targets:
# 1. Standalone library for the Example class (see component CMakeLists.txt)
# 2. Driver binary that uses the standalone library generated in (1)
# 3. Google Test framework binary that unit tests the library generated in (1)
#
# Source file structure:
# -> Source code for the whole component goes under src/
# -> All header files associated with the library go in src/include
# -> All implementation files associated with the library go in src/<component_name>
#
# Machine code structure:
# -> All static and dynamic libraries associated with component go in build/lib
# -> All binaries go to build/bin
# Note "build" is an example here. This path is your $PWD when you execute cmake

# Minimum cmake version specification
cmake_minimum_required(VERSION 3.24.0)

# Include macros
include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(FetchContent)

# Set the project name and version
project(template VERSION 1.0.0 LANGUAGES C CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# All static and dynamic libraries go to build/lib
# All binaries go to build/bin
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# Set var with all uppercase version of the project name. Used in config header
string(TOUPPER ${CMAKE_PROJECT_NAME} CMAKE_PROJECT_NAME_UPPER)

# Generate cfgparser_config.h from cfgparser_config.h.in. This file will go to the binary
# directory and we should make add this to all target include directories later
configure_file(${PROJECT_SOURCE_DIR}/src/include/${CMAKE_PROJECT_NAME}_config.h.in ${CMAKE_PROJECT_NAME}_config.h)

# Set option to include unit test suite target. Default is ON
option(DO_UT "Perform unit test suite" ON)

# Set special flags for compiling each different kind of release
# Debug builds get test coverage flag
# Release w/ debug info just gets debug symbols
# Release gets high efficiency optimization
# MinSizeRel gets space optimization
set(CMAKE_C_FLAGS_DEBUG "-Wall -g -Og --coverage")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -Og --coverage")
set(CMAKE_C_FLAGS_RELEASE "-Wall -O2")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Wall -O2 -g")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -O2 -g")
set(CMAKE_C_FLAGS_MINSIZEREL "-Wall -Os")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Wall -Os")

# Add subdirectory. The cmake config file there will dictate how to make the component libraries
add_subdirectory(${PROJECT_SOURCE_DIR}/src/${CMAKE_PROJECT_NAME})

# Specify list of sources for the component's driver binary
set(${CMAKE_PROJECT_NAME}_bin_src
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

# Add the test and main executables
add_executable(${CMAKE_PROJECT_NAME}_bin ${${CMAKE_PROJECT_NAME}_bin_src})
IF(DO_UT)

    ## Uncomment this block to do a local install of Google Test libraries
    ## if you don't want to 'sudo apt-get -y install libgtest-dev' on your system
    ## Install the google test libraries, setting relevant CMake policy to use
    ## downloaded file metadata behavior for FetchContent
    #cmake_policy(SET CMP0135 NEW)

    #message(STATUS "Downloading googletest dependencies")
    #FetchContent_Declare(
    #    googletest
    #    #URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
    #    URL https://github.com/google/googletest/archive/b514bdc898e2951020cbdca1304b75f5950d1f59.zip
    #)
    #FetchContent_MakeAvailable(googletest)
    #message(STATUS "Downloading googletest dependencies -- done")

    # Assert that Google Test is installed
    # Comment this line if doing local install as described above
    find_package(GTest REQUIRED)

    # Assert that gcovr is installed, and get its path
    # The variable 'gcovr_EXECUTABLE' should now hold the path to gcovr
    find_program(gcovr_EXECUTABLE gcovr REQUIRED)

    # Define the sources and executable for the UT suite
    set(${CMAKE_PROJECT_NAME}_unittest_src
        ${PROJECT_SOURCE_DIR}/test/${CMAKE_PROJECT_NAME}_unittest.cpp
        )

    # Add the component library interfaces to the test suite binary include path
    # Also add the binary directory to the include search path (for generated ${CMAKE_PROJECT_NAME}_config.h)
    add_executable(${CMAKE_PROJECT_NAME}_unittest_bin ${${CMAKE_PROJECT_NAME}_unittest_src})
    target_include_directories(${CMAKE_PROJECT_NAME}_unittest_bin PUBLIC "${PROJECT_SOURCE_DIR}/src/include")
    target_include_directories(${CMAKE_PROJECT_NAME}_unittest_bin PUBLIC "${PROJECT_BINARY_DIR}") # Link the testing binary against both the component library and google test libraries
    target_link_libraries(${CMAKE_PROJECT_NAME}_unittest_bin PUBLIC ${CMAKE_PROJECT_NAME} gtest gtest_main pthread)

    # Add the custom target "check", which runs the UT suite. We do this so that
    # we can run the UT binary without needing to know the name of it. We also
    # add a dependency on the test binary to ensure it's compiled before executing
    add_custom_target(
        check
        COMMENT "Executing UT suite (in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})..."
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_PROJECT_NAME}_unittest_bin
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating UT coverage report..."
        COMMAND ${gcovr_EXECUTABLE} -r .. -f '../src'
    )

    # Set UT binary as a dependency for the 'check' target
    add_dependencies(check ${CMAKE_PROJECT_NAME}_unittest_bin)

ENDIF(DO_UT)

# Add the component include directory to the binary targets
# Also add the binary directory to the include search path (for generated ${CMAKE_PROJECT_NAME}_config.h)
target_include_directories(${CMAKE_PROJECT_NAME}_bin PUBLIC "${PROJECT_SOURCE_DIR}/src/include")
target_include_directories(${CMAKE_PROJECT_NAME}_bin PUBLIC "${PROJECT_BINARY_DIR}")

# Link the driver binary against the component library
target_link_libraries(${CMAKE_PROJECT_NAME}_bin PUBLIC ${CMAKE_PROJECT_NAME})
