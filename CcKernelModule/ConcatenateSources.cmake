
file (GLOB SOURCE_FILES
      "*.cpp"
      "../CcKernel/Types/*.cpp"
      "../CcKernel/Types/Hash/*.cpp"
      "../CcKernel/Network/CcCommonIps.cpp"
)

file (WRITE ${CONCATENATED_FILE} "")
foreach(SOURCE ${SOURCE_FILES})
  file (READ ${SOURCE} FILE_CONTENT)
  file (APPEND ${CONCATENATED_FILE} "${FILE_CONTENT}")
endforeach()
