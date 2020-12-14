########
# Add Source Files
########
set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/CcKeyboardEventHandler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcKeyboardEventHandler.h
        ${CMAKE_CURRENT_LIST_DIR}/CcMouseEventHandler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcMouseEventHandler.h
)

add_to_kernel_sources(${SOURCE_FILES})
