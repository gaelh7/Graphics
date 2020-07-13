cmake_minimum_required(VERSION 3.17)

include(Dart)

add_executable(Geo_Init_Test Geo_Init_Test.cpp)
target_link_libraries(Geo_Init_Test PUBLIC gtest_main Graphics)

add_executable(Point_Dist_Test Point_Dist_Test.cpp)
target_link_libraries(Point_Dist_Test PUBLIC gtest_main Graphics)

add_executable(Line_Dist_Test Line_Dist_Test.cpp)
target_link_libraries(Line_Dist_Test PUBLIC gtest_main Graphics)

add_executable(LinSeg_Dist_Test LinSeg_Dist_Test.cpp)
target_link_libraries(LinSeg_Dist_Test PUBLIC gtest_main Graphics)

add_test(
    NAME Geo_Init_Test
    COMMAND Geo_Init_Test
)

add_test(
    NAME Point_Dist_Test
    COMMAND Point_Dist_Test
)

add_test(
    NAME Line_Dist_Test
    COMMAND Line_Dist_Test
)

add_test(
    NAME LinSeg_Dist_Test
    COMMAND LinSeg_Dist_Test
)
