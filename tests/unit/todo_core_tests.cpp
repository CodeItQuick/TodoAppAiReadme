// Unit tests for the pure rules in src/todo_core.cpp.
// An assert is the whole framework. Add a real one when a case needs a fixture.

#include "todo_core.h"

#undef NDEBUG  // Keep the asserts in a release build. They are the test.

#include <cassert>
#include <iostream>

namespace {

void trim_removes_the_space_at_both_ends() {
    assert(trim("  buy milk \t") == "buy milk");
    assert(trim("buy milk") == "buy milk");
    assert(trim("   ") == "");
    assert(trim("") == "");
}

void a_todo_starts_with_an_empty_marker() {
    assert(todoText("[] Buy milk") == "Buy milk");
    assert(todoText("   []   Buy milk   ") == "Buy milk");
}

void a_pushed_todo_holds_no_text() {
    assert(todoText("[#12] Buy milk") == "");
    assert(todoText("[x] Buy milk") == "");
    assert(todoText("Buy milk") == "");
    assert(todoText("# My Awesome Project") == "");
    assert(todoText("[]") == "");
}

void a_heading_drops_the_story_id() {
    assert(headingText("## TODO-1 Buy milk") == "Buy milk");
    assert(headingText("##   TODO-1   Buy milk  ") == "Buy milk");
}

void other_lines_are_no_heading() {
    assert(headingText("# Backlog") == "");
    assert(headingText("### Acceptance criteria") == "");
    assert(headingText("## TODO-1") == "");
    assert(headingText("Buy milk") == "");
    assert(headingText("") == "");
}

void a_body_stops_at_the_next_heading() {
    const std::vector<std::string> lines = {
        "# Backlog", "", "## TODO-1 Buy milk", "", "Two litres.", "", "## TODO-2 Walk the dog",
        "Before noon.",
    };
    assert(storyBody(lines, 2) == "Two litres.");
    assert(storyBody(lines, 6) == "Before noon.");
}

void a_body_is_empty_at_the_end_of_the_file() {
    const std::vector<std::string> lines = {"## TODO-1 Buy milk"};
    assert(storyBody(lines, 0) == "");
}

void a_deeper_heading_stays_in_the_body() {
    const std::vector<std::string> lines = {
        "## TODO-1 Buy milk", "### Acceptance criteria", "- Two litres.",
    };
    assert(storyBody(lines, 0) == "### Acceptance criteria\n- Two litres.");
}

void a_quoted_argument_escapes_a_quote_and_a_backslash() {
    assert(quote("Buy milk") == "\"Buy milk\"");
    assert(quote("Say \"hello\"") == "\"Say \\\"hello\\\"\"");
    assert(quote("C:\\path") == "\"C:\\\\path\"");
    assert(quote("") == "\"\"");
}

void an_issue_number_is_the_last_part_of_the_url() {
    assert(issueNumber("https://github.com/CodeItQuick/TodoAppAiReadme/issues/1\n") == "1");
    assert(issueNumber("  https://github.com/o/r/issues/423  ") == "423");
    assert(issueNumber("gh printed nothing useful") == "");
    assert(issueNumber("") == "");
}

}  // namespace

int main() {
    trim_removes_the_space_at_both_ends();
    a_todo_starts_with_an_empty_marker();
    a_pushed_todo_holds_no_text();
    a_heading_drops_the_story_id();
    other_lines_are_no_heading();
    a_body_stops_at_the_next_heading();
    a_body_is_empty_at_the_end_of_the_file();
    a_deeper_heading_stays_in_the_body();
    a_quoted_argument_escapes_a_quote_and_a_backslash();
    an_issue_number_is_the_last_part_of_the_url();
    std::cout << "all unit tests passed\n";
    return 0;
}