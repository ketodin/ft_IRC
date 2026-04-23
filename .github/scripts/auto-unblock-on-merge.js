// Trigger:  pull_request closed (merged only), targeting main
// Reads:    the merged branch name from context
// Finds:    all open PRs whose base branch = merged branch
// Writes:   removes state:blocked from each found PR and its linked issue
//           (issue number parsed from PR title: type/<issue> description)

module.exports = async ({ github, context }) => {
  const mergedBranch = context.payload.pull_request.head.ref;

  console.log(`[unblock-on-merge] Merged branch: ${mergedBranch}`);

  // Find all open PRs that were stacked on the merged branch
  const { data: openPRs } = await github.rest.pulls.list({
    ...context.repo,
    state: 'open',
    base: mergedBranch,
  });

  if (openPRs.length === 0) {
    console.log('[unblock-on-merge] No stacked PRs found — nothing to unblock.');
    return;
  }

  console.log(`[unblock-on-merge] Found ${openPRs.length} stacked PR(s) to unblock.`);

  for (const pr of openPRs) {
    console.log(`[unblock-on-merge] Processing PR #${pr.number}: "${pr.title}"`);

    // Remove state:blocked from the PR
    await github.rest.issues.removeLabel({
      ...context.repo,
      issue_number: pr.number,
      name: 'state:blocked',
    }).catch(() => {
      console.log(`[unblock-on-merge] PR #${pr.number} had no state:blocked — skipping.`);
    });

    // Parse linked issue from PR title: type/<issue-number> description
    const titleMatch = pr.title.match(/^[a-z]+\/(\d+)(?:\s|$)/i);
    if (!titleMatch) {
      console.log(`[unblock-on-merge] PR #${pr.number} title does not match convention — skipping issue unblock.`);
      continue;
    }

    const issueNumber = parseInt(titleMatch[1]);
    console.log(`[unblock-on-merge] Removing state:blocked from linked issue #${issueNumber}`);

    await github.rest.issues.removeLabel({
      ...context.repo,
      issue_number: issueNumber,
      name: 'state:blocked',
    }).catch(() => {
      console.log(`[unblock-on-merge] Issue #${issueNumber} had no state:blocked — skipping.`);
    });
  }

  console.log('[unblock-on-merge] ✅ Done.');
};
