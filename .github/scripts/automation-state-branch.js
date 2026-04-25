// ─────────────────────────────────────────────────────────────────────────────
// automation-state-branch.js
// Called by: automation-state-branch.yaml (create: branch)
//
// Extracts the issue number from the branch name (type/<issue>[-description]),
// validates the issue is open, and sets state:in-progress on it.
// ─────────────────────────────────────────────────────────────────────────────

const {
  header, step, ok, skip, warn, pass,
  extractIssueNumber, getIssue, applyStateLabels,
} = require('./automation-utils');

module.exports = async ({ github, context }) => {
  const branch = context.ref;

  header(`automation-state-branch  •  Branch: "${branch}"`);

  // ── Parse branch name ─────────────────────────────────────────────────────
  const issueNumber = extractIssueNumber(branch);
  if (!issueNumber) {
    skip('Branch name does not match type/<issue>[-description] — skipping.');
    return;
  }

  step(`Extracted issue number: #${issueNumber}`);

  // ── Validate issue ────────────────────────────────────────────────────────
  const issue = await getIssue(github, context, issueNumber);

  if (!issue) {
    warn(`Issue #${issueNumber} not found — skipping.`);
    return;
  }
  if (issue.pull_request) {
    warn(`#${issueNumber} is a PR, not an issue — skipping.`);
    return;
  }
  if (issue.state !== 'open') {
    warn(`Issue #${issueNumber} is ${issue.state} — skipping.`);
    return;
  }

  step(`Issue found: "${issue.title}" (${issue.state})`);

  // ── Apply state:in-progress ───────────────────────────────────────────────
  await applyStateLabels(github, context, issueNumber, issue.labels, ['state:in-progress']);

  pass(`automation-state-branch: issue #${issueNumber} → state:in-progress`);
};
