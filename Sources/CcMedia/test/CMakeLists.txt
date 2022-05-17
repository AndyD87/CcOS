################################################################################
# Create Tests only if we are building CcOS
################################################################################
if("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")

  set ( CURRENT_TEST_PROJECT ${CURRENT_PROJECT}Test )

  ########
  # Add Source Files
  ########
  set(SOURCE_FILES
        CImageBmpTest.cpp
        CImageBmpTest.h
        CImagePpmTest.cpp
        CImagePpmTest.h
        CImageRawTest.cpp
        CImageRawTest.h
        CImageTest.cpp
        CImageTest.h
        CM3UTest.cpp
        CM3UTest.h
        main.cpp
  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_TEST_PROJECT} SOURCE_FILES )
  endif()

  #############################################################################
  # Add resources to this project
  #############################################################################
  find_program(CcOSResource_FOUND CcOSResource)
  file (GLOB CURRENT_TEST_PROJECT_RESOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/Resources/*"
  )

  if(CcOSResource_FOUND)
      CcAddResourceBinary(${CMAKE_CURRENT_LIST_DIR}/Resources TestBild-1bit.bmp  TestBild_1Bit_Bmp  CURRENT_TEST_PROJECT_RESOURCE_FILES)
      CcAddResourceBinary(${CMAKE_CURRENT_LIST_DIR}/Resources TestBild-4bit.bmp  TestBild_4Bit_Bmp  CURRENT_TEST_PROJECT_RESOURCE_FILES)
      CcAddResourceBinary(${CMAKE_CURRENT_LIST_DIR}/Resources TestBild-16bit.bmp TestBild_16Bit_Bmp CURRENT_TEST_PROJECT_RESOURCE_FILES)
      CcAddResourceBinary(${CMAKE_CURRENT_LIST_DIR}/Resources TestBild-24bit.bmp TestBild_24Bit_Bmp CURRENT_TEST_PROJECT_RESOURCE_FILES)

      add_custom_target("${CURRENT_TEST_PROJECT}_Resources" DEPENDS  ${CURRENT_TEST_PROJECT_RESOURCE_FILES})
      set_target_properties("${CURRENT_TEST_PROJECT}_Resources" PROPERTIES FOLDER "CcOS/Resources/${CURRENT_PROJECT_IDE_PATH}")
      CcSetFiltersByFolders(${CURRENT_TEST_PROJECT_RESOURCE_FILES})
  endif()

  CcAddExecutable( ${CURRENT_TEST_PROJECT} ${SOURCE_FILES} ${CURRENT_TEST_PROJECT_RESOURCE_FILES} )

  set_property( TARGET ${CURRENT_TEST_PROJECT} PROPERTY FOLDER "CcOS/Testing/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_TEST_PROJECT} LINK_PRIVATE
                                ${CURRENT_PROJECT}
                                CcTesting )

  CcAddTest(${CURRENT_TEST_PROJECT})

  #################
  # Post configuration steps
  #################
  CcOSExeSettings(${CURRENT_PROJECT})
  CcSetFiltersByFolders(${SOURCE_FILES})

endif("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")
