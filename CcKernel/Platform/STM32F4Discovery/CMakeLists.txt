cmake_minimum_required (VERSION 2.6)
########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "*.h")

set(EXT_INC_LIST "")
foreach(file_path ${EXT_HEADER_FILES})
  get_filename_component(dir_path ${file_path} PATH)
  list(APPEND EXT_INC_LIST ${dir_path})
endforeach()
list(REMOVE_DUPLICATES EXT_INC_LIST)

set(DEFINITIONS -DARM_MATH_CM4)

########
# Add subdirectories
########

########
# Add to kernel
########

add_to_kernel_definitions( ${DEFINITIONS} )
add_to_kernel_sources    ( ${SOURCE_FILES} ${EXT_HEADER_FILES})
add_to_kernel_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} )
