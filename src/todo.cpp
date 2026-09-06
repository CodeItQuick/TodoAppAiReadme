// Pushes the first unchecked todo in TODO.md to the GitHub issue tracker.
// Usage: todo <TODO.md> <jira.md> [--push]

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

namespace {

std::string trim(const std::string &s) {
    const std::string spaces = " \t\r\n";
    const auto first = s.find_first_not_of(spaces);
    if (first == std::string::npos) return "";
    return s.substr(first, s.find_last_not_of(spaces) - first + 1);
}

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

// The todo text is the identity of a todo. An unchecked todo starts with "[]".
std::string todoText(const std::string &line) {
    const std::string t = trim(line);
    if (t.rfind("[]", 0) != 0) return "";
    return trim(t.substr(2));
}

// Pairing: the story heading text, after the story ID, equals the todo text.
std::string headingText(const std::string &line) {
    if (line.rfind("## ", 0) != 0) return "";
    const std::string rest = trim(line.substr(3));
    const auto space = rest.find(' ');
    if (space == std::string::npos) return "";
    return trim(rest.substr(space + 1));
}

std::string storyBody(const std::vector<std::string> &lines, size_t heading) {
    std::string body;
    for (size_t i = heading + 1; i < lines.size(); ++i) {
        if (lines[i].rfind("## ", 0) == 0) break;
        body += lines[i];
        body += "\n";
    }
    return trim(body);
}

std::string quote(const std::string &s) {
    std::string out = "\"";
    for (const char c : s) {
        if (c == '"' || c == '\\') out += '\\';
        out += c;
    }
    out += '"';
    return out;
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

std::string issueNumber(const std::string &ghOutput) {
    const std::string url = trim(ghOutput);
    const auto slash = url.find_last_of('/');
    if (slash == std::string::npos) return "";
    return trim(url.substr(slash + 1));
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