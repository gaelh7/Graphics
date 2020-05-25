cmake_minimum_required(VERSION 3.10.2)

project(Graphics VERSION 1.0)

# flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(GH_DEBUG=1)
endif()
add_compile_definitions(STB_IMAGE_IMPLEMENTATION)
add_compile_definitions(GLM_FORCE_SWIZZLE)
add_compile_definitions(PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# files
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(GLFW_DIR ${LIB_DIR}/GLFW)
set(XTL_DIR ${LIB_DIR}/xtl)
set(GLAD_DIR ${LIB_DIR}/glad)
set(XTENSOR_DIR ${LIB_DIR}/xtensor)
set(STB_DIR ${LIB_DIR}/stb_image)
set(GLM_DIR ${LIB_DIR}/glm)
find_package(OpenGL 3.3 REQUIRED)
# file(GLOB SOURCES ${SRC_DIR}/*.cpp)
set(SOURCES ${SRC_DIR}/shader.cpp ${SRC_DIR}/geometry.cpp ${SRC_DIR}/render.cpp ${SRC_DIR}/texture.cpp ${SRC_DIR}/camera.cpp ${SRC_DIR}/collision.cpp)
file(GLOB HEADERS ${INC_DIR}/*.hpp)
# set(HEADERS ${INC_DIR}/Graphics/geometry.hpp ${INC_DIR}/Graphics/render.hpp ${INC_DIR}/Graphics/shader.hpp ${INC_DIR}/Graphics/gmath.hpp)

# include
include_directories(${INC_DIR} ${STB_DIR} ${GLM_DIR})

# target
add_library(Graphics STATIC ${HEADERS} ${SOURCES})
add_executable(main ${SRC_DIR}/practice.cpp)
add_executable(gl ${SRC_DIR}/gl.cpp)
target_link_libraries(gl Graphics)
target_link_libraries(main Graphics)

# dependencies
add_subdirectory(${GLFW_DIR})
add_subdirectory(${GLAD_DIR})
add_subdirectory(${XTL_DIR})
add_subdirectory(${XTENSOR_DIR})

target_link_libraries(Graphics glfw ${GLFW_LIBRARIES})
target_link_libraries(Graphics glad)
target_link_libraries(Graphics xtl ${XTL_LIBRARIES})
target_link_libraries(Graphics xtensor ${XTENSOR_LIBARIES})
target_link_libraries(Graphics ${OPENGL_LIBRARIES})
