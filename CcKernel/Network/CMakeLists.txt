########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.cpp"
      "*.h"
      "Server/*.cpp"
      "Server/*.h")

add_to_kernel_sources(${SOURCE_FILES})