cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(NoSQLplusplus)

make_directory(../bin)
make_directory(../lib)

set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

# General Configuration: find all the required libraries.
list(APPEND CMAKE_MODULE_PATH "cmake")

include_directories(core)

# need to be changed to your g++ 10 directory
SET(CMAKE_C_COMPILER /usr/local/gcc-10/bin/gcc-10)
SET(CMAKE_CXX_COMPILER /usr/local/gcc-10/bin/g++-10)

#===================================================================
## Compiler
# set compiler flags for debug/release
if ( BUILD_DEBUG )
    add_definitions(-DDEBUG)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
else ()
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif ()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++20 -Wl,--no-as-needed")
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB CORE_FILES
    "core/*.cpp"
)

ADD_LIBRARY(nosql++ SHARED
	${CORE_FILES}
)


add_subdirectory(tests)