module.exports = async ({ github, context }) => {
  const pr = context.payload.pull_request;

  // Parse original issue from PR title: type/<issue-number> description
  const titleMatch = pr.title.match(/^[a-z]+\/(\d+)(?:\s|$)/i);
  if (!titleMatch) {
    console.log('[copy-labels] PR title does not match convention — skipping.');
    return;
  }

  const issueNumber = parseInt(titleMatch[1]);

  const { data: issue } = await github.rest.issues.get({
    ...context.repo,
    issue_number: issueNumber,
  }).catch(() => ({ data: null }));

  if (!issue) {
    console.log(`[copy-labels] Issue #${issueNumber} not found — skipping.`);
    return;
  }

  const toCopy = issue.labels
    .map(l => l.name)
    .filter(n => n.startsWith('type:') || n.startsWith('priority:'));

  if (toCopy.length === 0) {
    console.log(`[copy-labels] No type:/priority: labels on issue #${issueNumber} — skipping.`);
    return;
  }

  console.log(`[copy-labels] Copying labels from issue #${issueNumber} to PR #${pr.number}: ${toCopy.join(', ')}`);

  await github.rest.issues.addLabels({
    ...context.repo,
    issue_number: pr.number,
    labels: toCopy,
  });
};
