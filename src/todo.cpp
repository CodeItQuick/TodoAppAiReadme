///
/// @file
/// @details Pushes the first unchecked todo in TODO.md to the GitHub issue tracker.
///   Usage: todo <TODO.md> <jira.md> [--push]
///
///------------------------------------------------------------------------------------------------------------------///

#include "todo_core.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//--------------------------------------------------------------------------------------------------------------------//

namespace
{

	/// @details Opens a read pipe to a shell command. The C runtime names the call differently on Windows.
	FILE* OpenPipe(const std::string& command)
	{
#ifdef _WIN32
		return _popen(command.c_str(), "r");
#else
		return popen(command.c_str(), "r");
#endif
	}

	/// @details Closes the pipe and returns the exit status of the command.
	int ClosePipe(FILE* const pipe)
	{
#ifdef _WIN32
		return _pclose(pipe);
#else
		return pclose(pipe);
#endif
	}

};	//namespace

//--------------------------------------------------------------------------------------------------------------------//

namespace TodoApp
{
	namespace Utilities
	{

		/// @details Reads a text file into lines. Drops a UTF-8 byte order mark and a carriage return.
		///   Returns false when the file does not open.
		bool ReadLines(const std::string& filePath, std::vector<std::string>& lines);

		/// @details Writes the lines to a text file, one per line. Returns false when the file does not open.
		bool WriteLines(const std::string& filePath, const std::vector<std::string>& lines);

		/// @details Runs a shell command and appends its standard output to output.
		///   Returns false when the command does not start or exits with a failure.
		bool RunAndCapture(const std::string& command, std::string& output);

	};	//namespace Utilities

	/// @details The program. Returns the exit code for main.
	int Main(const int argumentCount, char* argumentValues[]);
};	//namespace TodoApp

//--------------------------------------------------------------------------------------------------------------------//

bool TodoApp::Utilities::ReadLines(const std::string& filePath, std::vector<std::string>& lines)
{
	std::ifstream inputFile(filePath);
	if (false == inputFile.is_open())
	{
		return false;
	}

	std::string line;
	while (std::getline(inputFile, line))
	{
		if (true == lines.empty() && 0 == line.rfind("\xEF\xBB\xBF", 0))
		{
			line = line.substr(3);
		}
		if (false == line.empty() && '\r' == line.back())
		{
			line.pop_back();
		}
		lines.push_back(line);
	}

	inputFile.close();
	return true;
}

//--------------------------------------------------------------------------------------------------------------------//

bool TodoApp::Utilities::WriteLines(const std::string& filePath, const std::vector<std::string>& lines)
{
	std::ofstream outputFile(filePath);
	if (false == outputFile.is_open())
	{
		return false;
	}

	for (const std::string& line : lines)
	{
		outputFile << line << "\n";
	}

	outputFile.close();
	return true;
}

//--------------------------------------------------------------------------------------------------------------------//

bool TodoApp::Utilities::RunAndCapture(const std::string& command, std::string& output)
{
	FILE* const pipe = OpenPipe(command);
	if (nullptr == pipe)
	{
		return false;
	}

	char buffer[256] = {};
	while (nullptr != fgets(buffer, sizeof buffer, pipe))
	{
		output += buffer;
	}

	return (0 == ClosePipe(pipe));
}

//--------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------//

int TodoApp::Main(const int argumentCount, char* argumentValues[])
{
	if (argumentCount < 3)
	{
		std::cerr << "usage: todo <TODO.md> <jira.md> [--push]\n";
		return 1;
	}

	const std::string todoPath = argumentValues[1];
	const std::string jiraPath = argumentValues[2];
	const bool push = (argumentCount > 3 && std::string("--push") == argumentValues[3]);

	std::vector<std::string> todos;
	if (false == Utilities::ReadLines(todoPath, todos))
	{
		std::cerr << "cannot read " << todoPath << "\n";
		return 1;
	}

	size_t todoLine = todos.size();
	std::string text;
	for (size_t todoIndex = 0; todoIndex < todos.size(); ++todoIndex)
	{
		text = Core::TodoText(todos[todoIndex]);
		if (false == text.empty())
		{
			todoLine = todoIndex;
			break;
		}
	}

	if (todos.size() == todoLine)
	{
		std::cout << "no unchecked todo in " << todoPath << "\n";
		return 0;
	}

	std::vector<std::string> stories;
	if (false == Utilities::ReadLines(jiraPath, stories))
	{
		std::cerr << "cannot read " << jiraPath << "\n";
		return 1;
	}

	size_t heading = stories.size();
	for (size_t storyIndex = 0; storyIndex < stories.size(); ++storyIndex)
	{
		if (text == Core::HeadingText(stories[storyIndex]))
		{
			heading = storyIndex;
			break;
		}
	}

	if (stories.size() == heading)
	{
		std::cerr << "no story in " << jiraPath << " for todo: " << text << "\n";
		return 1;
	}

	const std::string body = Core::StoryBody(stories, heading);

	if (false == push)
	{
		std::cout << "gh issue create --title " << Core::Quote(text) << " --body " << Core::Quote(body) << "\n";
		return 0;
	}

	const std::string kBodyPath = "todo-body.tmp";
	std::ofstream(kBodyPath) << body << "\n";

	const std::string ghCommand = "gh issue create --title " + Core::Quote(text) + " --body-file " + kBodyPath;
	std::string ghOutput;
	const bool ghSucceeded = Utilities::RunAndCapture(ghCommand, ghOutput);
	std::remove(kBodyPath.c_str());
	if (false == ghSucceeded)
	{
		std::cerr << "gh failed: " << ghOutput << "\n";
		return 1;
	}

	const std::string number = Core::IssueNumber(ghOutput);
	if (true == number.empty())
	{
		std::cerr << "gh printed no issue url\n";
		return 1;
	}

	const size_t marker = todos[todoLine].find("[]");
	todos[todoLine].replace(marker, 2, "[#" + number + "]");
	if (false == Utilities::WriteLines(todoPath, todos))
	{
		std::cerr << "cannot write " << todoPath << "\n";
		return 1;
	}

	std::cout << "filed issue #" << number << "\n";
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------//

int main(const int argumentCount, char* argumentValues[])
{
	return TodoApp::Main(argumentCount, argumentValues);
}

//--------------------------------------------------------------------------------------------------------------------//
