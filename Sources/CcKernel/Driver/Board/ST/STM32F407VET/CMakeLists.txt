########
# Add Source Files
########
set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/STM32F407VETBoard.h
        ${CMAKE_CURRENT_LIST_DIR}/STM32F407VETBoardDriver.cpp
        ${CMAKE_CURRENT_LIST_DIR}/STM32F407VETBoardDriver.h
        ${CMAKE_CURRENT_LIST_DIR}/STM32F407VETBoardLed.cpp
        ${CMAKE_CURRENT_LIST_DIR}/STM32F407VETBoardLed.h
)

add_to_driver_headers("Board/ST/STM32F407VET/STM32F407VETBoardDriver.h")
add_to_driver_names  (1 "STM32F407VETBoardDriver")
add_to_driver_sources(${SOURCE_FILES} ${STM_SOURCE_FILES})
add_to_driver_includes(${STM32F4_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
add_to_driver_definitions()

push_up_driver_vars()
