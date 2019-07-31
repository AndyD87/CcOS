#message("- CPU: STM32F4 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CCOS_MEMORY_MANAGMENT TRUE CACHE INTERNAL "")

set(CMAKE_SYSTEM_PROCESSOR arm CACHE INTERNAL "")
set(CMAKE_C_FLAGS   " -mfloat-abi=soft -I. -mlongcalls -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -lc -Wl,--end-group -lgcc -std=c11" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS " -mfloat-abi=soft -I. -mlongcalls -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -lc -Wl,--end-group -lgcc -std=c11 -std=c++11 -fno-exceptions -fno-rtti"  CACHE INTERNAL "")

set(CMAKE_EXE_LINKER_FLAGS    "-nostdlib -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -lc -Wl,--end-group -lgcc -Teagle.app.v6.ld" CACHE INTERNAL "")

#set(CC_EXECUTABLE_SUFFIX ".elf" CACHE INTERNAL "")
  
#add_definitions(-DARM_MATH_CM4 -DUSE_FULL_ASSERT -DSTM32F407xx)

set(CCOS_DRIVER_CPU    					    TRUE CACHE INTERNAL "")
set(CCOS_DRIVER_CPU_ESPRESSIF 				TRUE CACHE INTERNAL "")
set(CCOS_DRIVER_CPU_ESPRESSIF_ESP8266       TRUE CACHE INTERNAL "")
