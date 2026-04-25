// ─────────────────────────────────────────────────────────────────────────────
// automation-state-pr-close.js
// Called by: automation-state-pr.yaml (closed)
//
// Handles two distinct scenarios:
//
//   A. PR closed without merge (abandoned)
//      → Clear state labels from PR
//      → Revert linked issue to state:in-progress
//
//   B. PR merged
//      → Clear all state labels from PR and linked issue
//      → Find all open PRs whose base was the merged branch (stacked PRs):
//          → Retarget each to the merged PR's base (main)
//          → Remove state:blocked, set state:needs-review on each
//            stacked PR and its linked issue
//
// Note: retargeting a stacked PR triggers a pull_request.edited event,
// which re-runs automation-state-pr-open.js on that PR — this is intentional
// and idempotent (confirms needs-review with no blocker on the new base).
// ─────────────────────────────────────────────────────────────────────────────

const {
  header, section, step, ok, skip, warn, pass,
  extractIssueNumber, getIssue, getCurrentLabels, applyStateLabels,
} = require('./automation-utils');

module.exports = async ({ github, context }) => {
  const pr     = context.payload.pull_request;
  const merged = pr.merged === true;

  header(`automation-state-pr-close  •  PR #${pr.number}  •  ${merged ? 'MERGED ✓' : 'CLOSED — abandoned'}`);
  step(`Title : "${pr.title}"`);
  step(`Base  : "${pr.base.ref}"  →  Head: "${pr.head.ref}"`);

  const issueNumber = extractIssueNumber(pr.title);
  if (!issueNumber) {
    warn('Cannot extract issue number from PR title — skipping.');
    return;
  }
  step(`Linked issue: #${issueNumber}`);

  const issue    = await getIssue(github, context, issueNumber);
  const prLabels = await getCurrentLabels(github, context, pr.number);

  // ══════════════════════════════════════════════════════════════════════════
  // Case A — Abandoned
  // ══════════════════════════════════════════════════════════════════════════
  if (!merged) {
    section('PR abandoned — clearing PR state, reverting issue');

    await applyStateLabels(github, context, pr.number, prLabels, []);

    if (issue && issue.state === 'open') {
      await applyStateLabels(github, context, issueNumber, issue.labels, ['state:in-progress']);
      ok(`Issue #${issueNumber} reverted to state:in-progress.`);
    } else {
      skip(`Issue #${issueNumber} is not open — state revert skipped.`);
    }

    pass(`automation-state-pr-close: PR #${pr.number} abandoned — issue #${issueNumber} → state:in-progress`);
    return;
  }

  // ══════════════════════════════════════════════════════════════════════════
  // Case B — Merged
  // ══════════════════════════════════════════════════════════════════════════

  // B1. Clear merged PR state labels
  section('Clear state labels from merged PR and issue');

  await applyStateLabels(github, context, pr.number, prLabels, []);
  ok(`PR #${pr.number} state labels cleared.`);

  if (issue) {
    await applyStateLabels(github, context, issueNumber, issue.labels, []);
    ok(`Issue #${issueNumber} state labels cleared.`);
  } else {
    skip(`Issue #${issueNumber} not found — state clear skipped.`);
  }

  // B2. Unblock stacked PRs
  const mergedBranch = pr.head.ref;
  const targetBase   = pr.base.ref;

  section(`Unblock stacked PRs (base was "${mergedBranch}")`);
  step(`Scanning all open PRs for base="${mergedBranch}"...`);

  const { data: allOpenPRs } = await github.rest.pulls.list({
    ...context.repo,
    state: 'open',
  });

  const stacked = allOpenPRs.filter(p => p.base.ref === mergedBranch);

  if (stacked.length === 0) {
    skip('No stacked PRs found — nothing to unblock.');
    pass(`automation-state-pr-close: PR #${pr.number} merged, no stacked PRs`);
    return;
  }

  step(`Found ${stacked.length} stacked PR(s): [${stacked.map(p => `#${p.number}`).join(', ')}]`);

  for (const stackedPR of stacked) {
    section(`Stacked PR #${stackedPR.number}: "${stackedPR.title}"`);

    // Retarget base to main
    await github.rest.pulls.update({
      ...context.repo,
      pull_number: stackedPR.number,
      base: targetBase,
    });
    ok(`PR #${stackedPR.number} retargeted: "${mergedBranch}" → "${targetBase}"`);

    // Remove state:blocked, set state:needs-review on PR
    const stackedPRLabels = await getCurrentLabels(github, context, stackedPR.number);
    await applyStateLabels(github, context, stackedPR.number, stackedPRLabels, ['state:needs-review']);
    ok(`PR #${stackedPR.number} → state:needs-review`);

    // Same on linked issue
    const stackedIssueNumber = extractIssueNumber(stackedPR.title);
    if (!stackedIssueNumber) {
      warn(`Cannot extract issue number from PR #${stackedPR.number} title — skipping issue unblock.`);
      continue;
    }

    const stackedIssue = await getIssue(github, context, stackedIssueNumber);
    if (stackedIssue && stackedIssue.state === 'open') {
      await applyStateLabels(github, context, stackedIssueNumber, stackedIssue.labels, ['state:needs-review']);
      ok(`Issue #${stackedIssueNumber} → state:needs-review`);
    } else {
      skip(`Issue #${stackedIssueNumber} is not open — unblock skipped.`);
    }
  }

  pass(`automation-state-pr-close: PR #${pr.number} merged, ${stacked.length} stacked PR(s) unblocked`);
};
