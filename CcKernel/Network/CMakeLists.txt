########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.cpp"
      "*.h"
      "Protocols/Dhcp/*.cpp"
      "Protocols/Dhcp/*.h"
      "Server/*.cpp"
      "Server/*.h")

add_to_kernel_sources(${SOURCE_FILES})