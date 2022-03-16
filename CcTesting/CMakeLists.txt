if(NOT DEFINED CCOS_CCTESTING_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCTESTING_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcTesting )
  set ( CURRENT_PROJECT_IDE_PATH   )

  ########
  # Add Header Files
  ########
  set( HEADER_FILES
        CcTestFramework.h
        CcTest.h
        CcTesting.h
        CcTestModule.h
        CcTestSocket.h
        CcTestUtility.h
        ITest.h
        CcExportTest.h
  )

  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
        ${HEADER_FILES}
        CcTestFramework.cpp
        CcTestModule.cpp
        CcTestSocket.cpp
        CcTestUtility.cpp
        CcExportTest.cpp
  )

  set( LIBRARIES
        CcKernel
        CcModule
  )

  include_directories( ${CMAKE_CURRENT_LIST_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  # Make it always shared if not generic
  if(GENERIC)
    set(LIB_SHARED STATIC)
  else()
    set(LIB_SHARED SHARED)
  endif()
  CcAddLibrary( ${CURRENT_PROJECT}       ${LIB_SHARED} ${SOURCE_FILES} )
  CcAddLibrary( "${CURRENT_PROJECT}4Test"  ${LIB_SHARED} ${SOURCE_FILES} )

  set_property( TARGET ${CURRENT_PROJECT}4Test PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  CcTargetHeaders( ${CURRENT_PROJECT} "${HEADER_FILES}")


  target_link_libraries ( ${CURRENT_PROJECT}      LINK_PRIVATE ${LIBRARIES} )
  target_link_libraries ( ${CURRENT_PROJECT}4Test LINK_PRIVATE ${LIBRARIES} )
  #################
  # Post configuration steps
  #################
  CcTargetIncludes( ${CURRENT_PROJECT} )
  CcTargetIncludes( ${CURRENT_PROJECT}4Test )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
  CcOSLibSettings(${CURRENT_PROJECT}4Test TRUE TRUE ${SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_LIST_DIR}/test)
endif()
