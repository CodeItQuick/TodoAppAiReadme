# Anatomy template

Fill this in from the friction log (Process step 3) and the measured numbers
(step 4). Delete any section the skill genuinely does not need — an empty
section is noise, and the validator warns rather than fails for that reason.

Angle brackets mark what to replace. Parenthesised italics are instructions to
you, not text to keep.

---

```markdown
---
name: <directory-name>
description: Use when <the symptom a user would describe>, <the file paths involved>, or <the second trigger>. Also use when <the recovery trigger: the thing that happens after this skill was skipped>.
---

# <Skill Name>

## Overview

<The specific failure this closes, stated as something that already happened
here. Name the file, the commit, or the run.>

**Confirmed baseline:** <the number a script produced against real repo content,
and what it implies about the default state — e.g. "assume it's present until
measured otherwise">.

<Any format facts a reader needs before the rules make sense: the schema, the
answer shape, which fields are live.>

## When to Use

- <A phrase the user would actually type>
- <A condition you would notice yourself>
- <The state of a file that calls for this>

Not for: <the neighbouring case this skill must not absorb>.

## The seam with `<other-skill>`

| This skill | `<other-skill>` |
|---|---|
| <the question this one answers> | <the question that one answers> |
| <its evidence> | <its evidence> |
| <its scope> | <its scope> |
| <flags or rewrites?> | <flags or rewrites?> |

(*Add the mirrored table to the other skill in the same commit.*)

## Scope

**In, in priority order:** <targets>.

**Out permanently:** <target> (<the reason it can never qualify>).

**Deferred pending <condition>:** <targets>.

(*Date any decision that moved a target between these lists, and give the reason
that bounds it. A dated line survives being questioned six months later.*)

## Invariants

Check with `node .claude/skills/<name>/scripts/<validator>.mjs <target>`.

- **<The invariant, in bold, as a claim.>** <Why it breaks silently, and what
  downstream thing depends on it.>
- **<Next.>** <Why.>

(*Every entry needs its why. An invariant with no consequence attached gets
traded away the first time it is inconvenient.*)

## Process

Work **<one unit> at a time**. <The reason that unit size is right — what goes
wrong at two.>

### 1. <Read the current state>

<Which file holds it, and what field says what this run is allowed to do.>

### 2. <Do the work>

<The reference file holding the detail, so it stays out of SKILL.md.>

### 3. <Record it>

<What gets written, and where.>

### 4. Validate and commit

```bash
node .claude/skills/<name>/scripts/<validator>.mjs <target>
```

### 5. Stop

<What to report.> **Do not <the mutation this skill must not perform
unprompted>.**

The default outcome of this skill is <the artifact> and <the list>, nothing
else.

### 6. <Apply> (only when asked)

<One file per invocation, prompting between files.> Then:

```bash
cd domain && npm test -- --run && npm run build && npm run lint
```

## Common Mistakes

- **<The mistake, in bold.>** <Why it was tempting, and what it costs.>
- **<The one you actually made while extracting this skill.>** <Its cost.>
```

---

## Notes on filling it in

- **The `description` is loaded before the body.** Name paths and symptoms;
  never summarise the workflow, or it gets followed instead of the skill.
- **Baselines beat adjectives.** "2x the word count" ends an argument that
  "answers are often too long" starts.
- **The `Stop` step is the whole safety property** for any skill that edits
  files. Write it before the apply step, not after.
- **Reference files carry the detail.** Templates, query shapes and rubrics live
  in `references/`; SKILL.md carries the judgment.
- **Scripts carry the counting.** If a rule in prose could be a number, it
  should be.
