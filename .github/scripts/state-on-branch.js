// Triggered on branch creation.
// Branch must follow the format: type/<issue-number>-description
// e.g. chore/27-auto-label-issues-and-prs
module.exports = async ({ github, context }) => {
  const branch = context.ref;
  const match = branch.match(/^[a-z]+\/(\d+)-/);
  if (!match) return;

  const issueNumber = parseInt(match[1]);

  // Verify the issue exists and is open
  const { data: issue } = await github.rest.issues.get({
    ...context.repo,
    issue_number: issueNumber,
  }).catch(() => ({ data: null }));

  if (!issue || issue.state !== 'open' || issue.pull_request) return;

  // Remove any stale state:* labels
  for (const label of issue.labels) {
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
    labels: ['state:in-progress'],
  });
};
