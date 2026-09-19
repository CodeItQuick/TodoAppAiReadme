///
/// @file
/// @details The pure rules of the todo pusher. No file access, and no process launch.
///
///------------------------------------------------------------------------------------------------------------------///

#ifndef TodoApp_TodoCore_hpp
#define TodoApp_TodoCore_hpp

#include <string>
#include <vector>

namespace TodoApp
{
	namespace Core
	{

		/// @details The text without the spaces, tabs, and line ends at both ends.
		/// @details The text without the spaces, tabs, and line ends at both ends.
		std::string Trim(const std::string& text);

		/// @details The todo text is the identity of a todo. An unchecked todo starts with "[]".
		///   Returns an empty string for a line that holds no unchecked todo.
		std::string TodoText(const std::string& line);

		/// @details Pairing: the story heading text, after the story ID, equals the todo text.
		///   Returns an empty string for a line that is no story heading.
		std::string HeadingText(const std::string& line);

		/// @details The lines under a heading, up to the next heading.
		std::string StoryBody(const std::vector<std::string>& lines, const size_t heading);

		/// @details One shell argument, in double quotes.
		std::string Quote(const std::string& text);

		/// @details The number at the end of the issue url that gh prints.
		std::string IssueNumber(const std::string& ghOutput);

	};	//namespace Core
};	//namespace TodoApp

#endif /* TodoApp_TodoCore_hpp */
