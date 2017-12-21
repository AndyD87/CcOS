# @Author Andreas Dirmeier
#
# Add all Applications, in this folder, to build
# Applications must be stored in a seperate Folder, except Backup will be ignored.
# The Applications must have a CMakeLists.txt in its directory.
#############

if( DEFINED WINDOWS OR DEFINED LINUX)
  CcGetSubDirs(DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR})
  foreach(DIR ${DIR_LIST})
    add_subdirectory(${DIR})
  endforeach()
endif()
