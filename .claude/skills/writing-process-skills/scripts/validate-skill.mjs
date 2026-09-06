#!/usr/bin/env node
// Validate a process skill against the house anatomy.
//
//   node validate-skill.mjs .claude/skills/<name> [...more]
//
// Errors (exit 1) are rules all three reference skills already satisfy.
// Warnings (exit 0) are sections that two of three carry: real prompts to
// look, not defects. sharpening-quiz-questions legitimately lacks some.

import { readFileSync, existsSync, readdirSync, statSync } from 'node:fs';
import { basename, join, resolve } from 'node:path';

const REQUIRED_SECTIONS = ['Overview', 'When to Use', 'Process', 'Common Mistakes'];

const EXPECTED_SECTIONS = [
  { test: /^## The seam with /m, label: 'no "## The seam with <skill>" table (omit only if no skill overlaps)' },
  { test: /^## Scope/m, label: 'no "## Scope" section with in/out/deferred' },
  { test: /^## (Invariants|Hard Constraints)/m, label: 'no "## Invariants" section (required once the skill mutates files)' },
  { test: /^#{2,3} .*\bstop\b/im, label: 'no explicit Stop step between diagnosis and mutation' },
];

function parseFrontmatter(text) {
  const match = text.match(/^---\r?\n([\s\S]*?)\r?\n---/);
  if (!match) return null;
  const fields = {};
  // Field values may wrap onto continuation lines; join them.
  let key = null;
  for (const line of match[1].split(/\r?\n/)) {
    const start = line.match(/^([A-Za-z_-]+):\s*(.*)$/);
    if (start) {
      key = start[1];
      fields[key] = start[2];
    } else if (key && line.trim()) {
      fields[key] += ' ' + line.trim();
    }
  }
  return fields;
}

function validate(dir) {
  const errors = [];
  const warnings = [];
  const skillPath = resolve(dir);
  const name = basename(skillPath);
  const skillFile = join(skillPath, 'SKILL.md');

  if (!existsSync(skillFile)) {
    return { name, errors: [`no SKILL.md in ${dir}`], warnings };
  }

  const text = readFileSync(skillFile, 'utf8');
  const front = parseFrontmatter(text);

  if (!front) {
    errors.push('no YAML frontmatter (must open with --- on line 1)');
  } else {
    if (!front.name) errors.push('frontmatter has no `name`');
    else if (front.name !== name) {
      errors.push(`frontmatter name "${front.name}" does not match directory "${name}"`);
    }

    if (!front.description) {
      errors.push('frontmatter has no `description`');
    } else {
      const description = front.description.replace(/^["']|["']$/g, '');
      if (!/^Use when\b/.test(description)) {
        errors.push('`description` must start with "Use when" and name triggering conditions');
      }
      if (description.length < 60) {
        warnings.push('`description` is short; name the file paths and the symptom so it matches a real request');
      }
    }
  }

  const body = text.replace(/^---[\s\S]*?\r?\n---/, '');

  for (const section of REQUIRED_SECTIONS) {
    if (!new RegExp(`^## ${section}\\b`, 'm').test(body)) {
      errors.push(`missing required section "## ${section}"`);
    }
  }

  if (/^## When to Use/m.test(body) && !/Not for:/.test(body)) {
    errors.push('"## When to Use" has no explicit "Not for:" line');
  }

  // Every script named in prose must exist, or the quoted gate never ran.
  // An unqualified `scripts/x.mjs` is ambiguous: skills use it for their own
  // scripts AND for the repo-root scripts/ directory, which is how a reader
  // resolves a path written from the repo root. Accept either, or the check
  // invents a missing file. A reference qualified with a skill directory
  // resolves only there.
  const scriptsDir = join(skillPath, 'scripts');
  const repoScripts = join(skillPath, '..', '..', '..', 'scripts');
  const named = new Map();
  for (const m of body.matchAll(/(?:\.claude\/skills\/([\w.-]+)\/)?scripts\/([\w.-]+\.mjs)/g)) {
    const [reference, owner, script] = m;
    const dirs = owner ? [join(skillPath, '..', owner, 'scripts')] : [scriptsDir, repoScripts];
    named.set(reference, { dirs, script, local: !owner || owner === name });
  }
  for (const [reference, { dirs, script }] of named) {
    if (!dirs.some((dir) => existsSync(join(dir, script)))) {
      errors.push(`prose names ${reference}, which does not exist`);
    }
  }

  for (const { test, label } of EXPECTED_SECTIONS) {
    if (!test.test(body)) warnings.push(label);
  }

  const localNamed = [...named.values()].filter((n) => n.local);
  if (existsSync(scriptsDir) && localNamed.length === 0) {
    warnings.push('has a scripts/ directory that no prose references');
  }
  if (!existsSync(scriptsDir)) {
    warnings.push('ships no measuring script; confirm every rule here is a judgment call');
  }

  return { name, errors, warnings };
}

const args = process.argv.slice(2);
const targets = args.length
  ? args
  : readdirSync('.claude/skills')
      .map((d) => join('.claude/skills', d))
      .filter((p) => statSync(p).isDirectory());

let failed = 0;
for (const target of targets) {
  const { name, errors, warnings } = validate(target);
  if (errors.length === 0 && warnings.length === 0) {
    console.log(`PASS  ${name}`);
  } else if (errors.length === 0) {
    console.log(`PASS  ${name}  (${warnings.length} warning${warnings.length > 1 ? 's' : ''})`);
  } else {
    failed++;
    console.log(`FAIL  ${name}`);
  }
  for (const error of errors) console.log(`  error:   ${error}`);
  for (const warning of warnings) console.log(`  warning: ${warning}`);
}

process.exit(failed > 0 ? 1 : 0);
