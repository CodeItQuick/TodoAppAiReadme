// The pure rules of the todo pusher. No file access, and no process launch.

#pragma once

#include <string>
#include <vector>

std::string trim(const std::string &s);

// The todo text is the identity of a todo. An unchecked todo starts with "[]".
// Returns an empty string for a line that holds no unchecked todo.
std::string todoText(const std::string &line);

// Pairing: the story heading text, after the story ID, equals the todo text.
// Returns an empty string for a line that is no story heading.
std::string headingText(const std::string &line);

// The lines under a heading, up to the next heading.
std::string storyBody(const std::vector<std::string> &lines, size_t heading);

// One shell argument, in double quotes.
std::string quote(const std::string &s);

// The number at the end of the issue url that gh prints.
std::string issueNumber(const std::string &ghOutput);