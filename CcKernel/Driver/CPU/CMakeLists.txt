################################################################################
# Check if Modules are required for Applications by reading config
################################################################################

file (GLOB COMMON_SOURCE_FILES
        "Common/*.cpp"
        "Common/*.c" )

add_subdirectory(ST)
add_subdirectory(espressif)

push_up_driver_vars()
