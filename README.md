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

This project uses one repository skill and three Claude Code plugins.

### writing-process-skills

`writing-process-skills` lives in `.claude/skills/`, so it needs no install. Git
tracks it, and it works after a clone. Use it to write a new process skill, or to
revise one. The skill recovers what a past commit did, then writes the rule from
that evidence.

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

### ponytail

ponytail keeps the code minimal. It rejects speculative abstractions, and it
prefers the standard library over a new dependency. Set the level with
`/ponytail lite|full|ultra`. Turn it off with `stop ponytail`.

Repository: https://github.com/DietrichGebert/ponytail

### simple-english

simple-english writes documentation with ASD-STE100 Simplified Technical
English. Use it for the README, for runbooks, and for error messages.

Repository: https://github.com/AminBlg/SimpleEnglish

Run the skill with `/simple-english:simple-english` to rewrite one document.

To apply the rules to every answer instead, copy them into your global
`~/.claude/CLAUDE.md`. This prompt needs no plugin, and it runs on any machine:

```
Fetch
https://raw.githubusercontent.com/AminBlg/SimpleEnglish/main/prompts/system-prompt.md

That file holds the ASD-STE100 rules. Write them into ~/.claude/CLAUDE.md as a
section named "ASD-STE100 Simplified Technical English". Create the file if it
does not exist.

Add a line above the rules that states the scope: the rules govern all prose,
which includes chat replies, commit messages, documents, and error messages.
The rules do not govern code, code comments that quote code, or marketing copy.

If the section exists, replace it. Change nothing else in the file.
```
