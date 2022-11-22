########
# Add Source Files
########
set(SOURCE_FILES
      ${CMAKE_CURRENT_LIST_DIR}/SDD1306.cpp
      ${CMAKE_CURRENT_LIST_DIR}/SDD1306.h
      ${CMAKE_CURRENT_LIST_DIR}/SH1106.cpp
      ${CMAKE_CURRENT_LIST_DIR}/SH1106.h
)

add_to_driver_sources(${SOURCE_FILES})
