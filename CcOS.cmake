include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Macros.cmake )

###################
# Include Config File if config is set
###################
if(DEFINED CONFIGFILE)
  include( ${CONFIGFILE} )
else()
  # Use default System Config
  if(DEFINED GENERIC)
    include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Configs/Config.Generic.cmake)
  elseif(DEFINED WIN32)
    set(WINDOWS TRUE)
    include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Configs/Config.Windows.cmake)
  elseif(DEFINED APPLE)
    include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Configs/Config.Apple.cmake)
  else()
    set(LINUX TRUE)
    include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Configs/Config.Linux.cmake)
  endif()
endif()

###################
# Precheck all configurations and load thirdparty if required
###################
include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/ConfigCheck.cmake )

###################
# Add CcOS to compile
# Add all CcLib Objects to build-List:
###################
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcKernel/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcGui/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcNetwork/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcMedia/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcUtil/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcModules/ )
