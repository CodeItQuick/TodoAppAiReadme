// Pushes the first unchecked todo in TODO.md to the GitHub issue tracker.
// Usage: todo <TODO.md> <jira.md> [--push]

#include "todo_core.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

namespace {

std::vector<std::string> readLines(const std::string &path) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "cannot read " << path << "\n";
        exit(1);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        if (lines.empty() && line.rfind("\xEF\xBB\xBF", 0) == 0) line = line.substr(3);
        if (!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(line);
    }
    return lines;
}

std::string runAndCapture(const std::string &command) {
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "cannot run gh\n";
        exit(1);
    }
    std::string out;
    char buffer[256];
    while (fgets(buffer, sizeof buffer, pipe)) out += buffer;
    if (pclose(pipe) != 0) {
        std::cerr << "gh failed: " << out << "\n";
        exit(1);
    }
    return out;
}

void writeLines(const std::string &path, const std::vector<std::string> &lines) {
    std::ofstream out(path);
    for (const auto &line : lines) out << line << "\n";
}

}  // namespace

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "usage: todo <TODO.md> <jira.md> [--push]\n";
        return 1;
    }
    const std::string todoPath = argv[1];
    const std::string jiraPath = argv[2];
    const bool push = argc > 3 && std::string(argv[3]) == "--push";

    std::vector<std::string> todos = readLines(todoPath);
    size_t todoLine = todos.size();
    std::string text;
    for (size_t i = 0; i < todos.size(); ++i) {
        text = todoText(todos[i]);
        if (!text.empty()) {
            todoLine = i;
            break;
        }
    }
    if (todoLine == todos.size()) {
        std::cout << "no unchecked todo in " << todoPath << "\n";
        return 0;
    }

    const std::vector<std::string> stories = readLines(jiraPath);
    size_t heading = stories.size();
    for (size_t i = 0; i < stories.size(); ++i) {
        if (headingText(stories[i]) == text) {
            heading = i;
            break;
        }
    }
    if (heading == stories.size()) {
        std::cerr << "no story in " << jiraPath << " for todo: " << text << "\n";
        return 1;
    }
    const std::string body = storyBody(stories, heading);

    if (!push) {
        std::cout << "gh issue create --title " << quote(text) << " --body " << quote(body)
                  << "\n";
        return 0;
    }

    const std::string bodyPath = "todo-body.tmp";
    std::ofstream(bodyPath) << body << "\n";
    const std::string number = issueNumber(
        runAndCapture("gh issue create --title " + quote(text) + " --body-file " + bodyPath));
    std::remove(bodyPath.c_str());
    if (number.empty()) {
        std::cerr << "gh printed no issue url\n";
        return 1;
    }

    const auto marker = todos[todoLine].find("[]");
    todos[todoLine].replace(marker, 2, "[#" + number + "]");
    writeLines(todoPath, todos);
    std::cout << "filed issue #" << number << "\n";
    return 0;
}