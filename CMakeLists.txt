cmake_minimum_required(VERSION 3.8)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/GLFW)
add_subdirectory(lib/GLM)
add_subdirectory(lib/GLAD)

find_package(OpenGL REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG)
endif()

# Add C++ source files in src directory and its subdirectories
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/**/*.cpp")

# Add header files in include directory and its subdirectories
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/src/include/*.h" "${CMAKE_SOURCE_DIR}/src/include/**/*.h")

if(XCODE)
  add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} ${HEADERS})
else()
  add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
endif()

if(XCODE)
  add_custom_target(
    Copy_Assets ALL
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/copy_assets.py
      ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_SOURCE_DIR}/bin/LearnOpenGL.app/Contents/Resources/assets
    COMMENT "Copying assets to app bundle"
    VERBATIM
  )
else()
  add_custom_target(
    Copy_Assets ALL
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/copy_assets.py
      ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_SOURCE_DIR}/bin/assets
    COMMENT "Copying assets to bin directory"
    VERBATIM
  )
endif()

target_link_libraries(LearnOpenGL glfw)
target_link_libraries(LearnOpenGL OpenGL::GL)
target_link_libraries(LearnOpenGL glad)

target_include_directories(LearnOpenGL PRIVATE src/include/)
target_include_directories(LearnOpenGL PRIVATE lib/)
target_include_directories(LearnOpenGL PRIVATE lib/GLFW/)
target_include_directories(LearnOpenGL PRIVATE lib/GLM/)
target_include_directories(LearnOpenGL PRIVATE lib/GLAD/)