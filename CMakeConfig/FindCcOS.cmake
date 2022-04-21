list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

foreach(comp ${CcOS_FIND_COMPONENTS})
  if(NOT TARGET CcOS::${comp})
    find_package(${comp})
  endif()
endforeach()
