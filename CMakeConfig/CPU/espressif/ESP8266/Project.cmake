#message("- CPU: ESP8266 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CMAKE_SYSTEM_PROCESSOR arm CACHE INTERNAL "")
set(CMAKE_C_FLAGS   "-mlongcalls -nostdlib -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -lc -Wl,--end-group -lgcc -std=c11" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "-mlongcalls -nostdlib -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -lc -Wl,--end-group -lgcc -std=c++11 -fno-exceptions -fno-rtti"  CACHE INTERNAL "")

set(CMAKE_EXE_LINKER_FLAGS    "-Teagle.app.v6.ld" CACHE INTERNAL "")

set(CC_EXECUTABLE_SUFFIX ".elf" CACHE INTERNAL "")

set(CCOS_NO_APPS							              TRUE CACHE INTERNAL "")
set(CCOS_DRIVER_CPU    					            TRUE CACHE INTERNAL "")
set(CCOS_DRIVER_CPU_ESPRESSIF 				      TRUE CACHE INTERNAL "")
set(CCOS_DRIVER_CPU_ESPRESSIF_ESP8266       TRUE CACHE INTERNAL "")
set(CCOS_CCKERNEL_GENERIC_NO_SYSTEM_THREAD  TRUE CACHE INTERNAL "")
set(CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT  TRUE CACHE INTERNAL "")
set(CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT_NO_MALLOC  TRUE CACHE INTERNAL "")
