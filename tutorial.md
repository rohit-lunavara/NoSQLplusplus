# NoSQLplusplus Tutorial

## Compile

We use CMake to compile the library, you should have g++-10 installed on your machine

And please change the following lines in src/CMakeLists

<code>
SET(CMAKE_C_COMPILER /path/to/your/gcc-10)


SET(CMAKE_CXX_COMPILER /path/to/your/g++10)
</code>

And you should be able to compile the library

<code> cd src

cmake .

make</code>

After building it, it will automatically generate a folder lib in the root address of this project, and you can find the dynamic library libnosql++.so in it.

## Test

We already have lots of tests in folder src/tests, to add new test, simply edit CMakeLists.txt in src/tests
and add several lines

<code>
add_executable(

    name-of-your-program
    main-cpp-file
    other-source-files (optional)
)

target_link_libraries (test7 nosql++)
set_target_properties(test7 PROPERTIES LINKER_LANGUAGE CXX)

</code>

Enjoy!


