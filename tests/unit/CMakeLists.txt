add_executable(todo_core_tests todo_core_tests.cpp)
target_link_libraries(todo_core_tests PRIVATE todo_core)

add_test(NAME todo_core_unit COMMAND todo_core_tests)
