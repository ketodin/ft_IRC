// ─────────────────────────────────────────────────────────────────────────────
// automation-state-pr-open.js
// Called by: automation-state-pr.yaml (opened, ready_for_review, edited)
//
// Applies state:needs-review to the PR and its linked issue.
// Also applies state:blocked if a blocker is detected:
//   (a) PR base is not main/master and that base branch has its own open PR
//   (b) PR body explicitly references another open issue (not the linked one)
//
// For "edited" events, only reacts if the base branch changed.
// Skips entirely if the PR is a draft (handled by automation-state-pr-draft.js).
// ─────────────────────────────────────────────────────────────────────────────

const {
  header, section, step, ok, skip, warn, pass,
  extractIssueNumber, getIssue, getCurrentLabels, applyStateLabels,
} = require('./automation-utils');

module.exports = async ({ github, context }) => {
  const pr     = context.payload.pull_request;
  const action = context.payload.action;

  header(`automation-state-pr-open  •  PR #${pr.number}  •  ${action}`);
  step(`Title : "${pr.title}"`);
  step(`Base  : "${pr.base.ref}"  →  Head: "${pr.head.ref}"`);
  step(`Draft : ${pr.draft}`);

  // ── Guard: drafts are handled by automation-state-pr-draft.js ────────────
  if (pr.draft) {
    skip('PR is a draft — delegated to automation-state-pr-draft.js.');
    return;
  }

  // ── Guard: for "edited" only react to base-branch changes ────────────────
  if (action === 'edited') {
    const from = context.payload.changes?.base?.ref?.from;
    if (!from) {
      skip('PR edited — base branch did not change. Skipping.');
      return;
    }
    step(`Base branch changed: "${from}" → "${pr.base.ref}"`);
  }

  // ── 1. Extract and validate linked issue ──────────────────────────────────
  section('Linked issue');

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

  // ── 2. Blocker detection ──────────────────────────────────────────────────
  section('Blocker detection');

  let isBlocked          = false;
  let blockerDescription = '';

  // 2a. Stacked branch: base is not main/master and has an open PR
  if (pr.base.ref !== 'main' && pr.base.ref !== 'master') {
    step(`Base is not main — checking for an open PR on "${pr.base.ref}"...`);
    const { data: basePRs } = await github.rest.pulls.list({
      ...context.repo,
      state: 'open',
      head: `${context.repo.owner}:${pr.base.ref}`,
    });
    if (basePRs.length > 0) {
      isBlocked          = true;
      blockerDescription = `stacked on PR #${basePRs[0].number} ("${basePRs[0].title}")`;
      warn(`Blocker (stacking): ${blockerDescription}`);
    } else {
      step(`No open PR on base "${pr.base.ref}" — not blocked by stacking.`);
    }
  } else {
    step('Base is main/master — stacking check skipped.');
  }

  // 2b. Explicit dependency: PR body references other open issues
  if (!isBlocked) {
    const body = pr.body || '';
    const refs = [...body.matchAll(/#(\d+)/g)]
      .map(m => parseInt(m[1], 10))
      .filter((n, i, arr) => n !== issueNumber && arr.indexOf(n) === i);

    if (refs.length === 0) {
      step('No other issue references in PR body.');
    } else {
      step(`PR body references: [${refs.map(n => `#${n}`).join(', ')}] — validating...`);
      for (const ref of refs) {
        const refItem = await getIssue(github, context, ref);
        if (refItem && refItem.state === 'open' && !refItem.pull_request) {
          isBlocked          = true;
          blockerDescription = `depends on open issue #${ref} ("${refItem.title}")`;
          warn(`Blocker (explicit ref): ${blockerDescription}`);
          break;
        }
      }
      if (!isBlocked) step('All referenced items are closed or PRs — not blocked.');
    }
  }

  // ── 3. Apply labels ───────────────────────────────────────────────────────
  section('Applying state labels');

  const newLabels = ['state:needs-review', ...(isBlocked ? ['state:blocked'] : [])];
  step(`Outcome: ${isBlocked ? `BLOCKED — ${blockerDescription}` : 'NOT BLOCKED'}`);
  step(`Applying [${newLabels.join(', ')}] to issue #${issueNumber} and PR #${pr.number}...`);

  const prLabels = await getCurrentLabels(github, context, pr.number);
  await applyStateLabels(github, context, issueNumber, issue.labels, newLabels);
  await applyStateLabels(github, context, pr.number, prLabels, newLabels);

  pass(`automation-state-pr-open: PR #${pr.number} and issue #${issueNumber} → [${newLabels.join(', ')}]`);
};
