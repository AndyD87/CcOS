set(COMPONENT_SRCS "main.cpp")
set(COMPONENT_ADD_INCLUDEDIRS ".")

register_component()

target_link_libraries(${COMPONENT_NAME} "CcKernel" "CcApplicationLib")
