# Backlog

## TODO-1 Add my first todo to the github issue tracker

**As a** person with tasks
**I want** my todo to reach the GitHub issue tracker
**So that** one board holds the work

### Acceptance criteria

- The line in `TODO.md` becomes an issue in `CodeItQuick/TodoAppAiReadme`.
- The issue title starts with the story ID `TODO-1`.
- The issue carries the `enhancement` label. The repository already defines it.
- A second run creates no second issue.

### Notes

The story ID gives the idempotency. A create must read the current issues first,
so a check and an idempotent create are one operation, not two options.

Read the current issues for the ID:

```
gh issue list --state all --search "TODO-1 in:title" --json number
```

If the output is `[]`, create the issue. If the output names an issue, stop.

`TODO.md` holds the truth. The push runs one way, from the file to GitHub. Git
then reviews a backlog change in a pull request.

This story adds no sync script. Two `gh` commands cover one story. Write a
script at the third story.