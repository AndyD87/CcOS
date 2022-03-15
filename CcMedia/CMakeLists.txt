if(NOT DEFINED CCOS_CCMEDIA_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCMEDIA_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT CcMedia )

  ########
  # Add Source Files
  ########
  set (HEADER_FILES
        CcImage.h
        CcImageRaw.h
        CcMedia.h
        CcVideo.h
        Converter/CImageBmp.h
        Converter/CImagePpm.h
        Converter/IImageConverter.h
  )

  set (SOURCE_FILES
        ${HEADER_FILES}
        CcImage.cpp
        CcImageRaw.cpp
        CcVideo.cpp
        Converter/CImageBmp.cpp
        Converter/CImagePpm.cpp
        Converter/IImageConverter.cpp
  )

  set( LIBRARIES
          CcKernel )


  if( CCOS_THIRDPARTY_LIBJPG )
    list(APPEND SOURCE_FILES "External/CImageJpeg.h" )
    list(APPEND SOURCE_FILES "External/CImageJpeg.cpp" )
    list(APPEND LIBRARIES jpeg)
  endif()

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddLibrary(${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

  set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS")
  set_target_properties( ${CURRENT_PROJECT} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

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
