// Usage: called by actions/github-script with { github, context } injected
module.exports = async ({ github, context }) => {
  const title = context.payload.issue.title;
  const match = title.match(/^\[([A-Z]+)\]/);
  if (!match) return;

  const typeMap = {
    TASK:     'type:task',
    FIX:      'type:fix',
    REFACTOR: 'type:refactor',
    STYLE:    'type:style',
    DOCS:     'type:docs',
    TEST:     'type:test',
    CHORE:    'type:chore',
    PERF:     'type:perf',
  };

  const label = typeMap[match[1].toUpperCase()];
  if (!label) return;

  const { data: current } = await github.rest.issues.listLabelsOnIssue({
    ...context.repo,
    issue_number: context.issue.number,
  });

  for (const l of current) {
    if (l.name.startsWith('type:') && l.name !== label) {
      await github.rest.issues.removeLabel({
        ...context.repo,
        issue_number: context.issue.number,
        name: l.name,
      }).catch(() => {});
    }
  }

  await github.rest.issues.addLabels({
    ...context.repo,
    issue_number: context.issue.number,
    labels: [label],
  });
};
