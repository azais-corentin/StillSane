cmake_minimum_required(VERSION 3.17)

include(target_set_stillsane_options)

# Check Link Time Optimization support
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported LANGUAGES CXX)

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
  target_set_stillsane_options(${target})
  ##### Add common module library
  target_link_libraries(${target} PRIVATE StillSane::Module::Common)
  message(STATUS "Added module ${alias}")
endfunction()
