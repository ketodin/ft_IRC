// ─────────────────────────────────────────────────────────────────────────────
// automation-state-pr-draft.js
// Called by: automation-state-pr.yaml (converted_to_draft)
//
// Reverts both the PR and its linked issue back to state:in-progress,
// signalling that the work is not yet ready for review.
// ─────────────────────────────────────────────────────────────────────────────

const {
  header, step, ok, skip, warn, pass,
  extractIssueNumber, getIssue, getCurrentLabels, applyStateLabels,
} = require('./automation-utils');

module.exports = async ({ github, context }) => {
  const pr = context.payload.pull_request;

  header(`automation-state-pr-draft  •  PR #${pr.number}  •  converted to draft`);
  step(`Title: "${pr.title}"`);

  // ── Extract and validate linked issue ─────────────────────────────────────
  const issueNumber = extractIssueNumber(pr.title);
  if (!issueNumber) {
    warn('Cannot extract issue number from PR title — skipping.');
    return;
  }

  step(`Linked issue: #${issueNumber}`);
  const issue = await getIssue(github, context, issueNumber);

  if (!issue || issue.state !== 'open') {
    warn(`Issue #${issueNumber} not found or not open — skipping.`);
    return;
  }
  step(`Issue found: "${issue.title}"`);

  // ── Apply state:in-progress to both PR and issue ──────────────────────────
  step('Applying state:in-progress to PR and issue...');
  const prLabels = await getCurrentLabels(github, context, pr.number);
  await applyStateLabels(github, context, issueNumber, issue.labels, ['state:in-progress']);
  await applyStateLabels(github, context, pr.number, prLabels, ['state:in-progress']);

  pass(`automation-state-pr-draft: PR #${pr.number} and issue #${issueNumber} → state:in-progress`);
};
