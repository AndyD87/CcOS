# @Author Andreas Dirmeier
#
# Add all Applications, in this folder, to build
# Applications must be stored in a seperate Folder, except Backup will be ignored.
# The Applications must have a CMakeLists.txt in its directory.
#############
cmake_minimum_required (VERSION 2.6)

##
# @brief add all subdirectories to a list
####
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

# Add Subdirectories to list
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Remove Backup from List
LIST(REMOVE_ITEM SUBDIRS "Backup")

FOREACH(subdir ${SUBDIRS})
  IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt")
    ADD_SUBDIRECTORY(${subdir})
  ENDIF()
ENDFOREACH()
