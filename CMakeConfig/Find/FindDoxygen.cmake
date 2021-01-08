
if(WIN32)
  if(EXISTS "C:/Program Files/doxygen/bin/doxygen.exe")
    set(Doxygen_FOUND   TRUE)
    set(Doxygen_BINARY  "C:/Program Files/doxygen/bin/doxygen.exe")
  elseif(EXISTS "C:/Program Files (x86)/doxygen/bin/doxygen.exe")
    set(Doxygen_FOUND   TRUE)
    set(Doxygen_BINARY  "C:/Program Files (x86)/doxygen/bin/doxygen.exe")
  elseif(EXISTS "doxygen.exe")
    set(Doxygen_FOUND   TRUE)
    set(Doxygen_BINARY  "doxygen.exe")
  elseif(EXISTS "${CMAKE_CURRENT_LISTS_DIR}/doxygen.exe")
    set(Doxygen_FOUND   TRUE)
    set(Doxygen_BINARY  "${CMAKE_CURRENT_LISTS_DIR}/doxygen.exe")
  elseif(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/doxygen.exe")
    set(Doxygen_FOUND   TRUE)
    set(Doxygen_BINARY  "${CMAKE_CURRENT_BINARY_DIR}/doxygen.exe")
  endif()
else()
  if(EXISTS "doxygen")
    set(Doxygen_FOUND   TRUE)
    set(Doxygen_BINARY  "doxygen")
  endif()
endif()
