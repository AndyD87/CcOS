################################################################################
# Create Tests only if we are building CcOS
################################################################################
if("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")

  set(CURRENT_PROJECT CcKernelTest)

  ##############################################################################
  # Add Source Files
  ##############################################################################
  set( SOURCE_FILES
          ${CMAKE_CURRENT_LIST_DIR}/CBaseTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CBaseTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CBoardSupportTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CBoardSupportTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CBufferTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CBufferTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CByteArrayTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CByteArrayTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CEventTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CEventTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CFileTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CFileTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CGenericMallocTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CGenericMallocTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CIniFileTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CIniFileTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CHashTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CHashTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CKernelTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CKernelTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CListTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CListTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CMutexTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CMutexTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CNetworkTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CNetworkTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CPasswordTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CPasswordTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CProcessTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CProcessTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CReferenceCountTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CReferenceCountTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CSharedMemoryTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CSharedMemoryTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CStatusTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CStatusTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CStringListTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CStringListTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CStringTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CStringTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CStringUtilTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CStringUtilTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CSystemTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CSystemTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CThreadTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CThreadTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CTimeTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CTimeTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CUrlTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CUrlTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CUuidTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CUuidTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CVectorTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CVectorTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CVersionTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CVersionTest.h
          ${CMAKE_CURRENT_LIST_DIR}/CWorkerTest.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CWorkerTest.h
          ${CMAKE_CURRENT_LIST_DIR}/main.cpp
  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/Testing")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                CcKernel
                                CcTesting
  )
  add_dependencies( ${CURRENT_PROJECT}
                    CcTesting4Test
  )

  CcAddTest( ${CURRENT_PROJECT} )

  #################
  # Post configuration steps
  #################
  CcOSExeSettings(${CURRENT_PROJECT})
  CcSetFiltersByFolders(${SOURCE_FILES})

endif("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")
