###################
# Add additional and external projects into Workspace:
###################

if( CCOS_THIRDPARTY_SQLITE)
  add_subdirectory(CMakeConfig/ThirdParty/sqlite3)
endif()

if( CCOS_THIRDPARTY_LIBJPG )
  add_subdirectory(CMakeConfig/ThirdParty/jpeg)
endif()

if( CCOS_THIRDPARTY_OPENSSL )
  add_subdirectory(CMakeConfig/ThirdParty/openssl)
endif()
