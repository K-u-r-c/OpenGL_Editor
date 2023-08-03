cmake_minimum_required(VERSION 3.8)

project(3D-Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/GLFW)
add_subdirectory(lib/GLM)
add_subdirectory(lib/GLAD)
add_subdirectory(modules/feather) # physics module

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# Add C++ source files in src directory and its subdirectories
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/**/*.cpp")

# Add header files in include directory and its subdirectories
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/src/include/*.h" "${CMAKE_SOURCE_DIR}/src/include/**/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

if(XCODE)
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
endif()

# Find Python
find_package(Python COMPONENTS Interpreter)

# Check if Python was found
if (Python_FOUND)
    message(STATUS "Python found: ${Python_EXECUTABLE}")
    set(PYTHON_EXECUTABLE ${Python_EXECUTABLE})
else ()
    message(STATUS "Python not found, trying Python3")
    # Find Python3
    find_package(Python3 COMPONENTS Interpreter)

    # Check if Python3 was found
    if (Python3_FOUND)
        message(STATUS "Python3 found: ${Python3_EXECUTABLE}")
        set(PYTHON_EXECUTABLE ${Python3_EXECUTABLE})
    else ()
        message(STATUS "Python3 not found")
    endif ()
endif ()

set(COPY_TO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/assets)

if(XCODE)
    set(COPY_TO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/${PROJECT_NAME}.app/Contents/Resources/assets)
endif()

add_custom_target(
    Copy_Assets ALL
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/copy_assets.py
    ${CMAKE_CURRENT_SOURCE_DIR}/assets ${COPY_TO_DIR}
    COMMENT "Copying assets to ${COPY_TO_DIR}"
    VERBATIM
)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} feather)

target_include_directories(${PROJECT_NAME} PRIVATE lib/)
target_include_directories(${PROJECT_NAME} PRIVATE lib/GLFW/)
target_include_directories(${PROJECT_NAME} PRIVATE lib/GLM/)
target_include_directories(${PROJECT_NAME} PRIVATE lib/GLAD/)
target_include_directories(${PROJECT_NAME} PRIVATE modules/feather/public/)

add_custom_target(BuildAll
        DEPENDS ${PROJECT_NAME}
        Copy_Assets
        )