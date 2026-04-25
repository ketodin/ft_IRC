// ─────────────────────────────────────────────────────────────────────────────
// automation-label-issue.js
// Called by: automation-label-issue.yaml (issues: opened, edited)
//
// Reads the [TYPE] prefix from the issue title and applies the matching
// type:* label, removing any stale type:* label if the prefix changed.
// ─────────────────────────────────────────────────────────────────────────────

const { header, step, ok, skip, warn, pass, failExit } = require('./automation-utils');

const TYPE_MAP = {
  TASK:     'type:task',
  FIX:      'type:fix',
  REFACTOR: 'type:refactor',
  STYLE:    'type:style',
  DOCS:     'type:docs',
  TEST:     'type:test',
  CHORE:    'type:chore',
  PERF:     'type:perf',
};

module.exports = async ({ github, context }) => {
  const issue = context.payload.issue;

  header(`automation-label-issue  •  Issue #${issue.number}  •  ${context.payload.action}`);
  step(`Title: "${issue.title}"`);

  // ── Parse [TYPE] prefix ───────────────────────────────────────────────────
  const match = issue.title.match(/^\[([A-Za-z]+)\]/);
  if (!match) {
    skip('Title does not match [TYPE] format — no label applied.');
    return;
  }

  const key      = match[1].toUpperCase();
  const newLabel = TYPE_MAP[key];

  if (!newLabel) {
    warn(`Unknown type "[${match[1]}]" — no matching label defined.`);
    return;
  }

  step(`Detected type: "${key}" → label: "${newLabel}"`);

  // ── Remove stale type:* labels ────────────────────────────────────────────
  const currentNames = issue.labels.map(l => l.name);
  const stale        = currentNames.filter(n => n.startsWith('type:') && n !== newLabel);

  for (const label of stale) {
    await github.rest.issues
      .removeLabel({ ...context.repo, issue_number: issue.number, name: label })
      .catch(() => {});
    step(`Removed stale label "${label}"`);
  }

  // ── Apply new label ───────────────────────────────────────────────────────
  if (currentNames.includes(newLabel)) {
    skip(`"${newLabel}" already present — nothing to do.`);
    pass(`automation-label-issue: no change needed on issue #${issue.number}`);
    return;
  }

  await github.rest.issues.addLabels({
    ...context.repo,
    issue_number: issue.number,
    labels: [newLabel],
  });

  ok(`Applied "${newLabel}" to issue #${issue.number}`);
  pass(`automation-label-issue: labeled issue #${issue.number} as ${newLabel}`);
};
