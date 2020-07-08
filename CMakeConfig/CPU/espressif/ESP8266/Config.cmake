message("- CPU: ESP8266 @ ${CMAKE_CURRENT_LIST_FILE}")

set(CMAKE_SYSTEM_PROCESSOR arm )
set(CMAKE_C_FLAGS   "-DCCOS_GENERIC -mlongcalls -nostdlib -Wl,--start-group -lc -Wl,--end-group -lgcc -std=c11   -ffreestanding -Wall -Wextra -g" )
set(CMAKE_CXX_FLAGS "-DCCOS_GENERIC -mlongcalls -nostdlib -Wl,--start-group -lc -Wl,--end-group -lgcc -std=c++11 -ffreestanding -Wall -Wextra -g -fno-exceptions -fno-rtti -fabi-version=0 -fno-use-cxa-atexit"  )

# Can be removed after gcc 5.2.0 support is removed (ref GCC_NOT_5_2_0)
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -Wl,--gc-sections" )

set(CMAKE_C_FLAGS_DEBUG "")

set(CCOS_NO_APPS                            TRUE )
set(CCOS_DRIVER_CPU                          TRUE )
set(CCOS_DRIVER_CPU_ESPRESSIF               TRUE )
set(CCOS_DRIVER_CPU_ESPRESSIF_ESP8266       TRUE )
set(CCOS_CCKERNEL_GENERIC_NO_SYSTEM_THREAD  TRUE )

set(CC_NO_PROJECT               TRUE      )
set(CC_LINK_TYPE                STATIC    )
set(GENERIC                     TRUE      )
set(CMAKE_SYSTEM_NAME           Generic   )

set(CCOS_CMAKE_INCLUDES "${CMAKE_CURRENT_LIST_DIR}/Project.cmake" )

enable_language(CXX C ASM)
