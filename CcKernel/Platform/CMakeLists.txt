###################
# Depending on Target, just the correct System Library will be loaded:
###################
if(DEFINED TARGET_SYSTEM)
  set(TARGET_SYSTEM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_SYSTEM}" )
else ()
  set(TARGET_SYSTEM ${CMAKE_SYSTEM_NAME})
  set(TARGET_SYSTEM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_SYSTEM_NAME}" )
endif()

###################
# Include Target Settings:
###################
message( "TargetSystem: " ${TARGET_SYSTEM} )
if( EXISTS ${TARGET_SYSTEM_DIR} )
  add_subdirectory( ${TARGET_SYSTEM_DIR} )
  set(CC_TARGET_SYSTEM_DIR ${TARGET_SYSTEM_DIR} PARENT_SCOPE)
else()
  message( SEND_ERROR "No Matching System found for Kernel" )
endif()

push_up_kernel_vars()