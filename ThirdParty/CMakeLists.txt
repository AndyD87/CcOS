################################################################################
# Add additional and external projects into Workspace:
################################################################################

if( CCOS_THIRDPARTY_SQLITE)
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sqlite3)
endif()

if( CCOS_THIRDPARTY_LIBJPG )
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/jpeg)
endif()

if( CCOS_THIRDPARTY_OPENSSL )
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/openssl)
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/mtp)
