list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

incldue(CcMacros.cmake)
incldue(CcOS.config.cmake)

foreach(comp ${CcOS_FIND_COMPONENTS})
  if(NOT TARGET CcOS::${comp})
    find_package(${comp})
  endif()
endforeach()
