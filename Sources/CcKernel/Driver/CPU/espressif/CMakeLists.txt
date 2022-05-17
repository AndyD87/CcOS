################################################################################
# Check if Modules are required for Applications by reading config
################################################################################
if(CCOS_DRIVER_CPU_ESPRESSIF_ESP8266)
  add_subdirectory(ESP8266)
endif ()

push_up_driver_vars()
