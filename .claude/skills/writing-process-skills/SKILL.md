---
name: writing-process-skills
description: Use when authoring a new process skill in .claude/skills/, revising an existing one, or when a task in this repo has been performed enough times that the approach should be written down rather than re-derived. Also use when a skill was written and the next run still needed decisions it failed to cover.
---

# Writing Process Skills

## Overview

A process skill written from memory encodes the process you *imagine* you
follow. The evidence is in this repo: `frontend/src/quizzes/CLAUDE.md` documents
adding a quiz in four steps and three required files. The commit that actually
added one (`dfefe54`) touched a fourth file the doc never mentions, broke an
existing lobby test that appears in no checklist, and left the entire domain half
to a separate commit. The doc was not wrong when written. It was written from
recollection instead of from the diff.

So this skill inverts the order: **recover what happened, then write the rule.**
The unit of evidence is a `git show --stat` and a log of the points where you had
to stop and decide, not a remembered workflow.

**Confirmed baseline:** the three skills already in `.claude/skills/` converged
independently on the same anatomy — 3/3 carry `Overview`, `When to Use` with an
explicit `Not for:`, `Process`, and `Common Mistakes`; 2/3 carry a seam table, an
invariants section opening with a validator command, and an explicit `Stop`. That
convergence is what this skill encodes. It was measured, not designed.

**On `superpowers:writing-skills`.** It covers the same activity and this repo
deliberately departs from it on three points. It teaches RED-GREEN-REFACTOR
against baseline subagents; this skill extracts from a completed run instead. It
says project-specific conventions belong in the instructions file rather than a
skill; this repo puts them in skills, because `.claude/skills/` is loaded on
demand and `CLAUDE.md` is loaded always. It says to skip prose wherever a rule is
enforceable by regex; this repo ships *both*, and the scripts are the reason the
skills hold. Those are decisions, not oversights. Use `writing-skills` for a
skill meant to travel to other projects; use this one for a skill about this
repo.

## When to Use

- Authoring a new skill under `.claude/skills/`
- A task here has been done three or more times and keeps costing the same decisions
- A skill exists but the last run still hit a question it did not answer
- Deciding whether something belongs in a skill, in `CLAUDE.md`, or in a script

Not for: one-off tasks, a workflow performed once, or a rule that a linter or
type-checker already enforces. A skill that restates `eslint` is overhead.

## The seam with `superpowers:writing-skills`

| This skill | `superpowers:writing-skills` |
|---|---|
| Skills about *this* repo, in `.claude/skills/` | Portable skills, in `~/.claude/skills/` |
| Evidence is a completed run and its diff | Evidence is a subagent's baseline failure |
| Ships prose *and* a measuring script | Ships prose only where a script cannot |
| Extraction, then writing | RED-GREEN-REFACTOR |

Both are installed. Neither supersedes the other.

## Scope

**In:** process skills under `.claude/skills/` describing work done in this
repository.

**Out:** portable technique skills, and anything already enforced mechanically.

**Deferred:** `writing-house-plans`, a skill for implementation plans in this
repo's own style, to replace reaching for `superpowers:writing-plans`. Deferred
deliberately on 2026-08-29: plan-writing and skill-writing are different
activities, and folding them into one file would produce a skill that triggers on
both and is precise about neither. Generate it with this skill once this one has
been used twice.

## Invariants

Check with
`node .claude/skills/writing-process-skills/scripts/validate-skill.mjs .claude/skills/<name>`.

- **`name` matches the directory.** The loader addresses a skill by directory;
  a mismatch produces a skill that cannot be invoked by the name it announces.
- **`description` starts with `Use when` and names triggering conditions only.**
  It is the only part of the skill loaded before the decision to load the rest,
  so it must describe *when*, never *how*. A description that summarises the
  workflow gets followed instead of the skill.
- **Every script named in prose exists on disk.** A skill quoting a validator
  command that 404s is worse than one with no validator, because the reader
  assumes the gate ran.
- **A rule that can be counted ships as a script.** Prose is for judgment calls.
  This is the house rule the other three all follow and the one most often
  skipped under time pressure.
- **No rule without an observed failure behind it.** If you cannot name the run
  where it went wrong, it is a preference, not an invariant.

The validator hard-fails on the four universal sections, the frontmatter rules
and missing scripts. Seam, Scope, Invariants and Stop are reported as warnings,
because `sharpening-quiz-questions` legitimately lacks some of them. Warnings are
prompts to look, not defects.

