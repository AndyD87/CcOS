# - Try to find the NetworkManager includes
# Once done this will define
#
#  NetworkManager_FOUND - system has libNetworkManager
#  NetworkManager_INCLUDE_DIR - the libNetworkManager include directory
#

if (NetworkManager_INCLUDE_DIR)
  # in cache already
  SET(NetworkManager_FOUND TRUE)
else (NetworkManager_INCLUDE_DIR)
  if(NOT WIN32)
    # use pkg-config to get the directories and then use these values
    # in the FIND_PATH() and FIND_LIBRARY() calls
    INCLUDE(FindPkgConfig)

    pkg_check_modules(_NetworkManager NetworkManager)

    set(NetworkManager_DEFINITIONS ${_NetworkManager_CFLAGS})
  endif(NOT WIN32)

  FIND_PATH(NetworkManager_INCLUDE_DIR NetworkManager/NetworkManager.h
    ${_NetworkManager_INCLUDE_DIRS}
  )

  list(APPEND NetworkManager_INCLUDE_DIRS ${NetworkManager_INCLUDE_DIR})

  if (NetworkManager_INCLUDE_DIR)
    set(NetworkManager_FOUND TRUE)
  endif (NetworkManager_INCLUDE_DIR)
endif (NetworkManager_INCLUDE_DIR)
