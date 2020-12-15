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

# Driver mode is only available wit msvc compiler
if(DEFINED MSVC)
  find_package(WDK)
  
  if(WDK_FOUND)
    message("-- WDK available")
    set(CCKERNEL_MODULE_AVAILABLE TRUE)
    file (GLOB CCKERNEL_MODULE_SOURCE_FILES
          "${CMAKE_CURRENT_LIST_DIR}/Windows/*.cpp"
    )
    
    # Setup TestCertificate
    set(WDK_TESTCERT_NAME "CcKernelModule_TestCert") # CACHE INTERNAL "Name of Certificate for signing drivers")
    set(WDK_TESTCERT_STORE "currentUser") # CACHE INTERNAL "Storage location in certifacte manager from windows")
    if(NOT WDK_TESTCERT_AVAILABLE)
      if(EXISTS ${WDK_CERTMGR_FILE})
        execute_process(COMMAND ${WDK_CERTMGR_FILE} /v /s /r ${WDK_TESTCERT_STORE} PrivateCertStore
                        OUTPUT_VARIABLE CertMgr_EXTRACT_OUTPUT
                        RESULT_VARIABLE CertMgr_EXTRACT_RESULT
        )
        string(REGEX MATCH "CN=${WDK_TESTCERT_NAME}" WDK_TESTCERT_NAME_FOUND "${CertMgr_EXTRACT_OUTPUT}")
        if(WDK_TESTCERT_NAME_FOUND)
          message(STATUS "${WDK_TESTCERT_NAME} found")
        else()
          message(STATUS "${WDK_TESTCERT_NAME} not found, start generation")
          if(EXISTS ${WDK_MAKECERT_FILE})
            execute_process(COMMAND ${WDK_MAKECERT_FILE} -r -pe -ss my -sr currentuser -n CN=${WDK_TESTCERT_NAME} ${WDK_TESTCERT_NAME}.cer
                            RESULT_VARIABLE MakeCert_EXTRACT_RESULT
            )
            if(${MakeCert_EXTRACT_RESULT} EQUAL 0)
              message(STATUS "Certificate File ${WDK_TESTCERT_NAME} created")
              execute_process(COMMAND ${WDK_CERTMGR_FILE} /add ${WDK_TESTCERT_NAME}.cer /s /r ${WDK_TESTCERT_STORE} PrivateCertStore
                              RESULT_VARIABLE CertMgr_EXTRACT_RESULT
              )
              if(${CertMgr_EXTRACT_RESULT} EQUAL 0)
                message(STATUS "Certificate ${WDK_TESTCERT_NAME} installed")
                set(WDK_TESTCERT_AVAILABLE TRUE) # CACHE INTERNAL "${WDK_TESTCERT_NAME} available for signing drivers")
              else()
                message(WARNING "MakeCert.exe failed to generate ${WDK_TESTCERT_NAME} with: ${CertMgr_EXTRACT_RESULT}")
              endif()
            else()
              message(WARNING "MakeCert.exe failed to generate ${WDK_TESTCERT_NAME} with: ${MakeCert_EXTRACT_RESULT}")
            endif()
          else()
            message(WARNING "MakeCert.exe not found for generating ${WDK_TESTCERT_NAME}")
          endif()
        endif()
      else()
        message(WARNING "CertMgr.exe not found for verifying ${WDK_TESTCERT_NAME}")
      endif()
    endif()
    
    function(CcAddDriverOverride _target)
        cmake_parse_arguments(WDK "" "KMDF;WINVER" "" ${ARGN})
        CcAppendToBinaryOuptutPath("../drv")
        add_executable(${_target} ${WDK_UNPARSED_ARGUMENTS})
        
        if(EXISTS ${WDK_SIGNTOOL_FILE})
          add_custom_command(TARGET ${_target} POST_BUILD
            COMMAND ${WDK_SIGNTOOL_FILE} sign /a /v /s PrivateCertStore /n ${WDK_TESTCERT_NAME} /t http://timestamp.digicert.com $<TARGET_FILE:${_target}>
          )
        else()
          message(WARNING "SignTool.exe not found for test signing driver")
        endif()
        
        foreach(SOURCE_FILE ${WDK_UNPARSED_ARGUMENTS})
          if(${SOURCE_FILE} MATCHES ".inf$")
            get_filename_component(SOURCE_FILE_NAME ${SOURCE_FILE} NAME)
            get_filename_component(SOURCE_FILE_NAME_WLE ${SOURCE_FILE} NAME_WLE)
            add_custom_command(TARGET ${_target} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOURCE_FILE} $<TARGET_FILE_DIR:${_target}>/${SOURCE_FILE_NAME}
              COMMAND ${WDK_INF2CAT_FILE} /driver:. /os:7_X64,10_X64
              COMMAND ${WDK_SIGNTOOL_FILE} sign /a /v /s PrivateCertStore /n ${WDK_TESTCERT_NAME} /t http://timestamp.digicert.com $<TARGET_FILE_DIR:${_target}>/${SOURCE_FILE_NAME_WLE}.cat
              WORKING_DIRECTORY $<TARGET_FILE_DIR:${_target}>
            )
          endif()
        endforeach()
        
        # Custom command required for generating Cat-Files and signing them

        set_target_properties(${_target} PROPERTIES SUFFIX ".sys")
        set_target_properties(${_target} PROPERTIES COMPILE_OPTIONS "${WDK_COMPILE_FLAGS}")
        set_target_properties(${_target} PROPERTIES COMPILE_DEFINITIONS
            "${WDK_COMPILE_DEFINITIONS};$<$<CONFIG:Debug>:${WDK_COMPILE_DEFINITIONS_DEBUG}>;_WIN32_WINNT=${WDK_WINVER}"
            )
        set_target_properties(${_target} PROPERTIES LINK_FLAGS "${WDK_LINK_FLAGS}")
    
        target_include_directories(${_target} SYSTEM PRIVATE
            "${WDK_ROOT}/Include/${WDK_VERSION}/shared"
            "${WDK_ROOT}/Include/${WDK_VERSION}/km"
            "${WDK_ROOT}/Include/wdf/kmdf/1.9"
            )
    
        target_link_libraries(${_target} PUBLIC WDK::NTOSKRNL WDK::HAL WDK::BUFFEROVERFLOWK WDK::WMILIB)
    
        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            target_link_libraries(${_target} PUBLIC WDK::MEMCMP)
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
            "${WDK_ROOT}/Include/wdf/kmdf/1.9"
            )
    
        if(DEFINED WDK_KMDF)
            target_include_directories(${_target} SYSTEM PRIVATE "${WDK_ROOT}/Include/wdf/kmdf/${WDK_KMDF}")
        endif()
    endfunction()
  
  endif()
endif()