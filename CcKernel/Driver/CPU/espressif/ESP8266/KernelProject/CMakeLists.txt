########
# ESP8266 does not use main() as project.
# This is the reason why we use this project to build a simple blinky with CcKernel as lib
# to test if all is working correctly.
########
set(CURRENT_PROJECT ESP8266_KernelProject)

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.c"
      "*.cpp"
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR})

  add_executable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

  CcElfToImage(${CURRENT_PROJECT})

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC CcKernel )

# To keep Header an Source together, display them in same Directory
# It will only work with IDE's wich are supporting this features                              
source_group( ""        FILES ${SOURCE_FILES})
                            
set_target_properties(${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/CcApps")
