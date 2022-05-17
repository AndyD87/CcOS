# - Try to find the libDBus library
# Once done this will define
#
#  DBus_FOUND - system has libDBus
#  DBus_INCLUDE_DIR - the libDBus include directory
#  DBus_LIBRARIES - Link these to use libDBus
#  DBus_DEFINITIONS - Compiler switches required for using libDBus
#

if (DBus_INCLUDE_DIR AND DBus_LIBRARIES AND DBus_VERSION_OKAY)

  # in cache already
  SET(DBus_FOUND TRUE)

else (DBus_INCLUDE_DIR AND DBus_LIBRARIES AND DBus_VERSION_OKAY)
  if(NOT WIN32)
    # use pkg-config to get the directories and then use these values
    # in the FIND_PATH() and FIND_LIBRARY() calls
    INCLUDE(FindPkgConfig)

    pkg_check_modules(_DBus dbus-1)

    set(DBus_DEFINITIONS ${_DBus_CFLAGS})
  endif(NOT WIN32)

  FIND_PATH(DBus_INCLUDE_DIR dbus/dbus.h
    ${_DBus_INCLUDE_DIRS}
  )

  FIND_PATH(DBus_INCLUDE_DIR_2 dbus/dbus-arch-deps.h
    ${_DBus_INCLUDE_DIRS}
  )

  list(APPEND DBus_INCLUDE_DIRS ${DBus_INCLUDE_DIR} ${DBus_INCLUDE_DIR_2})

  FIND_LIBRARY(DBus_LIBRARIES NAMES dbus-1
    PATHS
    ${_DBus_LIBRARY_DIRS}
  )

  if (DBus_INCLUDE_DIR AND DBus_LIBRARIES)
     set(DBus_FOUND TRUE)
  endif (DBus_INCLUDE_DIR AND DBus_LIBRARIES)

  if (DBus_FOUND)
    if (NOT DBus_FIND_QUIETLY)
      message(STATUS "Found DBus: ${DBus_LIBRARIES}")
    endif (NOT DBus_FIND_QUIETLY)
  else (DBus_FOUND)
    if (DBus_INCLUDE_DIR AND DBus_LIBRARIES)
      message(STATUS "Found DBus but version requirements not met")
    endif (DBus_INCLUDE_DIR AND DBus_LIBRARIES)
    if (DBus_FIND_REQUIRED)
      message(FATAL_ERROR "Could NOT find DBus")
    endif (DBus_FIND_REQUIRED)
  endif (DBus_FOUND)

  MARK_AS_ADVANCED(DBus_INCLUDE_DIR DBus_LIBRARIES DBus_VERSION_OKAY)

endif (DBus_INCLUDE_DIR AND DBus_LIBRARIES AND DBus_VERSION_OKAY)
