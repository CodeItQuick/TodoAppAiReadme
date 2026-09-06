# Todo

A repository that turns a plain text task list into GitHub issues. The task
list, the story text, and the program that pairs them all live here.

## Language

**Todo**:
One line of work in `TODO.md`. It holds a state marker and the todo text.
_Avoid_: Task, item, ticket

**Todo text**:
The words of a todo, after the state marker. It is the identity of the todo, so
an edit to it makes a different todo.
_Avoid_: Description, name

**Unchecked**:
The state of a todo that holds no issue number. The marker is `[]`. Only an
unchecked todo can reach the board.
_Avoid_: Open, pending, new

**Story**:
One section of `jira.md`. It holds the reason for a todo, the acceptance
criteria, and the design. It becomes the body of an issue.
_Avoid_: Spec, requirement, description

**Story ID**:
The label at the front of a story heading, such as `TODO-1`. It names the story
for a person. It takes no part in the pairing.
_Avoid_: Key, ticket number

**Pairing**:
The rule that joins one todo to one story. The story heading text, after the
story ID, must equal the todo text exactly.
_Avoid_: Matching, linking, mapping

**Push**:
The act of creating a GitHub issue from a paired todo and story, then writing
the issue number back into the todo.
_Avoid_: Sync, publish, upload

**Issue number**:
The number that GitHub gives a new issue. Written into the todo line as
`[#12]`, it is the record that the todo reached the board.
_Avoid_: ID, reference