cmake_minimum_required(VERSION 3.17)

project(AutoTrade LANGUAGES CXX)

message(STATUS "CMake ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" )

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(AutoTrade "")
target_sources(AutoTrade PRIVATE
  main.cpp
  mainwindow.hh
  mainwindow.cpp
  mainwindow.ui
)

add_subdirectory(libs)
add_subdirectory(craft)
add_subdirectory(network)
add_subdirectory(poe)
add_subdirectory(ui)

### Compilation options
# C++20
target_compile_features(AutoTrade PUBLIC cxx_std_20)
set_target_properties(AutoTrade PROPERTIES CXX_EXTENSIONS OFF)
if (MSVC)
    target_compile_options(AutoTrade PUBLIC /std:c++latest /Zc:__cplusplus)
endif()
# Enable Link Time Optimization if available
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
  set_target_properties(AutoTrade PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
  message(STATUS "Enabled Link Time optimization")
endif()
# Define NOMINMAX on MSVC for ranges-v3
if(MSVC)
  target_compile_definitions(AutoTrade PUBLIC NOMINMAX)
endif()

##### Pre build actions
# Copy resources to build folder
add_custom_command(TARGET AutoTrade PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:AutoTrade>/resources/)

### Libraries
# Qt
find_package(Qt5 COMPONENTS Core Widgets Network WebSockets REQUIRED)
target_link_libraries(AutoTrade PRIVATE Qt5::Widgets Qt5::Network Qt5::WebSockets)

# lua
include(FindLua)
find_package(Lua REQUIRED)
target_link_libraries(AutoTrade PRIVATE ${LUA_LIBRARIES})
target_include_directories(AutoTrade PRIVATE ${LUA_INCLUDE_DIR})

# sol2
find_package(sol2 CONFIG REQUIRED)
target_link_libraries(AutoTrade PRIVATE sol2::sol2)

# fmt
#find_package(fmt CONFIG REQUIRED)
#target_link_libraries(AutoTrade PRIVATE fmt::fmt fmt::fmt-header-only)

# spdlog with external fmt
#target_compile_definitions(AutoTrade PUBLIC SPDLOG_FMT_EXTERNAL)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(AutoTrade PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

# ranges-v3
find_package(range-v3 CONFIG REQUIRED)
target_link_libraries(AutoTrade PRIVATE range-v3 range-v3-meta range-v3::meta range-v3-concepts)

# magic_enum
find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(AutoTrade PRIVATE magic_enum::magic_enum)

# QCodeEditor
target_link_libraries(AutoTrade PRIVATE QCodeEditor)
