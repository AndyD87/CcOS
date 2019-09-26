set(COMPONENT_SRCS "${CCOS_DIR}/Resources/dummy.c")
set(COMPONENT_ADD_INCLUDEDIRS ".")
if(CCOS_PROJECT)
    list(APPEND COMPONENT_SRCS "main.cpp")
endif()

register_component()

target_link_libraries(${COMPONENT_NAME} "CcKernel" "CcApplicationLib")

