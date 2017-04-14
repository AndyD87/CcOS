set(USE_PLATFORM_STM32F4 TRUE)
set(TARGET_SYSTEM STM32F4Discovery )
#set(VFP_FLAGS "")
#set(LD_FLAGS "-nostartfiles")
set(TARGET_TOOLCHAIN "arm-none")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(CMAKE_EXE_LINKER_FLAGS "-T \"${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/STM32F4/mem.ld\" -T \"${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/STM32F4/libs.ld\" -T \"${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/STM32F4/sections.ld\" -nostartfiles -Xlinker --gc-sections -Wl,-Map,\"CcLib.map\" --specs=nano.specs")
set(CMAKE_EXECUTABLE_SUFFIX .elf)

set(CMAKE_SYSTEM_PROCESSOR cortex-m4)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics")
