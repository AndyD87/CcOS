message("- CPU: STM32F4 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CMAKE_SYSTEM_PROCESSOR cortex-m4 CACHE INTERNAL "")
set(CMAKE_C_FLAGS   "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fno-threadsafe-statics -fdata-sections -ffreestanding -Wall -Wextra --specs=nosys.specs -g" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fno-threadsafe-statics -fdata-sections -ffreestanding -Wall -Wextra --specs=nosys.specs -g -fabi-version=0 -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics"  CACHE INTERNAL "")

set(CMAKE_EXE_LINKER_FLAGS    "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -T \"${CMAKE_CURRENT_LIST_DIR}/mem.ld\" -T \"${CMAKE_CURRENT_LIST_DIR}/sections.ld\" -T \"${CMAKE_CURRENT_LIST_DIR}/libs.ld\" -Xlinker --gc-sections -Wl,-Map=STM32F4.map" CACHE INTERNAL "")

set(CC_EXECUTABLE_SUFFIX ".elf" CACHE INTERNAL "")
  
add_definitions(-DUSE_FULL_ASSERT -DSTM32F40XX)

set(CCOS_DRIVER_CPU    					  TRUE CACHE INTERNAL "")
set(CCOS_DRIVER_CPU_ST 					  TRUE CACHE INTERNAL "")
set(CCOS_DRIVER_CPU_ST_STM32F407V TRUE CACHE INTERNAL "")