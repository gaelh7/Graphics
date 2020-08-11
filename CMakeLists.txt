cmake_minimum_required(VERSION 3.17)

project(Graphics VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(GH_DEBUG=1)
endif()
add_compile_definitions(GLM_FORCE_SWIZZLE)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
add_compile_definitions(PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# files
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(GLFW_DIR ${LIB_DIR}/GLFW)
set(GLAD_DIR ${LIB_DIR}/glad)
set(STB_DIR ${LIB_DIR}/stb)
set(GLM_DIR ${LIB_DIR}/glm)
set(FT_DIR ${LIB_DIR}/freetype)
find_package(OpenGL 4.6 REQUIRED)
# file(GLOB SOURCES ${SRC_DIR}/*.cpp)
set(SOURCES ${SRC_DIR}/shader.cpp
            ${SRC_DIR}/geometry.cpp
            ${SRC_DIR}/render.cpp
            ${SRC_DIR}/texture.cpp
            ${SRC_DIR}/camera.cpp
            ${SRC_DIR}/collision.cpp
            ${SRC_DIR}/input.cpp
            ${SRC_DIR}/hash.cpp
            ${SRC_DIR}/text.cpp
            ${SRC_DIR}/window.cpp
    )
file(GLOB HEADERS ${INC_DIR}/Graphics/*.hpp)
# set(HEADERS ${INC_DIR}/Graphics/geometry.hpp ${INC_DIR}/Graphics/render.hpp ${INC_DIR}/Graphics/shader.hpp ${INC_DIR}/Graphics/gmath.hpp)

# include
include_directories(${INC_DIR} ${STB_DIR} ${GLM_DIR} ${FT_DIR}/include)

# target
add_library(Graphics STATIC ${HEADERS} ${SOURCES})
add_executable(main ${SRC_DIR}/practice.cpp)
add_executable(gl ${SRC_DIR}/gl.cpp)
target_link_libraries(gl Graphics)
target_link_libraries(main Graphics)

# tests
enable_testing()
add_subdirectory(${LIB_DIR}/googletest)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)

# dependencies
add_subdirectory(${GLFW_DIR})
add_subdirectory(${GLAD_DIR})
add_subdirectory(${FT_DIR})

target_link_libraries(Graphics glfw ${GLFW_LIBRARIES})
target_link_libraries(Graphics glad)
target_link_libraries(Graphics ${OPENGL_LIBRARIES})
target_link_libraries(Graphics freetype)
target_compile_definitions(Graphics PRIVATE STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(Graphics PRIVATE GLFW_INCLUDE_NONE)
