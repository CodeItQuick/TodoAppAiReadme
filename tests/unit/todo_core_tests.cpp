///
/// @file
/// @details Unit tests for the pure rules in src/todo_core.cpp.
///   An assert is the whole framework. Add a real one when a case needs a fixture.
///
///------------------------------------------------------------------------------------------------------------------///

#include "todo_core.hpp"

#undef NDEBUG  // Keep the asserts in a release build. They are the test.

#include <cassert>
#include <iostream>

using namespace TodoApp::Core;

//--------------------------------------------------------------------------------------------------------------------//

namespace
{

	void TrimRemovesTheSpaceAtBothEnds(void)
	{
		assert("buy milk" == Trim("  buy milk \t"));
		assert("buy milk" == Trim("buy milk"));
		assert("" == Trim("   "));
		assert("" == Trim(""));
	}

	void ATodoStartsWithAnEmptyMarker(void)
	{
		assert("Buy milk" == TodoText("[] Buy milk"));
		assert("Buy milk" == TodoText("   []   Buy milk   "));
	}

	void APushedTodoHoldsNoText(void)
	{
		assert("" == TodoText("[#12] Buy milk"));
		assert("" == TodoText("[x] Buy milk"));
		assert("" == TodoText("Buy milk"));
		assert("" == TodoText("# My Awesome Project"));
		assert("" == TodoText("[]"));
	}

	void AHeadingDropsTheStoryID(void)
	{
		assert("Buy milk" == HeadingText("## TODO-1 Buy milk"));
		assert("Buy milk" == HeadingText("##   TODO-1   Buy milk  "));
	}

	void OtherLinesAreNoHeading(void)
	{
		assert("" == HeadingText("# Backlog"));
		assert("" == HeadingText("### Acceptance criteria"));
		assert("" == HeadingText("## TODO-1"));
		assert("" == HeadingText("Buy milk"));
		assert("" == HeadingText(""));
	}

	void ABodyStopsAtTheNextHeading(void)
	{
		const std::vector<std::string> lines = {
			"# Backlog", "", "## TODO-1 Buy milk", "", "Two litres.", "", "## TODO-2 Walk the dog",
			"Before noon.",
		};
		assert("Two litres." == StoryBody(lines, 2));
		assert("Before noon." == StoryBody(lines, 6));
	}

	void ABodyIsEmptyAtTheEndOfTheFile(void)
	{
		const std::vector<std::string> lines = { "## TODO-1 Buy milk" };
		assert("" == StoryBody(lines, 0));
	}

	void ADeeperHeadingStaysInTheBody(void)
	{
		const std::vector<std::string> lines = {
			"## TODO-1 Buy milk", "### Acceptance criteria", "- Two litres.",
		};
		assert("### Acceptance criteria\n- Two litres." == StoryBody(lines, 0));
	}

	void AQuotedArgumentEscapesAQuoteAndABackslash(void)
	{
		assert("\"Buy milk\"" == Quote("Buy milk"));
		assert("\"Say \\\"hello\\\"\"" == Quote("Say \"hello\""));
		assert("\"C:\\\\path\"" == Quote("C:\\path"));
		assert("\"\"" == Quote(""));
	}

	void AnIssueNumberIsTheLastPartOfTheUrl(void)
	{
		assert("1" == IssueNumber("https://github.com/CodeItQuick/TodoAppAiReadme/issues/1\n"));
		assert("423" == IssueNumber("  https://github.com/o/r/issues/423  "));
		assert("" == IssueNumber("gh printed nothing useful"));
		assert("" == IssueNumber(""));
	}

};	//namespace

//--------------------------------------------------------------------------------------------------------------------//

int main(void)
{
	TrimRemovesTheSpaceAtBothEnds();
	ATodoStartsWithAnEmptyMarker();
	APushedTodoHoldsNoText();
	AHeadingDropsTheStoryID();
	OtherLinesAreNoHeading();
	ABodyStopsAtTheNextHeading();
	ABodyIsEmptyAtTheEndOfTheFile();
	ADeeperHeadingStaysInTheBody();
	AQuotedArgumentEscapesAQuoteAndABackslash();
	AnIssueNumberIsTheLastPartOfTheUrl();

	std::cout << "all unit tests passed\n";
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------//
