message("- CPU: STM32F3 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT TRUE )

if("${CCOS_CPU_TYPE}" STREQUAL "STM32F303VCT6")
  set(CCOS_CPU_DRIVER                  STM32F303VCT6    )
  set(FLASH_FILE                       STM32F303VCTx_FLASH.ld)
else()
  set(CCOS_CPU_TYPE                    STM32F303VCT6    )
  set(CCOS_CPU_DRIVER                  STM32F303VCT6    )
  set(FLASH_FILE                       STM32F303VCTx_FLASH.ld)
  message("--- set default CPU: ${CCOS_CPU_TYPE}")
endif()

message("--- CPU selected: ${CCOS_CPU_TYPE}")

CcSetIfNotDefined( CCOS_GENERIC_USB        FALSE )
CcSetIfNotDefined( CCOS_GENERIC_NETWORK    FALSE )

set(CMAKE_SYSTEM_PROCESSOR         cortex-m4 )
set(MCPU_FLAGS "-mcpu=${CMAKE_SYSTEM_PROCESSOR} -mthumb")


set(CMAKE_EXE_LINKER_FLAGS  "${MCPU_FLAGS}     --specs=nano.specs -T \"${CMAKE_CURRENT_LIST_DIR}/${FLASH_FILE}\" -Xlinker --gc-sections" CACHE INTERNAL "")
set(CMAKE_C_FLAGS           "${MCPU_FLAGS}     --specs=nosys.specs -falign-functions=4 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -g -Wall -Wextra" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS         "${CMAKE_C_FLAGS}  -fno-exceptions -fno-rtti -fabi-version=0 -fno-use-cxa-atexit"  CACHE INTERNAL "")
CcSetAllAssemblerFlags(     ""   CACHE)
set(CMAKE_ASM_FLAGS         "${MCPU_FLAGS}" CACHE INTERNAL "")

set(CC_EXECUTABLE_SUFFIX ".elf" )

enable_language(CXX C ASM)

set(CCOS_NO_APPS TRUE  )