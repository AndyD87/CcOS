message("- CPU: STM32F4 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT TRUE CACHE INTERNAL "")

if("${CCOS_CPU_TYPE}" STREQUAL "STM32F407VET")
  set(CCOS_CPU_DRIVER                  STM32F407    CACHE INTERNAL "")
  set(FLASH_FILE                       STM32F407VETx_FLASH.ld)
else()
  #default use first ever used STM32F4Discovery
  set(CCOS_CPU_TYPE                    STM32F407VGT CACHE INTERNAL "")
  set(CCOS_CPU_DRIVER                  STM32F407    CACHE INTERNAL "")
  set(FLASH_FILE                       STM32F407VGTx_FLASH.ld)
  message("--- set default CPU: ${CCOS_CPU_TYPE}")
endif()

message("--- CPU selected: ${CCOS_CPU_TYPE}")

# Override Assembler Compiler
#set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER} CACHE INTERNAL "")

set(CMAKE_SYSTEM_PROCESSOR         cortex-m4 CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS  "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16  -lc -lm -lnosys -T \"${CMAKE_CURRENT_LIST_DIR}/${FLASH_FILE}\" -Xlinker --gc-sections" CACHE INTERNAL "")
set(CMAKE_C_FLAGS           "-mcpu=cortex-m4 -specs=nosys.specs -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -falign-functions=4 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra -g" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS         "${CMAKE_C_FLAGS} -fno-exceptions -fno-rtti -fabi-version=0 -fno-use-cxa-atexit"  CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS         "-x assembler-with-cpp -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wall -fdata-sections -ffunction-sections" CACHE INTERNAL "")

set(CC_EXECUTABLE_SUFFIX ".elf" CACHE INTERNAL "")

