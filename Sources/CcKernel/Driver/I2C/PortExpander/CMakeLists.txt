########
# Add Source Files
########
set( SOURCE_FILES
      ${CMAKE_CURRENT_LIST_DIR}/MCP23017.cpp
      ${CMAKE_CURRENT_LIST_DIR}/MCP23017.h
)

add_to_driver_sources(${SOURCE_FILES})
