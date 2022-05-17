########
# Add Source Files
########
set( SOURCE_FILES
      ${CMAKE_CURRENT_LIST_DIR}/TLC5940.cpp
      ${CMAKE_CURRENT_LIST_DIR}/TLC5940.h
      ${CMAKE_CURRENT_LIST_DIR}/TLC5947.cpp
      ${CMAKE_CURRENT_LIST_DIR}/TLC5947.h
      ${CMAKE_CURRENT_LIST_DIR}/TLC594xDotCorrection.cpp
      ${CMAKE_CURRENT_LIST_DIR}/TLC594xDotCorrection.h
)

add_to_driver_sources(${SOURCE_FILES})
