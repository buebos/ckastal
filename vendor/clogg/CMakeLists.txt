cmake_minimum_required(VERSION 3.12)
project(clogg)

set(CMAKE_SOURCE_DIR ".")

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Set the output directory for the shared library
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

# Add the source file
add_library(clogg SHARED src/clogg.c)

# Set the library output name
set_target_properties(clogg PROPERTIES OUTPUT_NAME "clogg")

# Create the "main" target to build the shared library
add_custom_target(main ALL DEPENDS clogg)