message("- CPU: STM32F4 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT TRUE CACHE INTERNAL "")

set(CCOS_DRIVER_CPU                  TRUE CACHE INTERNAL "")
set(CCOS_DRIVER_CPU_ST               TRUE CACHE INTERNAL "")

if("${CCOS_CPU_TYPE}" STREQUAL "STM32F407VET")
  set(CCOS_DRIVER_CPU_ST_STM32F407     TRUE CACHE INTERNAL "")
  set(FLASH_FILE                       STM32F407VET_FLASH.ld)
else()
  #default use first ever used STM
  message("--- CPU selected: STM32F407GTx")
  set(CCOS_DRIVER_CPU_ST_STM32F407     TRUE CACHE INTERNAL "")
  set(FLASH_FILE                       STM32F407GTx_FLASH.ld)
endif()

set(CMAKE_SYSTEM_PROCESSOR cortex-m4 CACHE INTERNAL "")
set(CMAKE_C_FLAGS   "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os --specs=nosys.specs -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra -g" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os --specs=nosys.specs -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra -g -fno-exceptions -fno-rtti -fabi-version=0 -fno-use-cxa-atexit"  CACHE INTERNAL "")

set(CMAKE_EXE_LINKER_FLAGS    "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=nano.specs -T \"${CMAKE_CURRENT_LIST_DIR}/${FLASH_FILE}\" -Xlinker --gc-sections -Wl,-Map=STM32F4.map" CACHE INTERNAL "")

set(CC_EXECUTABLE_SUFFIX ".elf" CACHE INTERNAL "")

add_definitions(-DARM_MATH_CM4 -DUSE_FULL_ASSERT -DSTM32F407xx)
