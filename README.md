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

This project uses the memsearch plugin for Claude Code. It stores and recalls
memory from past sessions.

Repository: https://github.com/zilliztech/memsearch

To install the plugin, run these commands in Claude Code:

```
/plugin marketplace add zilliztech/memsearch
/plugin install memsearch@memsearch-plugins
```

The plugin installs for your user account, not for this repository. Install it
once, and it works in every project.
