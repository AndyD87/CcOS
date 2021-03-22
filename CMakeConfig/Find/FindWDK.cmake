################################################################################
# @brief Find WDK to setup kernel mode for CcOS
# Redistribution and use is allowed under the OSI-approved 3-clause BSD license.
# Copyright (c) 2018 Sergey Podobry (sergey.podobry at gmail.com). All rights reserved.
################################################################################

# Do only search if msvc for build is set.
if(MSVC_VERSION)
  # Search for WDK10 wich is required for VS2017 AND HIGHER
  if(MSVC_VERSION GREATER 1909)
    if(DEFINED ENV{WDKContentRoot})
        file(GLOB WDK_NTDDK_FILES
            "$ENV{WDKContentRoot}/Include/*/km/ntddk.h"
        )
    elseif(DEFINED EWDK_PATH)
        file(GLOB WDK_NTDDK_FILES
            "${EWDK_PATH}/*/Program Files/Windows Kits/10/Include/*/km/ntddk.h"
        )
        if(NOT WDK_NTDDK_FILES)
          file(GLOB WDK_NTDDK_FILES
              "${EWDK_PATH}/Program Files/Windows Kits/10/Include/*/km/ntddk.h"
          )
        endif()
    else()
    file(GLOB WDK_NTDDK_FILES
        "C:/Program Files*/Windows Kits/10/Include/*/km/ntddk.h"
    )
    endif()

    if(WDK_NTDDK_FILES)
        list(GET WDK_NTDDK_FILES -1 WDK_LATEST_NTDDK_FILE)
    endif()

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(WDK REQUIRED_VARS WDK_LATEST_NTDDK_FILE)

    if (NOT WDK_LATEST_NTDDK_FILE)
        return()
    else()
      set(WDK_FOUND TRUE)
    endif()

    get_filename_component(WDK_ROOT ${WDK_LATEST_NTDDK_FILE} DIRECTORY)
    get_filename_component(WDK_ROOT ${WDK_ROOT} DIRECTORY)
    get_filename_component(WDK_VERSION ${WDK_ROOT} NAME)
    get_filename_component(WDK_ROOT ${WDK_ROOT} DIRECTORY)
    get_filename_component(WDK_ROOT ${WDK_ROOT} DIRECTORY)

    file(GLOB WDK_CERTMGR_FILES
        "${WDK_ROOT}/bin/*/x64/certmgr.exe"
    )
    if(WDK_CERTMGR_FILES)
        list(GET WDK_CERTMGR_FILES -1 WDK_CERTMGR_FILE)
    endif()

    file(GLOB WDK_MAKECERT_FILES
        "${WDK_ROOT}/bin/*/x64/makecert.exe"
    )
    if(WDK_MAKECERT_FILES)
        list(GET WDK_MAKECERT_FILES -1 WDK_MAKECERT_FILE)
    endif()

    file(GLOB WDK_SIGNTOOL_FILES
        "${WDK_ROOT}/bin/*/x64/signtool.exe"
    )
    if(WDK_SIGNTOOL_FILES)
        list(GET WDK_SIGNTOOL_FILES -1 WDK_SIGNTOOL_FILE)
    endif()

    file(GLOB WDK_INF2CAT_FILES
        "${WDK_ROOT}/bin/*/x86/inf2cat.exe"
    )
    if(WDK_INF2CAT_FILES)
        list(GET WDK_INF2CAT_FILES -1 WDK_INF2CAT_FILE)
    endif()

    message(STATUS "WDK_ROOT: " ${WDK_ROOT})
    message(STATUS "WDK_VERSION: " ${WDK_VERSION})

    set(WDK_WINVER "0x0A00" CACHE STRING "Default WINVER for WDK targets")

    # Remove RTC flag, which was set from cmake
    set(WDK_ADDITIONAL_FLAGS_FILE "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/wdkflags.h")
    file(WRITE ${WDK_ADDITIONAL_FLAGS_FILE} "#pragma runtime_checks(\"suc\", off)")

    set(WDK_COMPILE_FLAGS
        "/Zp8" # set struct alignment
        "/GF"  # enable string pooling
        "/GR-" # disable RTTI
        "/Gz" # __stdcall by default
        "/kernel"  # create kernel mode binary
        "/FIwarning.h" # disable warnings in WDK headers
        "/FI${WDK_ADDITIONAL_FLAGS_FILE}" # include file to disable RTC
        )

    set(WDK_COMPILE_DEFINITIONS "WINNT=1")
    set(WDK_COMPILE_DEFINITIONS_DEBUG "MSC_NOOPT;DEPRECATE_DDK_FUNCTIONS=1;DBG=1")

    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        list(APPEND WDK_COMPILE_DEFINITIONS "_X86_=1;i386=1;STD_CALL")
        set(WDK_PLATFORM "x86")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
        list(APPEND WDK_COMPILE_DEFINITIONS "_WIN64;_AMD64_;AMD64")
        set(WDK_PLATFORM "x64")
    else()
        message(FATAL_ERROR "Unsupported architecture")
    endif()

    string(CONCAT WDK_LINK_FLAGS
        "/DRIVER "                          # Enable driver mode
        "/MANIFEST:NO "                     # No Manifests for driver
        "/OPT:REF "                         # eliminates functions and data that are never referenced
        "/INCREMENTAL:NO "                  # Disable incremental linking
        "/OPT:ICF "                         # perform identical COMDAT folding, wich would be deactivated on debug.
        "/SUBSYSTEM:NATIVE "                # No subsystem!
        "/MERGE:_TEXT=.text;_PAGE=PAGE "    # Move static data to page
        "/NODEFAULTLIB "                    # do not link default CRT
        "/SECTION:INIT,d "                  # Remove init section
        # Disabled, Version should be set wich rc files "/VERSION:10.0 "
        "/ignore:4210 "                     # ignore CRT init static warning
        )

    # Generate imported targets for WDK lib files
    file(GLOB WDK_LIBRARIES "${WDK_ROOT}/Lib/${WDK_VERSION}/km/${WDK_PLATFORM}/*.lib")
    foreach(LIBRARY IN LISTS WDK_LIBRARIES)
        get_filename_component(LIBRARY_NAME ${LIBRARY} NAME_WE)
        string(TOUPPER ${LIBRARY_NAME} LIBRARY_NAME)
        add_library(WDK::${LIBRARY_NAME} INTERFACE IMPORTED)
        set_property(TARGET WDK::${LIBRARY_NAME} PROPERTY INTERFACE_LINK_LIBRARIES  ${LIBRARY})
    endforeach(LIBRARY)
    unset(WDK_LIBRARIES)
  endif() # VS 2017 with 1910
endif()
