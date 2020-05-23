include(CMakeChecks.txt)

add_executable(StillSane "")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
target_include_directories(StillSane PRIVATE CMAKE_SOURCE_DIR)

target_sources(StillSane PRIVATE
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

##### Compilation options
# C++20
target_compile_features(StillSane PRIVATE cxx_std_20)
set_target_properties(StillSane PROPERTIES CXX_EXTENSIONS OFF)
if (MSVC)
    target_compile_options(StillSane PRIVATE /std:c++latest /Zc:__cplusplus)
endif()
# Enable Link Time Optimization if available
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
  set_target_properties(StillSane PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
  message(STATUS "Enabled Link Time optimization")
endif()
# Warnings
if(MSVC)
  target_compile_options(StillSane PRIVATE /W4 /WX /Wv:18 /wd4127)
else()
  target_compile_options(StillSane PRIVATE  -Wall -Wextra -Werror)
endif()

##### Pre build actions
# Copy resources to build folder
add_custom_command(TARGET StillSane PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${PROJECT_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:StillSane>/resources/)

##### Libraries
# Qt
find_package(Qt5 COMPONENTS Core Widgets Network WebSockets REQUIRED)
target_link_libraries(StillSane PRIVATE Qt5::Widgets Qt5::Network Qt5::WebSockets)

# lua
include(FindLua)
find_package(Lua REQUIRED)
target_link_libraries(StillSane PRIVATE ${LUA_LIBRARIES})
target_include_directories(StillSane PRIVATE ${LUA_INCLUDE_DIR})

# sol2
find_package(sol2 CONFIG REQUIRED)
target_link_libraries(StillSane PRIVATE sol2::sol2)

# fmt
#find_package(fmt CONFIG REQUIRED)
#target_link_libraries(StillSane PRIVATE fmt::fmt fmt::fmt-header-only)

# spdlog with external fmt
#target_compile_definitions(StillSane PUBLIC SPDLOG_FMT_EXTERNAL)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(StillSane PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

# ranges-v3
find_package(range-v3 CONFIG REQUIRED)
target_link_libraries(StillSane PRIVATE range-v3 range-v3-meta range-v3::meta range-v3-concepts)
if(MSVC)
  # Define NOMINMAX on MSVC for ranges-v3
  target_compile_definitions(StillSane PUBLIC NOMINMAX)
endif()

# magic_enum
find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(StillSane PRIVATE magic_enum::magic_enum)

# QCodeEditor
target_link_libraries(StillSane PRIVATE QCodeEditor)

# Qt Advanced Docking System
target_link_libraries(StillSane PRIVATE qtadvanceddocking)
target_include_directories(StillSane PRIVATE "${PROJECT_SOURCE_DIR}/libs/QtAds/src")
