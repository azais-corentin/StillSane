cmake_minimum_required(VERSION 3.17)

project(StillSane LANGUAGES CXX)

message(STATUS "CMake ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" )

add_executable(StillSane "")

add_subdirectory(src)
add_subdirectory(docs)

##### Pre build actions
# Copy resources to build folder
add_custom_command(TARGET StillSane PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:StillSane>/resources/)