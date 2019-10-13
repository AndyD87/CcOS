if(NOT DEFINED CCOS_CCNETWORK_CCHTTP_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCNETWORK_CCHTTP_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT           "CcHttp" )
  set ( CURRENT_PROJECT_IDE_PATH  "CcNetwork" )
  
  ########
  # Add Source Files
  ########
  file (GLOB HTTP_SOURCE_FILES
        "*.c"
        "*.cpp"
        "*.h"
        "Resources/*.c"
        "Resources/*.cpp"
        "Resources/*.h")
  
  macro (add_to_http_sources)
      foreach (_src ${ARGN})
          CcListAppendOnce(HTTP_SOURCE_FILES "${_src}")
      endforeach()
      set (HTTP_SOURCE_FILES ${HTTP_SOURCE_FILES} PARENT_SCOPE)
  endmacro()
  
  ########
  # Add Subdirecotry Files
  ########
  add_subdirectory(Applications/)
  add_subdirectory(HttpProvider/)
  add_subdirectory(RestApi/)
  
  set( DEFINITIONS "" )
  set( LIBRARIES  CcKernel CcDocuments )

  if( CCOS_THIRDPARTY_OPENSSL_ACTIVE GREATER ${CCOS_BUILDLEVEL} )
    list( APPEND LIBRARIES CcSsl)
    list( APPEND DEFINITIONS -DCCSSL_ENABLED)
  endif()
  
  add_definitions( ${DEFINITIONS} )
  
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()
  
  #############################################################################
  # Add resources to this project
  #############################################################################
  find_program(CcOSResource_FOUND CcOSResource)
  set(CURRENT_PROJECT_RESOURCE_FILES
      ${CMAKE_CURRENT_LIST_DIR}/Resources/CcOSWebframework.js
      ${CMAKE_CURRENT_LIST_DIR}/Resources/CcOSWebframework.js.c
      ${CMAKE_CURRENT_LIST_DIR}/Resources/CcOSWebframework.js.h
      ${CMAKE_CURRENT_LIST_DIR}/Resources/CcOSWebframework.css
      ${CMAKE_CURRENT_LIST_DIR}/Resources/CcOSWebframework.css.c
      ${CMAKE_CURRENT_LIST_DIR}/Resources/CcOSWebframework.css.h
      ${CMAKE_CURRENT_LIST_DIR}/Resources/jquery-3.4.1.min.js
      ${CMAKE_CURRENT_LIST_DIR}/Resources/jquery-3.4.1.min.js.c
      ${CMAKE_CURRENT_LIST_DIR}/Resources/jquery-3.4.1.min.js.h)
   
  if(CcOSResource_FOUND)
      add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/Resources/CcOSWebframework.js.c
                                ${CMAKE_CURRENT_LIST_DIR}/Resources/CcOSWebframework.js.h
                                COMMAND CcOSResource -i CcOSWebframework.js -o CcOSWebframework.js -n CcOSWebframework_Js -s
                                DEPENDS ${CMAKE_CURRENT_LIST_DIR}/Resources/CcOSWebframework.js
                                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Resources)
      add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/Resources/CcOSWebframework.css.c
                                ${CMAKE_CURRENT_LIST_DIR}/Resources/CcOSWebframework.css.h
                                COMMAND CcOSResource -i CcOSWebframework.css -o CcOSWebframework.css -n CcOSWebframework_Css -s
                                DEPENDS ${CMAKE_CURRENT_LIST_DIR}/Resources/CcOSWebframework.css
                                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Resources)
      add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/Resources/jquery-3.4.1.min.js.c
                                ${CMAKE_CURRENT_LIST_DIR}/Resources/jquery-3.4.1.min.js.h
                                COMMAND CcOSResource -i jquery-3.4.1.min.js -n g_Jquery_3_4_1_Min
                                DEPENDS ${CMAKE_CURRENT_LIST_DIR}/Resources/jquery-3.4.1.min.js
                                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Resources)
      add_custom_target("${CURRENT_PROJECT}_Resources" DEPENDS  ${CURRENT_PROJECT_RESOURCE_FILES})
      set_target_properties("${CURRENT_PROJECT}_Resources" PROPERTIES FOLDER "CcOS/Resources/${CURRENT_PROJECT_IDE_PATH}")                              
      source_group( "Resources" FILES ${CURRENT_PROJECT_RESOURCE_FILES})
  endif()
  
  #############################################################################
  # Setup project
  #############################################################################
  add_library( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${HTTP_SOURCE_FILES} ${CURRENT_PROJECT_RESOURCE_FILES})
  
  #############################################################################
  # Setup project dependencies
  #############################################################################
  if(CcOSResource_FOUND)
    add_dependencies(${CURRENT_PROJECT} "${CURRENT_PROJECT}_Resources")
  endif()
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  
  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )
  

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${HTTP_SOURCE_FILES})
  
  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
