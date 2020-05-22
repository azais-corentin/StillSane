find_package(Doxygen REQUIRED)

option(BUILD_DOCUMENTATION "Build the documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif()

  set(DOXYGEN_INPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
  set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIRECTORY}/xml/index.xml)
  set(DOXYGEN_EXCLUDE_PATTERNS "*/libs/*")
  set(DOXYGEN_GENERATE_HTML NO)
  set(DOXYGEN_GENERATE_XML YES)

  message("Building documentation...")

  doxygen_add_docs(
    doxygen
    ${DOXYGEN_INPUT_DIRECTORY}
    COMMENT "Generate docs"
  )
endif()
