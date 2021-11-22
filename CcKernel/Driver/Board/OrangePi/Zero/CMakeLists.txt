########
# Add Source Files
########
set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/OrangePiZero.h
        ${CMAKE_CURRENT_LIST_DIR}/OrangePiZeroDriver.cpp
        ${CMAKE_CURRENT_LIST_DIR}/OrangePiZeroDriver.h
        ${CMAKE_CURRENT_LIST_DIR}/OrangePiZeroLed.cpp
        ${CMAKE_CURRENT_LIST_DIR}/OrangePiZeroLed.h
)

add_to_driver_headers("Board/OrangePi/Zero/OrangePiZeroDriver.h")
add_to_driver_names  (1 "OrangePiZeroDriver")
add_to_driver_sources(${SOURCE_FILES} ${STM_SOURCE_FILES})
add_to_driver_includes(${CMAKE_CURRENT_SOURCE_DIR})
add_to_driver_definitions()
