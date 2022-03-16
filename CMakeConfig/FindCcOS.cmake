

foreach(comp ${CcOS_FIND_COMPONENTS})
    if(TARGET CcOS::${comp})
         find_package(${comp})
    endif()
endif()
