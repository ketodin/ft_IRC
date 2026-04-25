// ─────────────────────────────────────────────────────────────────────────────
// automation-label-pr.js
// Called by: automation-label-pr.yaml (pull_request: opened, edited)
//
// 1. Reads the type/ prefix from the PR title → applies type:* label.
// 2. Extracts the linked issue number from the PR title → copies any
//    priority:* labels from that issue to the PR.
//
// Both operations run sequentially in one script to prevent concurrent
// label-write conflicts.
// ─────────────────────────────────────────────────────────────────────────────

const {
  header, section, step, ok, skip, warn, pass,
  extractIssueNumber, getIssue,
} = require('./automation-utils');

const TYPE_MAP = {
  task:     'type:task',
  fix:      'type:fix',
  refactor: 'type:refactor',
  style:    'type:style',
  docs:     'type:docs',
  test:     'type:test',
  chore:    'type:chore',
  perf:     'type:perf',
};

module.exports = async ({ github, context }) => {
  const pr = context.payload.pull_request;

  header(`automation-label-pr  •  PR #${pr.number}  •  ${context.payload.action}`);
  step(`Title: "${pr.title}"`);

  const { data: currentLabels } = await github.rest.issues.listLabelsOnIssue({
    ...context.repo,
    issue_number: pr.number,
  });

  // ── 1. Type label from PR title ───────────────────────────────────────────
  section('Type label from PR title');

  const typeMatch = pr.title.match(/^([a-z]+)\//i);
  if (!typeMatch) {
    warn('PR title does not match type/<issue> convention — skipping type label.');
  } else {
    const key      = typeMatch[1].toLowerCase();
    const newLabel = TYPE_MAP[key];

    if (!newLabel) {
      warn(`Unknown type prefix "${typeMatch[1]}" — no matching label defined.`);
    } else {
      step(`Detected type: "${key}" → label: "${newLabel}"`);

      const stale = currentLabels.filter(l => l.name.startsWith('type:') && l.name !== newLabel);
      for (const l of stale) {
        await github.rest.issues
          .removeLabel({ ...context.repo, issue_number: pr.number, name: l.name })
          .catch(() => {});
        step(`Removed stale label "${l.name}"`);
      }

      if (currentLabels.find(l => l.name === newLabel)) {
        skip(`"${newLabel}" already present on PR — nothing to do.`);
      } else {
        await github.rest.issues.addLabels({
          ...context.repo,
          issue_number: pr.number,
          labels: [newLabel],
        });
        ok(`Applied "${newLabel}" to PR #${pr.number}`);
      }
    }
  }

  // ── 2. Copy priority:* from linked issue ──────────────────────────────────
  section('Priority labels from linked issue');

  const issueNumber = extractIssueNumber(pr.title);
  if (!issueNumber) {
    skip('Cannot extract issue number from PR title — skipping priority copy.');
    pass(`automation-label-pr: done for PR #${pr.number}`);
    return;
  }

  step(`Linked issue: #${issueNumber}`);

  const issue = await getIssue(github, context, issueNumber);
  if (!issue) {
    warn(`Issue #${issueNumber} not found — skipping priority copy.`);
    pass(`automation-label-pr: done for PR #${pr.number}`);
    return;
  }

  const toCopy  = issue.labels.map(l => l.name).filter(n => n.startsWith('priority:'));
  const toApply = toCopy.filter(n => !currentLabels.find(l => l.name === n));
  const already = toCopy.filter(n =>  currentLabels.find(l => l.name === n));

  if (already.length > 0) skip(`Already on PR: [${already.join(', ')}]`);

  if (toApply.length === 0) {
    skip(`No new priority:* labels to copy from issue #${issueNumber}.`);
  } else {
    step(`Copying from issue #${issueNumber}: [${toApply.join(', ')}]`);
    await github.rest.issues.addLabels({
      ...context.repo,
      issue_number: pr.number,
      labels: toApply,
    });
    ok(`Copied [${toApply.join(', ')}] to PR #${pr.number}`);
  }

  pass(`automation-label-pr: done for PR #${pr.number}`);
};
