add_subdirectory( CcFtp/ )
add_subdirectory( CcTftp/ )
add_subdirectory( CcHttp/ )
add_subdirectory( CcShell/ )
add_subdirectory( CcDhcp/ )
if( WINDOWS OR
    LINUX )
  add_subdirectory( CcSsl/ )
endif()