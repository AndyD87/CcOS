# @Author Andreas Dirmeier
#
# Add all Applications, in this folder, to build
# Applications must be stored in a seperate Folder, except Backup will be ignored.
# The Applications must have a CMakeLists.txt in its directory.
#############

ADD_SUBDIRECTORY(CcSync)
ADD_SUBDIRECTORY(CcSyncServer)
ADD_SUBDIRECTORY(CcSyncClientCli)
ADD_SUBDIRECTORY(CcSyncClientGui)
ADD_SUBDIRECTORY(CcSyncClientSvc)