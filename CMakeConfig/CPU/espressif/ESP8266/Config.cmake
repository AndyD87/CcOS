#message("- CPU: ESP8266 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CMAKE_SYSTEM_PROCESSOR arm CACHE INTERNAL "")
set(CMAKE_C_FLAGS   "-DCCOS_GENERIC -mlongcalls -nostdlib -Wl,--start-group -lc -Wl,--end-group -lgcc -std=c11   -ffreestanding -Wall -Wextra -g" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "-DCCOS_GENERIC -mlongcalls -nostdlib -Wl,--start-group -lc -Wl,--end-group -lgcc -std=c++11 -ffreestanding -Wall -Wextra -g -fno-exceptions -fno-rtti -fabi-version=0 -fno-use-cxa-atexit"  CACHE INTERNAL "")

# Can be removed after gcc 5.2.0 support is removed (ref GCC_NOT_5_2_0)
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -Wl,--gc-sections" CACHE INTERNAL "")

set(CC_EXECUTABLE_SUFFIX ".elf" CACHE INTERNAL "")

set(CCOS_NO_APPS							              TRUE CACHE INTERNAL "")
set(CCOS_DRIVER_CPU    					            TRUE CACHE INTERNAL "")
set(CCOS_DRIVER_CPU_ESPRESSIF 				      TRUE CACHE INTERNAL "")
set(CCOS_DRIVER_CPU_ESPRESSIF_ESP8266       TRUE CACHE INTERNAL "")
set(CCOS_CCKERNEL_GENERIC_NO_SYSTEM_THREAD  TRUE CACHE INTERNAL "")
set(CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT  TRUE CACHE INTERNAL "")
set(CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT_NO_MALLOC  TRUE CACHE INTERNAL "")

list(APPEND CCOS_CMAKE_INCLUDES "${CMAKE_CURRENT_LIST_DIR}/Project" CACHE INTERNAL "")

set(CCOS_NO_PROJECT TRUE CACHE INTERNAL "")
