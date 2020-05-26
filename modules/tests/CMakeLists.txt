##### Test executable
add_executable(StillSane_Tests "")
add_executable(StillSane::Tests ALIAS StillSane_Tests)

##### Compilation options
# C++20
target_compile_features(StillSane_Tests PRIVATE cxx_std_20)
set_target_properties(StillSane_Tests PROPERTIES CXX_EXTENSIONS OFF)
if (MSVC)
  target_compile_options(StillSane_Tests PRIVATE /std:c++latest /Zc:__cplusplus)
endif()
# Enable Link Time Optimization if available
if(ipo_supported)
  set_target_properties(StillSane_Tests PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
# Warnings
if(MSVC)
  target_compile_options(StillSane_Tests PRIVATE /W4 /WX)
else()
  target_compile_options(StillSane_Tests PRIVATE  -Wall -Wextra -Werror)
endif()

##### Add sources
target_sources(StillSane_Tests PRIVATE
  tests.cpp
  mouse.cpp
  interface.cpp
)

##### Link modules
target_link_libraries(StillSane_Tests PRIVATE StillSane::Module::Interface)
target_link_libraries(StillSane_Tests PRIVATE StillSane::Module::Mouse)

##### Link Catch2
find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(StillSane_Tests PRIVATE Catch2::Catch2)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(StillSane_Tests PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
