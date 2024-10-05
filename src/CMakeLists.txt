# src/CMakeLists.txt
# Created by Robin Rowe 2024-10-05
# License Open Source GPL3

set (MODULE_NAME libbeetle)
message("Configuring ${MODULE_NAME} 14 source file(s)")
file(STRINGS sources.cmake SOURCES)
add_library(${MODULE_NAME} ${SOURCES})
