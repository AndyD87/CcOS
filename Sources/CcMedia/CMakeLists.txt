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
        IImageConverter.h
        Image/CImageBmp.h
        Image/CImagePixmap.h
        Image/CImagePpm.h
        Playlist/CcM3U.h
  )

  set (SOURCE_FILES
        ${HEADER_FILES}
        CcImage.cpp
        CcImageRaw.cpp
        CcVideo.cpp
        IImageConverter.cpp
        Image/CImageBmp.cpp
        Image/CImagePixmap.cpp
        Image/CImagePpm.cpp
        Playlist/CcM3U.cpp
  )

  set( LIBRARIES
          CcKernel )


  if( CCOS_THIRDPARTY_LIBJPG )
    list(APPEND SOURCE_FILES "Image/CImageJpeg.h" )
    list(APPEND SOURCE_FILES "Image/CImageJpeg.cpp" )
    list(APPEND LIBRARIES jpeg)
  endif()

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddLibrary(${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

  set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS")
  CcTargetHeaders( ${CURRENT_PROJECT} "${HEADER_FILES}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} CcHttp )

  #################
  # Post configuration steps
  #################
  CcTargetIncludes( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
