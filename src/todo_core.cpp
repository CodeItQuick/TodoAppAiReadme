#include "todo_core.h"

std::string trim(const std::string &s) {
    const std::string spaces = " \t\r\n";
    const auto first = s.find_first_not_of(spaces);
    if (first == std::string::npos) return "";
    return s.substr(first, s.find_last_not_of(spaces) - first + 1);
}

std::string todoText(const std::string &line) {
    const std::string t = trim(line);
    if (t.rfind("[]", 0) != 0) return "";
    return trim(t.substr(2));
}

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

std::string issueNumber(const std::string &ghOutput) {
    const std::string url = trim(ghOutput);
    const auto slash = url.find_last_of('/');
    if (slash == std::string::npos) return "";
    return trim(url.substr(slash + 1));
}