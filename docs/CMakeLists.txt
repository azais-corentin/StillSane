cmake_minimum_required(VERSION 3.17)

find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

option(BUILD_DOCUMENTATION "Build the documentation (requires Doxygen)" ${DOXYGEN_FOUND})


if(BUILD_DOCUMENTATION)
  ##### Doxygen
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif()

  set(DOXYGEN_INPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
  set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIRECTORY}/xml/index.xml)
  set(DOXYGEN_EXCLUDE_PATTERNS "*/libs/*")
  set(DOXYGEN_GENERATE_HTML NO)
  set(DOXYGEN_GENERATE_XML YES)

  doxygen_add_docs(
    Doxygen
    ${DOXYGEN_INPUT_DIRECTORY}
    COMMENT "Generating documentation (1/2)"
  )

  ##### Sphinx
  set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
  set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/html)
  set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

  if (UNIX)
    set(separator ":")
  else ()
    set(separator ";")
  endif()

  # Only regenerate Sphinx when:
  # - Doxygen has rerun
  # - Our doc files have been updated
  # - The Sphinx config has been updated
  add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
                     COMMAND ${Python3_EXECUTABLE} -m sphinx -b html
                       # Tell Breathe where to find the Doxygen output
                       -Dbreathe_projects.StillSane=${DOXYGEN_OUTPUT_DIRECTORY}/xml
                     ${SPHINX_SOURCE} ${SPHINX_BUILD}
                     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                     DEPENDS
                     # Other docs files you want to track should go here (or in some variable)
                     ${SPHINX_SOURCE}/index.rst
                     ${SPHINX_SOURCE}/getting-started.rst
                     ${SPHINX_SOURCE}/autocraft.rst
                     ${SPHINX_SOURCE}/autotrade.rst
                     MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
                     COMMENT "Generating documentation (2/2)")

  # Nice named target so we can run the job easily
  add_custom_target(Sphinx DEPENDS ${SPHINX_INDEX_FILE})
  add_dependencies(Sphinx Doxygen)

  # Add an install target to install the docs
  install(DIRECTORY ${SPHINX_BUILD} TYPE DOC)
endif()
