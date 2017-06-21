cmake_minimum_required (VERSION 2.8)
project (EntityBox)

set (EntityBox_VERSION_MAJOR 0)
set (EntityBox_VERSION_MINOR 1)


find_package(boost REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

enable_testing()
add_subdirectory(src bin)
add_subdirectory(test)
