cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(TrigoTrain)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)

find_package(Shark REQUIRED)
include(${SHARK_USE_FILE}) #compile with "-DShark_DIR=/home/quin/Shark/newbuild"

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#enable_testing ()
#add_subdirectory (tests)
add_subdirectory (src) 
add_subdirectory (lib) 


