#########
# Cmake script for signing files with wdk signtool
# Required Parameters
# - SIGN_WORKING_DIRECTORY
# - WDK_SIGNTOOL_FILE # Full path to signtool.exe
# - WDK_TESTCERT_NAME # Zertificate in PrivateCertStore
# - SIGN_FILE         # Target file to sing
#
###

execute_process(
  COMMAND ${WDK_SIGNTOOL_FILE} sign /a /v /s PrivateCertStore /n ${WDK_TESTCERT_NAME} /t http://timestamp.digicert2.com ${SIGN_FILE}
  WORKING_DIRECTORY ${SIGN_WORKING_DIRECTORY}
  RESULT_VARIABLE PROC_RESULT
  ERROR_QUIET
)

if(NOT ${PROC_RESULT} EQUAL 0)
  message(WARNING "COMMAND ${WDK_SIGNTOOL_FILE} sign /a /v /s PrivateCertStore /n ${WDK_TESTCERT_NAME} /t http://timestamp.digicert.com ${SIGN_FILE}")
endif()
