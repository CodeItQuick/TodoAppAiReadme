# Prompt log

One row per commit. The prompt is the request that produced it.

| Commit title | Prompt |
| --- | --- |
| Add hello world C++ source and CMake build | Can you give me a simple C++ file with hello world, that is easy to compile and run |
| .d Adopt Arlo's Commit Notation | Why don't we add the belshee commits: https://github.com/RefactoringCombos/ArlosCommitNotation |
| .d Fill in TODO.md and ignore the .idea directory | commit what is in here right now. gitignore the idea items |
| .d Ignore the .memsearch directory | gitignore .memsearch too |
| .t Add a CTest check for the hello output | add a test for hello.cpp in tests/ |
| .d Enable the memsearch plugin for this project | can you install this skill locally to this project? |
| .d Document the build and the memsearch plugin in the README | that sounds awful. I probably just want a link in the readme with a general instruction on how to do it. |
| .d Document the ponytail and simple-english plugins | I want instructions for ponytail and simple-english as well |
| .d Add the writing-process-skills skill | commit what's in here, I just added a writing-process-skills |
| .d Document the writing-process-skills skill in the README | add the writing-process-skills to the readme too |
| .d Add the prompt that installs simple-english into the global CLAUDE.md | add that prompt to the readme |
| .d Make the simple-english install prompt work on any machine | they aren't going to have that cache thing in their own machine right? Can you modify this prompt so it can be run on any machine? |
| .d Shorten the simple-english install prompt | ok still not what I want. The person running this is going to install simple-english, then I want a very simple prompt for them to enter that will naturally grab this installation and put it into their main claude.md file |
| .d Note that ponytail activates through a hook and needs no setup | add that note about ponytail to the readme |
| .d Note that memsearch activates through hooks and needs no setup | do the same for memsearch |
| .d Add the grill-with-docs skill and its two dependencies | I have a /grill-with-docs skill. Add the necessary skill(s) to this project to run this skill, and instructions on how to install into README.md |
| .d Simplify TODO.md to one line | I want my todo's to be way simpler, just [] Add my first todo to the github issues tracker |
| .d Add jira.md with the first story | for the jira.md file I want you to create, put the title as the Add my first todo to the github issue tracker with the description I had from the plan to create this todo for acceptance criteria etc. |
| .d Expand CLAUDE.md with the build, the structure, and the backlog rules | /init |
| .d Add log.md, a table of commit titles and prompts | I want a log written now as I write prompts. Put this log in log.md and have it as a markdown table with commit title, and prompt, nothing more is needed right? |
