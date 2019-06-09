########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.cpp"
      "*.h" )

if(NOT MEMORYMONITOR_ENABLED)
    list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcRestApiMemoryMonitor.cpp")
    list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcRestApiMemoryMonitor.h")
endif()

add_to_http_sources(${SOURCE_FILES})
