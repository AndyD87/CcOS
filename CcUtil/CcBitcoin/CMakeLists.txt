if(NOT DEFINED CCOS_CCUTIL_CCBITCOIN_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCUTIL_CCBITCOIN_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcBitcoin )
  set ( CURRENT_PROJECT_IDE_PATH  CcUtil    )


  ########
  # Add Source Files
  ########
  set(SOURCE_FILES
      CcBitcoin.h
      CcBitcoinAddress.cpp
      CcBitcoinAddress.h
      CcBitcoinBlockTemplate.cpp
      CcBitcoinBlockTemplate.h
      CcBitcoinClient.cpp
      CcBitcoinClient.h
  )

  set( LIBRARIES
        CcKernel
        CcHttp
        CcDocuments
  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
