################################################################################
# Check if Modules are required for Applications by reading config
################################################################################

add_subdirectory(ST)
add_subdirectory(espressif)

file (GLOB COMMON_SOURCE_FILES
        "Common/*.cpp"
        "Common/*.c" )

push_up_driver_vars()
