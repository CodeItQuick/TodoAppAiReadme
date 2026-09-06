add_test(NAME todo_prints_gh_command
    COMMAND todo ${CMAKE_CURRENT_SOURCE_DIR}/fixtures/todo-ok.md
                 ${CMAKE_CURRENT_SOURCE_DIR}/fixtures/jira-ok.md)
set_tests_properties(todo_prints_gh_command PROPERTIES
    PASS_REGULAR_EXPRESSION "gh issue create --title \"Buy milk\" --body \"Two litres, semi skimmed.\"")

add_test(NAME todo_stops_without_a_story
    COMMAND todo ${CMAKE_CURRENT_SOURCE_DIR}/fixtures/todo-nomatch.md
                 ${CMAKE_CURRENT_SOURCE_DIR}/fixtures/jira-ok.md)
set_tests_properties(todo_stops_without_a_story PROPERTIES WILL_FAIL TRUE)
