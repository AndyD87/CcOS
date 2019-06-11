########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.cpp" 
      "*.h" 
      "Simulations/*.cpp" 
      "Simulations/*.h" )

add_to_kernel_sources(${SOURCE_FILES})
