set(CC_LINK_TYPE STATIC)
set(GENERIC TRUE)

set(CURRENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/CPU/ST/STM32F4")
message("- CPU: STM32F4")

set(CMAKE_SYSTEM_PROCESSOR cortex-m4 CACHE INTERNAL "")
set(CMAKE_C_FLAGS   "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra --specs=nosys.specs -g" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra --specs=nosys.specs -g -fabi-version=0 -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics"  CACHE INTERNAL "")

set(CMAKE_EXE_LINKER_FLAGS    "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -T \"${CURRENT_DIR}/mem.ld\" -T \"${CURRENT_DIR}/sections.ld\" -T \"${CURRENT_DIR}/libs.ld\" -Xlinker --gc-sections -Wl,-Map=${CURRENT_DIR}/STM32F4.map" CACHE INTERNAL "")

set(CCOS_EXECUTABLE_SUFFIX ".elf")
  
add_definitions(-DDEBUG -DUSE_FULL_ASSERT -DSTM32F40XX  -DHSE_VALUE=8000000)
