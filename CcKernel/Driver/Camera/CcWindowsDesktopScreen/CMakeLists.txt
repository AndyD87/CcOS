########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.cpp"
      "src/*.c" )

########
# Add Source Files
########
file (GLOB HEADER_FILES
      "*h" )

add_to_driver_headers(Camera/CcWindowsDesktopScreen/CcWindowsDesktopScreenDriver.h)
add_to_driver_names  (CcWindowsDesktopScreenDriver 4)
add_to_driver_sources(${SOURCE_FILES} ${HEADER_FILES})
