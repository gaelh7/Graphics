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
set(IMG_DIR ${LIB_DIR}/imgui)
set(ASMP_DIR ${LIB_DIR}/assimp)
# file(GLOB SOURCES ${SRC_DIR}/*.cpp)
set(SOURCES ${SRC_DIR}/shader.cpp
            ${SRC_DIR}/geometry.cpp
            ${SRC_DIR}/georender.cpp
            ${SRC_DIR}/texture.cpp
            ${SRC_DIR}/camera.cpp
            ${SRC_DIR}/collision.cpp
            ${SRC_DIR}/input.cpp
            ${SRC_DIR}/hash.cpp
            ${SRC_DIR}/text.cpp
            ${SRC_DIR}/window.cpp
            ${SRC_DIR}/model.cpp
    )
file(GLOB HEADERS ${INC_DIR}/Graphics/*.hpp)
# set(HEADERS ${INC_DIR}/Graphics/geometry.hpp ${INC_DIR}/Graphics/render.hpp ${INC_DIR}/Graphics/shader.hpp ${INC_DIR}/Graphics/gmath.hpp)

# include

# target
add_library(Graphics STATIC ${HEADERS} ${SOURCES})
add_executable(main ${SRC_DIR}/practice.cpp)
add_executable(gl ${SRC_DIR}/gl.cpp)
target_link_libraries(gl Graphics)
target_link_libraries(main Graphics)

target_include_directories(Graphics PUBLIC ${INC_DIR} ${STB_DIR} ${GLM_DIR} ${FT_DIR}/include ${IMG_DIR})

# target_compile_options(Graphics PRIVATE
#   $<$<CXX_COMPILER_ID:MSVC>:/W4>
#   $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
# )

# tests
enable_testing()
add_subdirectory(${LIB_DIR}/googletest)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)

# dependencies
add_subdirectory(${GLFW_DIR})
add_subdirectory(${GLAD_DIR})
add_subdirectory(${FT_DIR})
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${ASMP_DIR})
target_sources(Graphics PRIVATE
    ${IMG_DIR}/imgui.h
    ${IMG_DIR}/imgui.cpp
    ${IMG_DIR}/imgui_draw.cpp
    ${IMG_DIR}/imgui_widgets.cpp
    ${IMG_DIR}/examples/imgui_impl_glfw.h
    ${IMG_DIR}/examples/imgui_impl_glfw.cpp
    ${IMG_DIR}/examples/imgui_impl_opengl3.h
    ${IMG_DIR}/examples/imgui_impl_opengl3.cpp
)

target_link_libraries(Graphics glfw ${GLFW_LIBRARIES})
target_link_libraries(Graphics glad)
target_link_libraries(Graphics freetype)
target_link_libraries(Graphics assimp ${ASSIMP_LIBRARIES})
target_compile_definitions(Graphics PRIVATE GLFW_INCLUDE_NONE)
