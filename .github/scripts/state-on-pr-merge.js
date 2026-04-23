// Triggered when a PR is merged.
// Reads the linked issue from "Closes/Fixes/Resolves #N" in the PR body
// and removes all state:* labels from it.
module.exports = async ({ github, context }) => {
  const body = context.payload.pull_request.body || '';
  const match = body.match(/(?:closes|fixes|resolves):?\s+#(\d+)/i);
  if (!match) return;

  const issueNumber = parseInt(match[1]);

  const { data: issue } = await github.rest.issues.get({
    ...context.repo,
    issue_number: issueNumber,
  }).catch(() => ({ data: null }));

  if (!issue) return;

  for (const label of issue.labels) {
    if (label.name.startsWith('state:')) {
      await github.rest.issues.removeLabel({
        ...context.repo,
        issue_number: issueNumber,
        name: label.name,
      }).catch(() => {});
    }
  }
};
