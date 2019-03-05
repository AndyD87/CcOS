cmake_minimum_required (VERSION 2.6)
########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.c"
      "*.cpp"
      "*.h")

set(EXT_INC_LIST "")
foreach(file_path ${EXT_HEADER_FILES})
  get_filename_component(dir_path ${file_path} PATH)
  list(APPEND EXT_INC_LIST ${dir_path})
endforeach()
list(REMOVE_DUPLICATES EXT_INC_LIST)

set(FREE_RTOS_DIR ${CC_CACHE_DIR}/ThirdParty/FreeRTOS)

  CcDownloadAndExtract( "FreeRTOS" 
                        "${FREE_RTOS_DIR}"
                        "http://coolcow.de/projects/ThirdParty/FreeRTOS/10.2.0.0/FreeRTOS.7z")

########
# Add subdirectories
########

########
# Add to kernel
########

add_to_kernel_definitions( ${DEFINITIONS} )
add_to_kernel_sources    (  ${EXT_HEADER_FILES} 
                            ${FREE_RTOS_DIR}/FreeRTOSv10.2.0/FreeRTOS/Source/tasks.c
                            ${FREE_RTOS_DIR}/FreeRTOSv10.2.0/FreeRTOS/Source/list.c
                            ${FREE_RTOS_DIR}/FreeRTOSv10.2.0/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
                            ${SOURCE_FILES})
add_to_kernel_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} 
                           ${FREE_RTOS_DIR}/FreeRTOSv10.2.0/FreeRTOS/Source/include 
                           ${FREE_RTOS_DIR}/FreeRTOSv10.2.0/FreeRTOS/Source/portable/GCC/ARM_CM4F)
