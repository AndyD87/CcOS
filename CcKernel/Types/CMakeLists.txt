########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.c"
      "*.cpp"
      "Hash/*.cpp"
      "*.h"
      "Hash/*.h"
)

add_to_kernel_sources(${SOURCE_FILES})
