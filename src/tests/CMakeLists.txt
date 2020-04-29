add_executable(
    test1
    main.cpp
)

target_link_libraries (test1 nosql++)
set_target_properties(test1 PROPERTIES LINKER_LANGUAGE CXX)


add_executable(
    test2
    test2.cpp
)

target_link_libraries (test2 nosql++)
set_target_properties(test2 PROPERTIES LINKER_LANGUAGE CXX)


add_executable(
    test3
    test_rtree.cpp
)

target_link_libraries (test3 nosql++)
set_target_properties(test3 PROPERTIES LINKER_LANGUAGE CXX)

add_executable(
    test4
    test1_string.cpp
)

target_link_libraries (test4 nosql++)
set_target_properties(test4 PROPERTIES LINKER_LANGUAGE CXX)