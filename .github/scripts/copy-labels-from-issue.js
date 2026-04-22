// Usage: called by actions/github-script with { github, context } injected
module.exports = async ({ github, context }) => {
  const body = context.payload.pull_request.body || '';
  const issueMatch = body.match(/(?:closes|fixes|resolves):?\s+#(\d+)/i);
  if (!issueMatch) return;

  const issueNumber = parseInt(issueMatch[1]);

  const { data: issueLabels } = await github.rest.issues.listLabelsOnIssue({
    ...context.repo,
    issue_number: issueNumber,
  });

  const toCopy = issueLabels
    .map(l => l.name)
    .filter(n => n.startsWith('type:') || n.startsWith('priority:'));

  if (toCopy.length === 0) return;

  await github.rest.issues.addLabels({
    ...context.repo,
    issue_number: context.payload.pull_request.number,
    labels: toCopy,
  });
}
