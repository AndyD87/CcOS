message("- CPU: STM32F1 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT TRUE CACHE INTERNAL "")

#if("${CCOS_CPU_TYPE}" STREQUAL "STM32F103C8")
#else()
  #default use first ever used STM32F103C8
  set(CCOS_CPU_TYPE                    STM32F103C8 CACHE INTERNAL "")
  set(CCOS_CPU_DRIVER                  STM32F103   CACHE INTERNAL "")
  set(FLASH_FILE                       STM32F103C8.ld)
  message("--- set default CPU: ${CCOS_CPU_TYPE}")
#endif()

message("--- CPU selected: ${CCOS_CPU_TYPE}")

set(CMAKE_SYSTEM_PROCESSOR cortex-m3 CACHE INTERNAL "")
set(CMAKE_C_FLAGS   "-mcpu=cortex-m3 --specs=nosys.specs -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra -g" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "-mcpu=cortex-m3 --specs=nosys.specs -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra -g -fno-exceptions -fno-rtti -fno-threadsafe-statics -fabi-version=0 -fno-rtti -fno-use-cxa-atexit"  CACHE INTERNAL "")

set(CMAKE_EXE_LINKER_FLAGS    "-mcpu=cortex-m3 -mthumb --specs=nano.specs -T \"${CMAKE_CURRENT_LIST_DIR}/${FLASH_FILE}\" -Xlinker --gc-sections" CACHE INTERNAL "")

set(CC_EXECUTABLE_SUFFIX ".elf" CACHE INTERNAL "")
 
set(CCOS_NO_APPS TRUE  CACHE INTERNAL "")
add_definitions(-DARM_MATH_CM3-DUSE_FULL_ASSERT -DSTM32F103xx)
