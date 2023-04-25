# CMakeLists.txt for GLAD subdirectory

# Add the glad library to the project
add_library(glad STATIC glad.c)

# Set include directories for glad library
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Specify C++ standard
set_property(TARGET glad PROPERTY CXX_STANDARD 11)

# Hide internal symbols of the glad library on Unix-like systems
if(UNIX)
  target_compile_options(glad PRIVATE -fvisibility=hidden)
endif()
