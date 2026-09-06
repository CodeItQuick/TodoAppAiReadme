# Project instructions

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
