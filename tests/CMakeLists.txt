cmake_minimum_required(VERSION 3.17)

include(Dart)

add_executable(Geo_Init_Test Geo_Init_Test.cpp)
target_link_libraries(Geo_Init_Test PUBLIC gtest_main Graphics)

add_executable(Point_Dist_Test Point_Dist_Test.cpp)
target_link_libraries(Point_Dist_Test PUBLIC gtest_main Graphics)

add_executable(Point_Inter_Test Point_Inter_Test.cpp)
target_link_libraries(Point_Inter_Test PUBLIC gtest_main Graphics)

add_executable(Line_Dist_Test Line_Dist_Test.cpp)
target_link_libraries(Line_Dist_Test PUBLIC gtest_main Graphics)

add_executable(Line_Inter_Test Line_Inter_Test.cpp)
target_link_libraries(Line_Inter_Test PUBLIC gtest_main Graphics)

add_executable(LinSeg_Dist_Test LinSeg_Dist_Test.cpp)
target_link_libraries(LinSeg_Dist_Test PUBLIC gtest_main Graphics)

add_executable(LinSeg_Inter_Test LinSeg_Inter_Test.cpp)
target_link_libraries(LinSeg_Inter_Test PUBLIC gtest_main Graphics)

add_executable(Plane_Dist_Test Plane_Dist_Test.cpp)
target_link_libraries(Plane_Dist_Test PUBLIC gtest_main Graphics)

add_executable(Plane_Inter_Test Plane_Inter_Test.cpp)
target_link_libraries(Plane_Inter_Test PUBLIC gtest_main Graphics)

add_executable(Polygon_Dist_Test Polygon_Dist_Test.cpp)
target_link_libraries(Polygon_Dist_Test PUBLIC gtest_main Graphics)

add_executable(Polygon_Inter_Test Polygon_Inter_Test.cpp)
target_link_libraries(Polygon_Inter_Test PUBLIC gtest_main Graphics)

add_executable(Polyhedron_Dist_Test Polyhedron_Dist_Test.cpp)
target_link_libraries(Polyhedron_Dist_Test PUBLIC gtest_main Graphics)

add_executable(Polyhedron_Inter_Test Polyhedron_Inter_Test.cpp)
target_link_libraries(Polyhedron_Inter_Test PUBLIC gtest_main Graphics)

add_test(
    NAME Geo_Init_Test
    COMMAND Geo_Init_Test
)

add_test(
    NAME Point_Dist_Test
    COMMAND Point_Dist_Test
)

add_test(
    NAME Point_Inter_Test
    COMMAND Point_Inter_Test
)

add_test(
    NAME Line_Dist_Test
    COMMAND Line_Dist_Test
)

add_test(
    NAME Line_Inter_Test
    COMMAND Line_Inter_Test
)

add_test(
    NAME LinSeg_Dist_Test
    COMMAND LinSeg_Dist_Test
)

add_test(
    NAME LinSeg_Inter_Test
    COMMAND LinSeg_Inter_Test
)

add_test(
    NAME Plane_Dist_Test
    COMMAND Plane_Dist_Test
)

add_test(
    NAME Plane_Inter_Test
    COMMAND Plane_Inter_Test
)

add_test(
    NAME Polygon_Dist_Test
    COMMAND Polygon_Dist_Test
)

add_test(
    NAME Polygon_Inter_Test
    COMMAND Polygon_Inter_Test
)

add_test(
    NAME Polyhedron_Dist_Test
    COMMAND Polyhedron_Dist_Test
)

add_test(
    NAME Polyhedron_Inter_Test
    COMMAND Polyhedron_Inter_Test
)
