// Usage: called by actions/github-script with { github, context } injected
module.exports = async ({ github, context }) => {
  const title = context.payload.pull_request.title;
  const match = title.match(/^([a-z]+)\//i);
  if (!match) return;

  const typeMap = {
    task:     'type:task',
    fix:      'type:fix',
    refactor: 'type:refactor',
    style:    'type:style',
    docs:     'type:docs',
    test:     'type:test',
    chore:    'type:chore',
    perf:     'type:perf',
  };

  const label = typeMap[match[1].toLowerCase()];
  if (!label) return;

  const { data: current } = await github.rest.issues.listLabelsOnIssue({
    ...context.repo,
    issue_number: context.payload.pull_request.number,
  });

  for (const l of current) {
    if (l.name.startsWith('type:') && l.name !== label) {
      await github.rest.issues.removeLabel({
        ...context.repo,
        issue_number: context.payload.pull_request.number,
        name: l.name,
      }).catch(() => {});
    }
  }

  await github.rest.issues.addLabels({
    ...context.repo,
    issue_number: context.payload.pull_request.number,
    labels: [label],
  });
};
