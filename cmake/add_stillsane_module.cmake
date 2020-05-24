cmake_minimum_required(VERSION 3.17)

# Check Link Time Optimization support
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)

##### Creates a library from a module target name
function(add_stillsane_module target)
  ##### Generate target name and alias
  set(alias "StillSane::Module::${target}")
  ##### Create static library and alias
  add_library(${target} STATIC "")
  add_library(${alias} ALIAS ${target})
  ##### Setup include directories
  target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  ##### Compilation options
  # C++20
  target_compile_features(${target} PRIVATE cxx_std_20)
  set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
  if (MSVC)
    target_compile_options(${target} PRIVATE /std:c++latest /Zc:__cplusplus)
  endif()
  # Enable Link Time Optimization if available
  if(ipo_supported)
    set_target_properties(${target} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
  endif()
  # Warnings
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /WX)
  else()
    target_compile_options(${target} PRIVATE  -Wall -Wextra -Werror)
  endif()
  message(STATUS "Added module ${alias}")
endfunction()
