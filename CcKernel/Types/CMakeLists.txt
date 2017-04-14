########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "Hash/src/*.cpp"
      "*.h"
      "Hash/*.h" )

add_to_kernel_sources(${SOURCE_FILES})