## The house anatomy

| Section | What it carries | Required |
|---|---|---|
| `## Overview` | The specific failure mode, plus a measured baseline where one exists | Yes |
| `## When to Use` | Phrases a user would really say, plus an explicit `Not for:` | Yes |
| `## The seam with <skill>` | Two columns splitting ownership with any overlapping skill | Where overlap exists |
| `## Scope` | In / out / deferred, with dated decisions and the reason bounding them | Where scope is contested |
| `## Invariants` | What breaks silently, each with its *why*, opening with the validator command | Where the skill mutates files |
| `## Process` | Numbered, one unit at a time, with the reason for that unit size | Yes |
| `### N. Stop` | The halt between diagnosis and mutation | Where the skill mutates files |
| `## Common Mistakes` | Only observed failures, each with why it was tempting | Yes |

### The four house rules

1. **Measure what is measurable.** Ship a `scripts/*.mjs` gate for any countable
   rule, run it against existing repo content, and record the number in the
   Overview. `sharpening-quiz-questions` says "`c` and `d` averaging 2x the word
   count… assume it's present until measured otherwise." That sentence is worth
   more than a page of advice because a number cannot be argued with.
2. **Flag, then stop.** Diagnosis and mutation are separate invocations.
   `research-backed-questions` gives the reason: "a proposed rewrite arriving
   alongside its own justification gets far less scrutiny than one that arrives
   after the justification has been read and accepted."
3. **One unit per invocation**, prompting between units, with the unit named —
   one factor, one area, one file, one quiz. "Nine factors researched in parallel
   produce nine shallow notes."
4. **Name the seam.** A skill overlapping an existing one adds a seam table to
   *both* files. A seam documented on one side only gets read from the other side
   as an invitation.

## Process

One skill per invocation. Two skills drafted together share their evidence and
both come out generic.

### 1. Establish the task was done three times

Fewer, and the file encodes one instance rather than a pattern. Find the runs:

```bash
git log --oneline --all -- <path touched by the task>
```

If there are not three, stop and say so. The right output is "not yet", not a
speculative skill.

### 2. Recover the real file list

```bash
git show --stat <commit>
```

for at least two completed instances. The intersection is the required set; the
difference is the per-instance variation. Do not take the file list from an
existing doc — checking the doc against the diff is how the gap gets found.

### 3. Do one more instance, logging friction

Perform the task. Log every point where you re-decided, backed out, or found a
file nothing told you about. This log is the entire raw material. A rule with no
log entry behind it does not go in.

### 4. Sort the log

Split each entry into **measurable** or **judgment**. Write a script for each
measurable one, run it against existing content, and keep the number. Judgment
entries become prose. An entry that is neither is usually a preference — cut it.

### 5. Write the skill

Fill `references/anatomy-template.md`. Rules come only from step 3's log and
step 4's numbers.

### 6. Validate and stop

```bash
node .claude/skills/writing-process-skills/scripts/validate-skill.mjs .claude/skills/<name>
```

Commit the skill. **Report what was written and stop here.** Do not use the new
skill in the same invocation that authored it — a skill read back by the session
that just wrote it is being checked against intent, not against the file.

### 7. Prove it (only when asked, in a fresh run)

Run the skill on a real instance of its task. Every decision it failed to cover
becomes a `Common Mistakes` entry. A skill that has never been run is a draft.

## Common Mistakes

- **Writing the workflow before the extraction.** Produces the process you
  imagine you follow. This is the failure `frontend/src/quizzes/CLAUDE.md`
  already demonstrates, and it is the reason this skill exists.
- **A generic `description`.** "Use when working with quizzes" matches
  everything and therefore fires on nothing useful. Name the file paths and the
  symptom, as all three existing skills do.
- **Summarising the workflow in the `description`.** It is loaded before the
  skill body, so a summary there gets followed *instead* of the skill.
- **Documenting a rule a script could check.** The rule drifts, the prose does
  not, and nobody notices. If it is countable, count it.
- **Omitting the `Stop` step.** A skill that mutates on its first invocation
  hands you a rewrite and its justification at the same moment, which is exactly
  when justification gets the least scrutiny.
- **Copying this anatomy into a skill that does not need it.** A skill with no
  overlapping neighbour needs no seam table; an empty section is noise. The
  validator warns rather than fails for exactly this reason.
- **Using the new skill in the session that wrote it.** You will supply from
  memory every decision the file omits, and conclude it is complete.
