# Backlog

A story here pairs with a line in `TODO.md`. The heading text after the story ID
must equal the todo text exactly. The program uses that text to pair them.

## TODO-1 Add my first todo to the github issues tracker

**As a** person with tasks
**I want** a program that puts a todo on the GitHub board
**So that** one board holds the work

### Design

The program is the product. `src/hello.cpp` becomes `src/todo.cpp`, and the
CMake target `hello` becomes `todo`.

Run it with both file paths:

```
todo TODO.md jira.md
```

The program takes the first unchecked line in `TODO.md`. A line is unchecked
when it starts with `[]`. The program then finds the story in `jira.md` whose
heading text, after the story ID, equals the todo text. It builds one
`gh issue create` command from the two: the todo text is the title, and the
story section is the body.

With no flag, the program prints that command and exits. With `--push`, it runs
the command, reads the new issue number, and writes the number back into the
todo line. The line `[] Buy milk` becomes `[#12] Buy milk`.

That number is the whole idempotency mechanism. A line with a number is not
unchecked, so a second run skips it. The check needs no network.

The story is self-hosting. After the program exists, run it against this file.
It files this story as the first issue.

### Acceptance criteria

- `todo TODO.md jira.md` prints the `gh issue create` command, and files
  nothing.
- The printed title is the todo text. The printed body is the story section.
- `todo TODO.md jira.md --push` files the issue in
  `CodeItQuick/TodoAppAiReadme`, and rewrites the line as `[#<number>]`.
- A second `--push` run finds no unchecked line, and files nothing.
- The program stops with an error when no story heading matches the todo text.
  It files nothing in that case.
- A CTest case runs the default mode and matches the printed command. The test
  reaches no network.

### Notes

The old test `hello_prints_greeting` goes away with the greeting.

The commit letter is `F`. The program behavior changes for the person who runs
it.

Out of scope: a second todo in one run, a check for a closed issue, an edit
pushed to an existing issue, and a `.md` parser beyond the two rules above.