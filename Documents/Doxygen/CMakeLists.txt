################################################################################
# This cmake file is placed to update Dokument version with ccos version.
################################################################################
cmake_minimum_required (VERSION 2.8)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.txt.in" "${CMAKE_CURRENT_SOURCE_DIR}/config.txt.tmp" @ONLY)
CcMoveFile("${CMAKE_CURRENT_SOURCE_DIR}/config.txt.tmp" "${CMAKE_CURRENT_SOURCE_DIR}/config.txt")