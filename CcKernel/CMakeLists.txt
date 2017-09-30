if(NOT DEFINED CCOS_CCKERNEL_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCKERNEL_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  ########
  # Add Source Files
  ########
  file (GLOB KERNEL_SOURCE_FILES
        "src/*.c"
        "src/*.cpp"
        "*.h" )
  
  set ( CURRENT_PROJECT CcKernel)
        
  set ( KERNEL_LIBRARIES )    
            
  set ( KERNEL_DEFINITIONS )    
            
  set ( KERNEL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Types )  
  
  macro (add_to_kernel_sources)
      foreach (_src ${ARGN})
          list (APPEND KERNEL_SOURCE_FILES "${_src}")
      endforeach()
      set (KERNEL_SOURCE_FILES ${KERNEL_SOURCE_FILES} PARENT_SCOPE)
  endmacro()
  
  macro (add_to_kernel_definitions)
      foreach (_src ${ARGN})
          list (APPEND KERNEL_DEFINITIONS "${_src}")
      endforeach()
      set (KERNEL_DEFINITIONS ${KERNEL_DEFINITIONS} PARENT_SCOPE)
  endmacro()
  
  macro (add_to_kernel_includes)
      foreach (_src ${ARGN})
          list (APPEND KERNEL_INCLUDES "${_src}")
      endforeach()
      set (KERNEL_INCLUDES ${KERNEL_INCLUDES} PARENT_SCOPE)
  endmacro()
  
  macro (add_to_kernel_libraries)
      foreach (_src ${ARGN})
          list (APPEND KERNEL_LIBRARIES "${_src}")
      endforeach()
      set (KERNEL_LIBRARIES ${KERNEL_LIBRARIES} PARENT_SCOPE)
  endmacro()
  
  macro (push_up_kernel_vars)
    add_to_kernel_sources     ( ${KERNEL_SOURCESS} )
    add_to_kernel_definitions ( ${KERNEL_DEFINITIONS} )
    add_to_kernel_includes    ( ${KERNEL_INCLUDES} )
    add_to_kernel_libraries   ( ${KERNEL_LIBRARIES} )
  endmacro()
  
  ########
  # Add Subdirecotry Files
  ########
  add_subdirectory(Driver/)
  add_subdirectory(Network/)
  add_subdirectory(Communication/)
  add_subdirectory(Devices/)
  add_subdirectory(Types/)
  add_subdirectory(Platform/)
  add_subdirectory(Events/)
  
  ########
  # Update Version Info
  ########
  configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.h.in ${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.h @ONLY)
  
  add_definitions( ${KERNEL_DEFINITIONS} )
  
  include_directories( ${KERNEL_INCLUDES} )
  
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT})
    list(APPEND KERNEL_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
  endif()
  
  add_library( ${CURRENT_PROJECT} ${CCOS_LINK_TYPE} ${KERNEL_SOURCE_FILES} )
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS")
  
  target_link_libraries ( ${CURRENT_PROJECT} ${KERNEL_LIBRARIES} )
  
  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} ${KERNEL_INCLUDES})
  CcOSSetInstall( ${CURRENT_PROJECT} )
  CcSetFiltersByFolders(${KERNEL_SOURCE_FILES})
  
  ########
  # Add an unity-test, if available
  ########
  if( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
    add_subdirectory("test")
  endif()
endif(NOT DEFINED CCOS_CCKERNEL_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
