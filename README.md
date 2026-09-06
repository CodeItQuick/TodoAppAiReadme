# TodoAppAiReadme

A C++ project. The build uses CMake.

## Build and test

```
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

On Windows with MSYS2, add `-G "MinGW Makefiles"` to the first command.

## Tools

This project uses four repository skills and three Claude Code plugins.

Every repository skill lives in `.claude/skills/`. Git tracks them, so they need
no install. Clone the repository, and the skills work.

### grill-with-docs

`/grill-with-docs` runs a relentless interview that sharpens a plan or a design.
It writes an architecture decision record and a glossary as the interview runs.
Use it before you build a feature, not after.

The skill is a wrapper. It calls two other skills, and this repository carries
all three:

- `grill-with-docs` starts the session.
- `grilling` asks the questions.
- `domain-modeling` writes the decision record and the glossary.

Run `/grill-with-docs`. A copy under `~/.claude/skills/` is not needed.

### writing-process-skills

Use `writing-process-skills` to write a new process skill, or to revise one.
The skill recovers what a past commit did, then writes the rule from that
evidence.

## Plugins

Each plugin installs for your user account, not for this repository. Install a
plugin once, and it works in every project.

To install all three, run these commands in Claude Code:

```
/plugin marketplace add zilliztech/memsearch
/plugin install memsearch@memsearch-plugins

/plugin marketplace add DietrichGebert/ponytail
/plugin install ponytail@ponytail

/plugin marketplace add AminBlg/SimpleEnglish
/plugin install simple-english@simple-english
```

### memsearch

memsearch stores and recalls memory from past sessions.

Repository: https://github.com/zilliztech/memsearch

memsearch needs no further setup. It ships SessionStart, UserPromptSubmit,
Stop, and SessionEnd hooks, so it captures and recalls memory on its own.
Restart Claude Code after the install to activate the hooks.

The plugin writes to a `.memsearch/` directory in the repository root. This
repository ignores that directory, because the memory belongs to one machine.

### ponytail

ponytail keeps the code minimal. It rejects speculative abstractions, and it
prefers the standard library over a new dependency. Set the level with
`/ponytail lite|full|ultra`. Turn it off with `stop ponytail`.

Repository: https://github.com/DietrichGebert/ponytail

ponytail needs no further setup. It ships a SessionStart hook, so it activates
at the start of every session. simple-english ships a skill instead, and a
skill loads only when the task matches it. That difference is the reason why
the next section adds a prompt for simple-english, and none for ponytail.

### simple-english

simple-english writes documentation with ASD-STE100 Simplified Technical
English. Use it for the README, for runbooks, and for error messages.

Repository: https://github.com/AminBlg/SimpleEnglish

Run the skill with `/simple-english:simple-english` to rewrite one document.

After you install the plugin, apply the rules to every answer. Enter this
prompt in Claude Code:

```
Read the simple-english skill, then write its rules into my global CLAUDE.md
so that they apply to every answer in every project.
```
