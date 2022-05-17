# @Author Andreas Dirmeier
#
# Add all Applications, in this folder, to build
# Applications must be stored in a seperate Folder, except Backup will be ignored.
# The Applications must have a CMakeLists.txt in its directory.
#############

##
# @brief add all subdirectories to a list
####
macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
        list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

# Add Subdirectories to list
subdirlist(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Remove Backup from List
list(REMOVE_ITEM SUBDIRS "Backup")

foreach(subdir ${SUBDIRS})
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt")
    add_subdirectory(${subdir})
  endif()
endforeach()
