if( CCOS_DRIVER_RASPICAM )
  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/RaspiCam.cpp
        ${CMAKE_CURRENT_LIST_DIR}/RaspiCam.h
        ${CMAKE_CURRENT_LIST_DIR}/RaspiCamDriver.cpp
        ${CMAKE_CURRENT_LIST_DIR}/RaspiCamDriver.h
        ${CMAKE_CURRENT_LIST_DIR}/RaspiCamModule.cpp
        ${CMAKE_CURRENT_LIST_DIR}/RaspiCamModule.h
  )
  
  
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
                       
  CcAddLibrary(driverLinuxV4L ${CC_LINK_TYPE} ${SOURCE_FILES} ${HEADER_FILES} )
  
  link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../../lib )
  
  set_property( TARGET driverLinuxV4L PROPERTY FOLDER "CcOS")
  
  target_link_libraries ( driverLinuxV4L LINK_PRIVATE
                                CcKernel
                                libv4l-0 )
  
  set_property( TARGET CcMedia PROPERTY FOLDER "CcOS/IDriver/Camera")
  
  target_include_directories( driverLinuxV4L PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif()