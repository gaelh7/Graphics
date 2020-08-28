include(Dart)

function(AddTest NAME)
    set(TNAME ${NAME}_Test)
    add_executable(${TNAME} ${NAME}.cpp)
    target_link_libraries(${TNAME} PUBLIC gtest_main Graphics)
    add_test(
        NAME ${TNAME}
        COMMAND ${TNAME}
    )
endfunction()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/geometry)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rendering)
