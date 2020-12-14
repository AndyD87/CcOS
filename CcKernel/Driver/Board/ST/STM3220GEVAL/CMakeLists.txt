########
# Add Source Files
########
set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/STM3220GEVAL.h
        ${CMAKE_CURRENT_LIST_DIR}/STM3220GEVALDriver.cpp
        ${CMAKE_CURRENT_LIST_DIR}/STM3220GEVALDriver.h
        ${CMAKE_CURRENT_LIST_DIR}/STM3220GEVALLed.cpp
        ${CMAKE_CURRENT_LIST_DIR}/STM3220GEVALLed.h
)

add_to_driver_headers("Board/ST/STM3220GEVAL/STM3220GEVALDriver.h")
add_to_driver_names  (1 "STM3220GEVALDriver")
add_to_driver_sources(${SOURCE_FILES} ${STM_SOURCE_FILES})
add_to_driver_includes(${STM32F4_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
add_to_driver_definitions()
