# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build and test

```
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

On Windows with MSYS2, the first command needs `-G "MinGW Makefiles"`.

To run one test, name it:

```
ctest --test-dir build -R todo_prints_gh_command --output-on-failure
```

## Structure

The build declares one executable target, `todo`, from `src/`. The root
`CMakeLists.txt` calls `enable_testing()` and adds the `tests/` subdirectory.

`src/todo.cpp` reads the first unchecked todo in `TODO.md`, pairs it with a
story in `jira.md`, and builds a `gh issue create` command. With no flag it
prints the command. With `--push` it runs the command and writes the new issue
number into the todo line.

`tests/CMakeLists.txt` holds no test source. It declares two CTest cases that
run the `todo` binary against the files in `tests/fixtures/`, and it matches
the standard output against a regular expression. The tests use fixtures, not
the real `TODO.md`, so a push does not break them. They reach no network,
because the default mode only prints. Add GoogleTest or Catch2 only when a test
calls a function instead.

## The MinGW link workaround

The root `CMakeLists.txt` carries an `if(MINGW)` block that links libstdc++ and
libgcc statically. Do not delete it without a check. This MSYS2 install fails a
dynamic link to libstdc++ with `collect2.exe: error: ld returned 116 exit
status`. A plain C link works, so the fault sits in the C++ runtime library, not
in CMake.

## Backlog

`TODO.md` holds one line per task, in the form `[] <task>`. Keep it that short.

`jira.md` holds the story for a task: the as-a/i-want/so-that lines, the
acceptance criteria, and the notes. A story carries a stable ID, such as
`TODO-1`. The ID also goes in the GitHub issue title, and that ID makes an
issue create idempotent. Read the issues before a create:

```
gh issue list --state all --search "TODO-1 in:title" --json number
```

`TODO.md` and `jira.md` hold the truth. The push runs one way, to GitHub.

## Commit messages

Use Arlo's Commit Notation. Reference:
https://github.com/RefactoringCombos/ArlosCommitNotation

The subject line starts with a risk character, then a change-type letter, then
the description.

Risk characters:

- `.` safe. The change addresses all known and unknown risks.
- `^` validated. The change addresses all known risks.
- `!` risky. Some known risks remain unverified.
- `@` broken. The change carries no risk attestation.

Change-type letters:

- `F` feature. The change alters one aspect of the program behavior.
- `B` bugfix. The change repairs one undesirable program behavior.
- `R` refactoring. The change alters the implementation, not the behavior.
- `D` documentation. The change communicates to the team only.

Write the letter in uppercase for a user-visible change. Write the letter in
lowercase for a change that the user cannot see.

Examples:

- `.r Extract the parser into a function`
- `^F Add the CSV export button`
- `!B Correct the off-by-one error in the pager`

## Prompt log

Every commit gets a row in `log.md`. The row holds the commit title and the
prompt that produced it. Add the row in the same commit.

## Skills in this repository

`.claude/skills/` holds four skills. Git tracks them, so they need no install.

- `grill-with-docs` runs an interview that sharpens a plan. It is a wrapper.
- `grilling` asks the questions for that wrapper.
- `domain-modeling` writes the decision record and the glossary.
- `writing-process-skills` writes or revises a process skill.

The README documents three Claude Code plugins as well: memsearch, ponytail,
and simple-english. Those install per user account, not per repository.