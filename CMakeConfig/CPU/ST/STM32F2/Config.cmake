message("- CPU: STM32F2 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT TRUE )

set(CMAKE_SYSTEM_PROCESSOR cortex-m3 )
set(CMAKE_C_FLAGS   "-mcpu=cortex-m3 --specs=nosys.specs -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra -g" )
set(CMAKE_CXX_FLAGS "-mcpu=cortex-m3 --specs=nosys.specs -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra -g -fno-exceptions -fno-rtti -fno-threadsafe-statics -fabi-version=0 -fno-rtti -fno-use-cxa-atexit"  )

set(CMAKE_EXE_LINKER_FLAGS    "-mcpu=cortex-m3 -mthumb --specs=nano.specs -T \"${CMAKE_CURRENT_LIST_DIR}/STM32F207IG.ld\" -Xlinker --gc-sections -Wl,-Map=STM32F2.map" )

set(CC_EXECUTABLE_SUFFIX ".elf" )
  
add_definitions(-DARM_MATH_CM3-DUSE_FULL_ASSERT -DSTM32F207xx)


set(CCOS_DRIVER_CPU                 TRUE )
set(CCOS_DRIVER_CPU_ST              TRUE )
set(CCOS_DRIVER_CPU_ST_STM32F207IG TRUE )

enable_language(CXX C ASM)