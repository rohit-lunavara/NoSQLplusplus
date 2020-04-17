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
