cmake_minimum_required(VERSION 3.17)

project(StillSane
  VERSION 0.3.0
  DESCRIPTION "Keep your sanity by automating some things in Path of Exile."
  HOMEPAGE_URL "https://github.com/Haellsigh/StillSane"
  LANGUAGES CXX)

message(STATUS "CMake ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" )

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(init_submodules)

add_subdirectory(docs)
add_subdirectory(modules)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(src)
