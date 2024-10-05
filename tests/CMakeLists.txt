# tests/CMakeLists.txt
# Created by Robin Rowe 2024-10-05
# License Open Source GPL3

set (MODULE_NAME tests)
message("Configuring ${MODULE_NAME} 17 source file(s)")
file(STRINGS sources.cmake SOURCES)
add_executable(${MODULE_NAME} ${SOURCES})
