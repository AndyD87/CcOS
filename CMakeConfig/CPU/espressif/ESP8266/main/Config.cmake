set(COMPONENT_SRCS "${CCOS_DIR}/Resources/dummy.c")
set(COMPONENT_LIBS "CcKernel")
set(COMPONENT_ADD_INCLUDEDIRS ".")
if(CCOS_PROJECT)
  list(APPEND COMPONENT_SRCS "main.cpp")
else()
  list(APPEND COMPONENT_LIBS "CcApplicationLib")
endif()

register_component()

target_link_libraries(${COMPONENT_NAME} ${COMPONENT_LIBS})

