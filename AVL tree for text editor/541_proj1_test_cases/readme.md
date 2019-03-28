Compile
-------

    make

If the code compiles correctly, then /proj1_tester will be created. A few warnings will also be generated, please ignore them if your code is compiling correctly.

Run
-----

    ./proj1_tester

For more options:

    ./proj1_tester --help

Create a test case
------------------

You can open `test_cases.cc` to see examples. Here is one simple example:

    TEST(test_length_text, one_line_test){
        text_t* text = create_text();
        append_line(text, "foobar");
        ASSERT_EQ(length_text(text), 1);
    }

`test_length_text` and `one_line_test` are just names, the first one is used to group multiple test cases together, thus it will be convenient to run a group of test cases.

To test the result, use `ASSERT_*` macros, if the assertion fails, the test case fails. Here are some basic ASSERT macros:
* ASSERT_TRUE(condition);
* ASSERT_FALSE(condition);
* ASSERT_EQ(val1,val2);
* ASSERT_NE(val1,val2);
* ASSERT_LT(val1,val2);
* ASSERT_LE(val1,val2);
* ASSERT_GT(val1,val2);
* ASSERT_GE(val1,val2);

For complete list, please refer [Assertions](https://github.com/google/googletest/blob/master/googletest/docs/Primer.md#assertions) and [More Assertions](https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#more-assertions) in documents of googletest

Integrate with students' submission
-----------------------------------
`naive_implementation.cc` is a simple implementation.

To test your submission, just remove `naive_implementation.cc` and add your code in. To be compatible with this test framework, you must use `API.h` and follow the class name and function signatures defined in it.

Fuse to a single file
---------------------
    cd fuse/
    sh fuse_code.sh
The script will check correctness after fusing. Two files will be generated in the fuse directory: `API.h` and `proj1_tester.cc`.

To compile with students files, pthread library must be used:

    g++ proj1_tester.cc StudentsImplementaionFiles -lpthread -o proj1_tester

googletest documents
--------------------
[Introduction: Why Google C++ Testing Framework?](https://github.com/google/googletest/blob/master/googletest/docs/Primer.md#introduction-why-google-c-testing-framework)

[Advanced Guide](https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md)
