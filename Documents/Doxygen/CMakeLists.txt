################################################################################
# Build doxygen documentation if doxygen is available
################################################################################
set(CURRENT_PROJECT RUN_DOXYGEN)

find_package(Doxygen)

if(Doxygen_FOUND)
  # Support cmake find doxygen too
  if(TARGET Doxygen::doxygen)
    get_target_property(Doxygen_EXECUTABLE Doxygen::doxygen IMPORTED_LOCATION)
    if(NOT EXISTS "${Doxygen_EXECUTABLE}")
        message(FATAL_ERROR "Invalid doxygen location ${Doxygen_EXECUTABLE}")
    endif()
  endif()
  
  configure_file( ${CMAKE_CURRENT_LIST_DIR}/config.in.txt
                  ${CMAKE_CURRENT_BINARY_DIR}/config.txt
                  @ONLY)

  add_custom_target(
    ${CURRENT_PROJECT}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/config.txt
    SOURCES ${CMAKE_CURRENT_LIST_DIR}/config.in.txt
    COMMAND echo Execute doxygen: ${Doxygen_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/config.txt
    COMMAND ${Doxygen_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/config.txt
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CMakePredefinedTargets")
endif()
