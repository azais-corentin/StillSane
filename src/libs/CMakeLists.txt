target_include_directories(StillSane PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Qt Advanced Docking System
option(BUILD_STATIC "" ON)
option(BUILD_EXAMPLES "" OFF)
add_subdirectory(QtAds)

# QCodeEditor
option(BUILD_EXAMPLE "" Off)
add_subdirectory(QCodeEditor)
