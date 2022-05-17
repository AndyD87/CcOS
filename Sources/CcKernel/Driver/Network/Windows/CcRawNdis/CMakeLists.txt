if(WINDOWS AND CCOS_GENERIC_NETWORK_WINDOWS_CCRAWNDIS)
  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/CcRawNdisDriver.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcRawNdisDriver.h
        ${CMAKE_CURRENT_LIST_DIR}/CcRawNdisNetwork.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcRawNdisNetwork.h
  )
        
  set(CCRAWNDIS_DIR  ${CC_CACHE_DIR}/Sources/CcRawNdis)
  CcGitClone(${CCRAWNDIS_DIR} https://coolcow.de/projects/Tools/CcRawNdis.git)
  CcGitUpdateAndCheckout(${CCRAWNDIS_DIR} "master" )
  
  list(APPEND SOURCE_FILES "${CCRAWNDIS_DIR}/CcRawNdisTest/CcRawNdisAccess.h"
                           "${CCRAWNDIS_DIR}/CcRawNdisTest/CcRawNdisAccess.cpp"
  )
  
  add_to_driver_headers("Network/Windows/CcRawNdis/CcRawNdisDriver.h")
  add_to_driver_names  (2 "CcRawNdisDriver")
  add_to_driver_sources(${HEADER_FILES} ${SOURCE_FILES})
  add_to_driver_includes(${CMAKE_CURRENT_LIST_DIR} "${CCRAWNDIS_DIR}/CcRawNdisTest" "${CCRAWNDIS_DIR}/Common")
  add_to_driver_definitions()
elseif(CCOS_GENERIC_NETWORK_WINDOWS_CCRAWNDIS)
  message(WARNING "-- Windows network driver not supported")
endif()