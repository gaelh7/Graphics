cmake_minimum_required(VERSION 3.10.2)

project(Graphics VERSION 1.0)

# flags

# files
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(GLFW_DIR ${LIB_DIR}/GLFW)
set(XTL_DIR ${LIB_DIR}/xtl)
set(GLAD_DIR ${LIB_DIR}/glad)
set(XTENSOR_DIR ${LIB_DIR}/xtensor)
find_package(OpenGL 3.3 REQUIRED)
# file(GLOB SOURCES ${SRC_DIR}/*.cpp)
set(SOURCES ${SRC_DIR}/practice.cpp ${SRC_DIR}/geometry.cpp ${SRC_DIR}/render.cpp)

# include
include_directories(${INC_DIR} ${SRC_DIR} ${GLFW_DIR}/include ${GLAD_DIR}/include)

# target
add_executable(main ${SOURCES})
add_executable(gl ${SRC_DIR}/gl.cpp)

# dependencies
add_subdirectory(${GLFW_DIR})
target_link_libraries(main glfw ${GLFW_LIBRARIES})
target_link_libraries(gl glfw ${GLFW_LIBRARIES})

add_subdirectory(${GLAD_DIR})
target_link_libraries(main glad)
target_link_libraries(gl glad)

add_subdirectory(${XTL_DIR})
target_link_libraries(main xtl ${XTL_LIBRARIES})
target_link_libraries(gl xtl ${XTL_LIBRARIES})

add_subdirectory(${XTENSOR_DIR})
target_link_libraries(main xtensor ${XTENSOR_LIBARIES})
target_link_libraries(gl xtensor ${XTENSOR_LIBARIES})


target_link_libraries(main ${OPENGL_LIBRARIES})
target_link_libraries(gl ${OPENGL_LIBRARIES})

