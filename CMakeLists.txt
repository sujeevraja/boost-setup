cmake_minimum_required(VERSION 3.12)

project("boost-setup-test")

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

option(debug "Turn on debug mode" OFF)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

set(CMAKE_BUILD_TYPE "Release")
if (debug)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# generate compile_commands.json for use by IntelliSense.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CPLEX flags
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DIL_STD")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wignored-attributes -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DIL_STD")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -lpthread -pthread -Wl,--no-as-needed -ldl ")
endif()

# Boost filesystem
# To understand how Boost is added, check the comments in the beginning of
# FindBoost.cmake. This file is available at
#
# ~/bin/cmake-3.14.3/share/cmake-3.14/Modules/FindBoost.cmake.
#
# Also note that boost needs to be built using the instructions in the boost
# website, i.e., with `bootstrap.sh` and `./b2 install` scripts in the boost
# folder. The folder listed as BOOST_ROOT should contain an "include" folder
# with headers and a "lib" folder with built boost libraries.
set(BOOST_ROOT "$ENV{HOME}/bin/boost/binaries")
find_package(Boost 1.72 REQUIRED COMPONENTS filesystem)
# set(LIBS ${LIBS} ${Boost_LIBRARIES})
set(LIBS ${LIBS} Boost::filesystem)

# Source files
add_subdirectory(src)

