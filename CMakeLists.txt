cmake_minimum_required(VERSION 3.17)

project(StillSane
  VERSION 0.2.0
  DESCRIPTION "Keep your sanity by automating some things in Path of Exile."
  HOMEPAGE_URL "https://github.com/Haellsigh/StillSane"
  LANGUAGES CXX)

message(STATUS "CMake ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" )

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(init_submodules)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(src)
add_subdirectory(docs)
