message("- CPU: STM32F1 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT TRUE )

#if("${CCOS_CPU_TYPE}" STREQUAL "STM32F103C8")
#else()
  #default use first ever used STM32F103C8
  set(CCOS_CPU_TYPE                    STM32F103C8 )
  set(CCOS_CPU_DRIVER                  STM32F103   )
  set(FLASH_FILE                       STM32F103C8.ld)
  message("--- set default CPU: ${CCOS_CPU_TYPE}")
#endif()

message("--- CPU selected: ${CCOS_CPU_TYPE}")


set(CMAKE_SYSTEM_PROCESSOR cortex-m3 )
set(MCPU_FLAGS "-mcpu=${CMAKE_SYSTEM_PROCESSOR} -mthumb  -Wall")

set(CMAKE_C_FLAGS           "${MCPU_FLAGS} --specs=nosys.specs -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra -g" )
set(CMAKE_EXE_LINKER_FLAGS  "${MCPU_FLAGS} --specs=nosys.specs -lc -lm -lnosys -T \"${CMAKE_CURRENT_LIST_DIR}/${FLASH_FILE}\" -Xlinker --gc-sections" )
set(CMAKE_CXX_FLAGS         "${CMAKE_C_FLAGS} -fno-exceptions -fno-rtti -fno-threadsafe-statics -fabi-version=0 -fno-rtti -fno-use-cxa-atexit"  )
set(CMAKE_ASM_FLAGS         "${MCPU_FLAGS} -fdata-sections -ffunction-sections" )

set(CC_EXECUTABLE_SUFFIX ".elf" )
 
set(CCOS_NO_APPS TRUE  )
