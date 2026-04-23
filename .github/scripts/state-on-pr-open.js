// Triggered when a PR is opened.
//
// 1. Extract original-issue from PR title: type/<issue-number> description
//
// 2. Detect blocking PRs via two checks:
//    a) PR base branch is not main/master → base branch has its own open PR
//    b) PR body contains #N references that are NOT the original-issue
//       and those issues are still open
//
// 3a. No blocker found:
//     - original-issue: remove state:in-progress, add state:needs-review
//     - current PR:     remove state:in-progress, add state:needs-review
//
// 3b. Blocker found:
//     - original-issue: remove state:in-progress, add state:needs-review + state:blocked
//     - current PR:     remove state:in-progress, add state:needs-review + state:blocked

module.exports = async ({ github, context }) => {
  const pr = context.payload.pull_request;

  // ── 1. Extract original-issue from PR title ───────────────────────────────
  const titleMatch = pr.title.match(/^[a-z]+\/(\d+)\s/i);
  if (!titleMatch) return;

  const originalIssueNumber = parseInt(titleMatch[1]);

  const { data: originalIssue } = await github.rest.issues.get({
    ...context.repo,
    issue_number: originalIssueNumber,
  }).catch(() => ({ data: null }));

  if (!originalIssue || originalIssue.state !== 'open') return;

  // ── 2. Detect blocking conditions ─────────────────────────────────────────
  let isBlocked = false;

  // 2a. Base branch is not main/master and has its own open PR (stacked branch)
  const baseBranch = pr.base.ref;
  if (baseBranch !== 'main' && baseBranch !== 'master') {
    const { data: basePRs } = await github.rest.pulls.list({
      ...context.repo,
      state: 'open',
      head: `${context.repo.owner}:${baseBranch}`,
    });
    if (basePRs.length > 0) {
      isBlocked = true;
    }
  }

  // 2b. PR body references other open issues that are not the original-issue
  if (!isBlocked) {
    const body = pr.body || '';
    const allRefs = [...body.matchAll(/#(\d+)/g)]
      .map(m => parseInt(m[1]))
      .filter((n, i, arr) => n !== originalIssueNumber && arr.indexOf(n) === i);

    for (const ref of allRefs) {
      const { data: refItem } = await github.rest.issues.get({
        ...context.repo,
        issue_number: ref,
      }).catch(() => ({ data: null }));

      // Only an open issue (not a PR) counts as a blocker
      if (refItem && refItem.state === 'open' && !refItem.pull_request) {
        isBlocked = true;
        break;
      }
    }
  }

  // ── 3. Apply labels ────────────────────────────────────────────────────────
  const newLabels = ['state:needs-review'];
  if (isBlocked) newLabels.push('state:blocked');

  // Helper: strip all state:* labels then apply the new set
  const applyStateLabels = async (issueNumber, currentLabels) => {
    for (const label of currentLabels) {
      if (label.name.startsWith('state:')) {
        await github.rest.issues.removeLabel({
          ...context.repo,
          issue_number: issueNumber,
          name: label.name,
        }).catch(() => {});
      }
    }
    await github.rest.issues.addLabels({
      ...context.repo,
      issue_number: issueNumber,
      labels: newLabels,
    });
  };

  // Apply to original-issue
  await applyStateLabels(originalIssueNumber, originalIssue.labels);

  // Apply to the PR itself (GitHub treats PRs as issues for label endpoints)
  const { data: prLabels } = await github.rest.issues.listLabelsOnIssue({
    ...context.repo,
    issue_number: pr.number,
  });
  await applyStateLabels(pr.number, prLabels);
};

