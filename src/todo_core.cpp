///
/// @file
/// @details The pure rules of the todo pusher. No file access, and no process launch.
///
///------------------------------------------------------------------------------------------------------------------///

#include "todo_core.hpp"

//--------------------------------------------------------------------------------------------------------------------//

std::string TodoApp::Core::Trim(const std::string& text)
{
	const std::string kSpaces = " \t\r\n";
	const size_t first = text.find_first_not_of(kSpaces);
	if (std::string::npos == first)
	{
		return "";
	}

	return text.substr(first, text.find_last_not_of(kSpaces) - first + 1);
}

//--------------------------------------------------------------------------------------------------------------------//

std::string TodoApp::Core::TodoText(const std::string& line)
{
	const std::string trimmed = Trim(line);
	if (0 != trimmed.rfind("[]", 0))
	{
		return "";
	}

	return Trim(trimmed.substr(2));
}

//--------------------------------------------------------------------------------------------------------------------//

std::string TodoApp::Core::HeadingText(const std::string& line)
{
	if (0 != line.rfind("## ", 0))
	{
		return "";
	}

	const std::string rest = Trim(line.substr(3));
	const size_t space = rest.find(' ');
	if (std::string::npos == space)
	{
		return "";
	}

	return Trim(rest.substr(space + 1));
}

//--------------------------------------------------------------------------------------------------------------------//

std::string TodoApp::Core::StoryBody(const std::vector<std::string>& lines, const size_t heading)
{
	std::string body;
	for (size_t lineIndex = heading + 1; lineIndex < lines.size(); ++lineIndex)
	{
		if (0 == lines[lineIndex].rfind("## ", 0))
		{
			break;
		}

		body += lines[lineIndex];
		body += "\n";
	}

	return Trim(body);
}

//--------------------------------------------------------------------------------------------------------------------//

std::string TodoApp::Core::Quote(const std::string& text)
{
	std::string quoted = "\"";
	for (const char character : text)
	{
		if ('"' == character || '\\' == character)
		{
			quoted += '\\';
		}
		quoted += character;
	}

	quoted += '"';
	return quoted;
}

//--------------------------------------------------------------------------------------------------------------------//

std::string TodoApp::Core::IssueNumber(const std::string& ghOutput)
{
	const std::string url = Trim(ghOutput);
	const size_t slash = url.find_last_of('/');
	if (std::string::npos == slash)
	{
		return "";
	}

	return Trim(url.substr(slash + 1));
}

//--------------------------------------------------------------------------------------------------------------------//
