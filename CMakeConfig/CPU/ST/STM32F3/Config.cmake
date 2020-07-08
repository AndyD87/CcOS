message("- CPU: STM32F2 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CMAKE_SYSTEM_PROCESSOR cortex-m4 )
set(CMAKE_C_FLAGS   "-mcpu=cortex-m4 -mthumb -Os --specs=nosys.specs -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra -g" )
set(CMAKE_CXX_FLAGS "-mcpu=cortex-m4 -mthumb -Os --specs=nosys.specs -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra -g -fno-exceptions -fno-rtti -fno-threadsafe-statics -fabi-version=0 -fno-rtti -fno-use-cxa-atexit"  )

set(CMAKE_EXE_LINKER_FLAGS    "-mcpu=cortex-m4 -mthumb --specs=nano.specs -T \"${CMAKE_CURRENT_LIST_DIR}/STM32F303VCTx_FLASH.ld\" -Xlinker --gc-sections -Wl,-Map=STM32F3.map" )

set(CC_EXECUTABLE_SUFFIX ".elf" )

add_definitions(-DARM_MATH_CM3-DUSE_FULL_ASSERT -DSTM32F207xx)


set(CCOS_DRIVER_CPU                   TRUE )
set(CCOS_DRIVER_CPU_ST                TRUE )
set(CCOS_DRIVER_CPU_ST_STM32F303VCT6  TRUE )

enable_language(CXX C ASM)