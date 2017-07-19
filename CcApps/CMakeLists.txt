# @Author Andreas Dirmeier
#
# Add all Applications, in this folder, to build
# Applications must be stored in a seperate Folder, except Backup will be ignored.
# The Applications must have a CMakeLists.txt in its directory.
#############

IF( DEFINED WINDOWS OR DEFINED LINUX)
    ADD_SUBDIRECTORY(CcHash)
    ADD_SUBDIRECTORY(CcSync)
ENDIF()

IF(GENERIC)
    add_subdirectory(CcSTM32F4DiscoveryTest)
endif()
