add_executable(
    test1
    main.cpp
)

target_link_libraries (test1 nosql++)
set_target_properties(test1 PROPERTIES LINKER_LANGUAGE CXX)


add_executable(
    test2
    test_geo.cpp
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

add_executable(
    test5
    test2_string.cpp
)

target_link_libraries (test5 nosql++)
set_target_properties(test5 PROPERTIES LINKER_LANGUAGE CXX)

add_executable(
    test6
    ListTests.cpp
)

target_link_libraries (test6 nosql++)
set_target_properties(test6 PROPERTIES LINKER_LANGUAGE CXX)

add_executable(
    test7
    ListTest2.cpp
)

target_link_libraries (test7 nosql++)
set_target_properties(test7 PROPERTIES LINKER_LANGUAGE CXX)
