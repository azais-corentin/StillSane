cmake_minimum_required(VERSION 3.17)

find_package (Python3 COMPONENTS Interpreter)

execute_process(COMMAND ${Python3_EXECUTABLE} -c "import sphinx;import breathe"  RESULT_VARIABLE sphinx_ret)

if (sphinx_ret EQUAL "0")
  set(Sphinx_FOUND 1)
  message(STATUS "Found python modules sphinx and breathe")
else()
  set(Sphinx_FOUND 0)
  message(FATAL_ERROR "Unable to import python modules sphinx and breathe")
endif()
