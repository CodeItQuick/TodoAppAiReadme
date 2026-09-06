add_test(NAME hello_prints_greeting COMMAND hello)
set_tests_properties(hello_prints_greeting PROPERTIES
    PASS_REGULAR_EXPRESSION "^Hello, World!\n$")
