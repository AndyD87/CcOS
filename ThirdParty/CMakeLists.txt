################################################################################
# Add additional and external projects into Workspace:
################################################################################

if( CCOS_THIRDPARTY_SQLITE)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sqlite3)
  set(sqlite3_LIBRARIES    ${sqlite3_LIBRARIES}   PARENT_SCOPE)
  set(sqlite3_INCLUDE_DIR  ${sqlite3_INCLUDE_DIR} PARENT_SCOPE)
  set(sqlite3_LIB_DIR      ${sqlite3_LIB_DIR}     PARENT_SCOPE)
endif()

if( CCOS_THIRDPARTY_LIBJPG )
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/jpeg)
endif()

if( CCOS_THIRDPARTY_OPENSSL )
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/openssl)
  set(OPENSSL_LIBRARIES    ${OPENSSL_LIBRARIES}   PARENT_SCOPE)
  set(OPENSLL_INCLUDE_DIR  ${OPENSLL_INCLUDE_DIR} PARENT_SCOPE)
  set(OPENSLL_LIB_DIR      ${OPENSLL_LIB_DIR}     PARENT_SCOPE)
endif()
