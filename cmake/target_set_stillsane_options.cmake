cmake_minimum_required(VERSION 3.17)

function(target_set_stillsane_options target)
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
    target_compile_options(${target} PRIVATE /W4)
  else()
    target_compile_options(${target} PRIVATE  -Wall -Wextra -Werror)
  endif()
endfunction()