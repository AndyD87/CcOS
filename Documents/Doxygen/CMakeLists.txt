################################################################################
# Build doxygen documentation if doxygen is available
################################################################################
set(CURRENT_PROJECT RUN_DOXYGEN)

find_package(Doxygen)

if(Doxygen_FOUND)
  configure_file( ${CMAKE_CURRENT_LIST_DIR}/config.in.txt
                  ${CMAKE_CURRENT_BINARY_DIR}/config.txt
                  @ONLY)

  add_custom_target(
    ${CURRENT_PROJECT}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/config.txt
    SOURCES ${CMAKE_CURRENT_LIST_DIR}/config.in.txt
    COMMAND ${Doxygen_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/config.txt
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CMakePredefinedTargets")
endif()
