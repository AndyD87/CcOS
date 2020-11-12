################################################################################
# Default Windows Config
################################################################################
message("- Default Configuration for Windows is selected")

################################################################################
# Setup default directories
################################################################################
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set( CMAKE_INSTALL_PREFIX       "C:/Program Files/CcOS/" )
endif()

set(WINDOWS TRUE)

find_package(WDK)

if(WDK_FOUND)
  message("-- WDK available")
  set(CCKERNEL_MODULE_AVAILABLE TRUE)
  file (GLOB CCKERNEL_MODULE_SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/Windows/*.cpp"
  )
  
  function(CcAddDriverOverride _target) #wdk_add_driver
      cmake_parse_arguments(WDK "" "KMDF;WINVER" "" ${ARGN})
      
      add_executable(${_target} ${WDK_UNPARSED_ARGUMENTS})
  
      set_target_properties(${_target} PROPERTIES SUFFIX ".sys")
      set_target_properties(${_target} PROPERTIES COMPILE_OPTIONS "${WDK_COMPILE_FLAGS}")
      set_target_properties(${_target} PROPERTIES COMPILE_DEFINITIONS
          "${WDK_COMPILE_DEFINITIONS};$<$<CONFIG:Debug>:${WDK_COMPILE_DEFINITIONS_DEBUG}>;_WIN32_WINNT=${WDK_WINVER}"
          )
      set_target_properties(${_target} PROPERTIES LINK_FLAGS "${WDK_LINK_FLAGS}")
  
      target_include_directories(${_target} SYSTEM PRIVATE
          "${WDK_ROOT}/Include/${WDK_VERSION}/shared"
          "${WDK_ROOT}/Include/${WDK_VERSION}/km"
          )
  
      target_link_libraries(${_target} PUBLIC WDK::NTOSKRNL WDK::HAL WDK::BUFFEROVERFLOWK WDK::WMILIB)
  
      if(CMAKE_SIZEOF_VOID_P EQUAL 4)
          target_link_libraries(${_target} WDK::MEMCMP)
      endif()
  
      if(DEFINED WDK_KMDF)
          target_include_directories(${_target} SYSTEM PRIVATE "${WDK_ROOT}/Include/wdf/kmdf/${WDK_KMDF}")
          target_link_libraries(${_target}
              "${WDK_ROOT}/Lib/wdf/kmdf/${WDK_PLATFORM}/${WDK_KMDF}/WdfDriverEntry.lib"
              "${WDK_ROOT}/Lib/wdf/kmdf/${WDK_PLATFORM}/${WDK_KMDF}/WdfLdr.lib"
              )
  
          if(CMAKE_SIZEOF_VOID_P EQUAL 4)
              set_property(TARGET ${_target} APPEND_STRING PROPERTY LINK_FLAGS "/ENTRY:FxDriverEntry@8")
          elseif(CMAKE_SIZEOF_VOID_P  EQUAL 8)
              set_property(TARGET ${_target} APPEND_STRING PROPERTY LINK_FLAGS "/ENTRY:FxDriverEntry")
          endif()
      else()
          if(CMAKE_SIZEOF_VOID_P EQUAL 4)
              set_property(TARGET ${_target} APPEND_STRING PROPERTY LINK_FLAGS "/ENTRY:GsDriverEntry@8")
          elseif(CMAKE_SIZEOF_VOID_P  EQUAL 8)
              set_property(TARGET ${_target} APPEND_STRING PROPERTY LINK_FLAGS "/ENTRY:GsDriverEntry")
          endif()
      endif()
  endfunction()
  
  function(CcAddDriverLibraryOverride _target)
      cmake_parse_arguments(WDK "" "KMDF;WINVER" "" ${ARGN})
  
      add_library(${_target} ${WDK_UNPARSED_ARGUMENTS})
  
      set_target_properties(${_target} PROPERTIES COMPILE_OPTIONS "${WDK_COMPILE_FLAGS}")
      set_target_properties(${_target} PROPERTIES COMPILE_DEFINITIONS
          "${WDK_COMPILE_DEFINITIONS};$<$<CONFIG:Debug>:${WDK_COMPILE_DEFINITIONS_DEBUG};_WIN32_WINNT=${WDK_WINVER}>"
          )
  
      target_include_directories(${_target} SYSTEM PRIVATE
          "${WDK_ROOT}/Include/${WDK_VERSION}/shared"
          "${WDK_ROOT}/Include/${WDK_VERSION}/km"
          )
  
      if(DEFINED WDK_KMDF)
          target_include_directories(${_target} SYSTEM PRIVATE "${WDK_ROOT}/Include/wdf/kmdf/${WDK_KMDF}")
      endif()
  endfunction()

endif()
