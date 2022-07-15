message("- CPU: STM32F1 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CCOS_CCKERNEL_GENERIC_PRINTF           TRUE )
set(CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT TRUE )

if("${CCOS_CPU_TYPE}" STREQUAL "STM32F103C8")
  set(CCOS_CPU_DRIVER                  STM32F103   )
  set(FLASH_FILE                       STM32F103C8.ld)
else()
  set(CCOS_CPU_TYPE                    STM32F103C8 )
  set(CCOS_CPU_DRIVER                  STM32F103   )
  set(FLASH_FILE                       STM32F103C8.ld)
  message("--- set default CPU: ${CCOS_CPU_TYPE}")
endif()

message("--- CPU selected: ${CCOS_CPU_TYPE}")

CcSetIfNotDefined( CCOS_GENERIC_USB        TRUE )
CcSetIfNotDefined( CCOS_GENERIC_NETWORK    FALSE )

set(CMAKE_SYSTEM_PROCESSOR cortex-m3 )
set(MCPU_FLAGS "-mcpu=${CMAKE_SYSTEM_PROCESSOR} -mthumb")

set(CMAKE_EXE_LINKER_FLAGS  "${MCPU_FLAGS}     --specs=nano.specs -T \"${CMAKE_CURRENT_LIST_DIR}/${FLASH_FILE}\" -Xlinker --gc-sections -Wl,--print-memory-usage")
set(CMAKE_C_FLAGS           "${MCPU_FLAGS}     --specs=nosys.specs -falign-functions=4 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra")
set(CMAKE_CXX_FLAGS         "${CMAKE_C_FLAGS}  -fno-exceptions -fno-rtti -fabi-version=0 -fno-use-cxa-atexit")
CcSetAllAssemblerFlags(     "")
set(CMAKE_ASM_FLAGS         "${MCPU_FLAGS}")

set(CC_EXECUTABLE_SUFFIX ".elf" )

enable_language(CXX C ASM)

set(CCOS_NO_APPS TRUE  )
