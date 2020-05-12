target_include_directories(AutoTrade PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

option(BUILD_STATIC "" ON)
option(BUILD_EXAMPLES "" OFF)
add_subdirectory(QtAds)
target_link_libraries(AutoTrade PRIVATE qtadvanceddocking)
target_include_directories(AutoTrade PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/QtAds/src")
