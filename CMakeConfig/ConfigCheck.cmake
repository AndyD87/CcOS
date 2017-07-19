###################
# Add additional and external projects into Workspace:
###################

if( CCOS_THIRDPARTY_SQLITE)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/ThirdParty/sqlite3)
endif()

if( CCOS_THIRDPARTY_LIBJPG )
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/ThirdParty/jpeg)
endif()

if( CCOS_THIRDPARTY_OPENSSL )
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/ThirdParty/openssl)
endif()
