message("- CPU: STM32F4 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CMAKE_SYSTEM_PROCESSOR cortex-m4 CACHE INTERNAL "")
set(CMAKE_C_FLAGS   "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra --specs=nosys.specs -g" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra --specs=nosys.specs -g -fabi-version=0 -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics"  CACHE INTERNAL "")

set(CMAKE_EXE_LINKER_FLAGS    "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -T \"${CMAKE_CURRENT_LIST_DIR}/mem.ld\" -T \"${CMAKE_CURRENT_LIST_DIR}/sections.ld\" -T \"${CMAKE_CURRENT_LIST_DIR}/libs.ld\" -Xlinker --gc-sections -Wl,-Map=${CMAKE_CURRENT_LIST_DIR}/STM32F4.map" CACHE INTERNAL "")

set(CC_EXECUTABLE_SUFFIX ".elf")
  
add_definitions(-DDEBUG -DUSE_FULL_ASSERT -DSTM32F40XX  -DHSE_VALUE=8000000)
