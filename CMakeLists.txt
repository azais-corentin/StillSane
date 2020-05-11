cmake_minimum_required(VERSION 3.16)

project(AutoTrade LANGUAGES CXX)

add_executable(AutoTrade "")

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
target_include_directories(AutoTrade PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

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
    add_compile_options(/std:c++latest /Zc:__cplusplus)
endif()
# Enable Link Time Optimization if available
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
  set_target_properties(AutoTrade PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
  message("Enabled Link Time optimization")
endif()

### Libraries
# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Core Widgets Network WebSockets REQUIRED)
target_link_libraries(AutoTrade PRIVATE Qt5::Core Qt5::Widgets Qt5::Network Qt5::WebSockets)
message("Using Qt version ${Qt5_VERSION}")

# lua
include(FindLua)
find_package(lua REQUIRED)
target_link_libraries(AutoTrade PRIVATE ${LUA_LIBRARIES})
target_include_directories(AutoTrade PRIVATE ${LUA_INCLUDE_DIR})

# sol2
find_package(sol2 CONFIG REQUIRED)
target_link_libraries(AutoTrade PRIVATE sol2::sol2)

# fmt
find_package(fmt CONFIG REQUIRED)
target_link_libraries(AutoTrade PRIVATE fmt::fmt fmt::fmt-header-only)

# spdlog with external fmt
target_compile_definitions(AutoTrade PRIVATE SPDLOG_FMT_EXTERNAL)
find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(AutoTrade PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
