# @Author Andreas Dirmeier
#
# Add all Applications, in this folder, to build
# Applications must be stored in a seperate Folder, except Backup will be ignored.
# The Applications must have a CMakeLists.txt in its directory.
#############

IF( DEFINED WINDOWS OR DEFINED LINUX)
    ADD_SUBDIRECTORY(CcSync)
    ADD_SUBDIRECTORY(CcCrc)
    ADD_SUBDIRECTORY(CcMd5)
    ADD_SUBDIRECTORY(CcSha256)
ENDIF()

IF(GENERIC)
    add_subdirectory(CcSTM32F4DiscoveryTest)
endif()
