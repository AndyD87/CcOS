########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "RestApiWebframework/*.cpp"
      "RestApiWebframework/*.h"
)

add_to_http_sources(${SOURCE_FILES})
