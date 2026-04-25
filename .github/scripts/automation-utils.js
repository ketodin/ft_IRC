// ─────────────────────────────────────────────────────────────────────────────
// automation-utils.js — Shared helpers for all automation scripts
//
// Visual output mirrors ci-utils.sh intentionally:
//   same dividers, same symbols (→ ✓ ⊘ △ ✗), same ANSI color scheme.
// ─────────────────────────────────────────────────────────────────────────────

const IS_CI = process.env.GITHUB_ACTIONS === 'true' || process.stdout.isTTY;

const C = IS_CI ? {
  red: '\x1b[0;31m', grn: '\x1b[0;32m', yel: '\x1b[1;33m',
  blu: '\x1b[0;34m', cyn: '\x1b[0;36m', wht: '\x1b[1;37m',
  dim: '\x1b[2m',    rst: '\x1b[0m',
} : Object.fromEntries(
  ['red','grn','yel','blu','cyn','wht','dim','rst'].map(k => [k, ''])
);

const DIVIDER  = '━'.repeat(60);
const THIN_DIV = '─'.repeat(60);

// ── Print helpers ─────────────────────────────────────────────────────────────

function header(title) {
  console.log(`\n${C.cyn}${DIVIDER}${C.rst}`);
  console.log(`${C.wht}  ${title}${C.rst}`);
  console.log(`${C.cyn}${DIVIDER}${C.rst}`);
}

function section(title) {
  console.log(`\n${C.dim}  ${THIN_DIV.slice(0, 48)}${C.rst}`);
  console.log(`${C.dim}  ${title}${C.rst}`);
}

function step(msg) { console.log(`  ${C.blu}→${C.rst} ${msg}`); }
function ok(msg)   { console.log(`  ${C.grn}✓${C.rst} ${msg}`); }
function skip(msg) { console.log(`  ${C.dim}⊘ ${msg}${C.rst}`); }
function warn(msg) { console.log(`  ${C.yel}△${C.rst} ${msg}`); }
function fail(msg) { console.log(`  ${C.red}✗${C.rst} ${msg}`); }

// ── GitHub Actions annotations ────────────────────────────────────────────────

function notice(msg)         { console.log(`::notice::${msg}`); }
function warning(msg)        { console.log(`::warning::${msg}`); }
function error(msg)          { console.log(`::error::${msg}`); }
function errorAt(file, line, msg) { console.log(`::error file=${file},line=${line}::${msg}`); }

// ── Final banners ─────────────────────────────────────────────────────────────

function pass(label) {
  console.log(`\n${C.grn}  ${THIN_DIV.slice(0, 48)}${C.rst}`);
  console.log(`${C.grn}  ✓ PASSED${C.rst}${C.dim} — ${label}${C.rst}`);
  console.log(`${C.grn}  ${THIN_DIV.slice(0, 48)}${C.rst}\n`);
  notice(`PASSED — ${label}`);
}

function failExit(label) {
  console.log(`\n${C.red}  ${THIN_DIV.slice(0, 48)}${C.rst}`);
  console.log(`${C.red}  ✗ FAILED${C.rst}${C.dim} — ${label}${C.rst}`);
  console.log(`${C.red}  ${THIN_DIV.slice(0, 48)}${C.rst}\n`);
  error(`FAILED — ${label}`);
  process.exit(1);
}

// ── GitHub REST helpers ───────────────────────────────────────────────────────

// Extracts the issue number from a branch name or PR title.
// Expects: type/<issue>[-description]  or  type/<issue> Description
function extractIssueNumber(str) {
  const match = str.match(/^[a-z]+\/(\d+)/i);
  return match ? parseInt(match[1], 10) : null;
}

async function getIssue(github, context, issueNumber) {
  const { data } = await github.rest.issues
    .get({ ...context.repo, issue_number: issueNumber })
    .catch(() => ({ data: null }));
  return data;
}

async function getCurrentLabels(github, context, itemNumber) {
  const { data } = await github.rest.issues.listLabelsOnIssue({
    ...context.repo,
    issue_number: itemNumber,
  });
  return data;
}

// Strips all state:* labels then applies newLabels.
// Pass newLabels = [] to only strip existing state labels.
async function applyStateLabels(github, context, itemNumber, currentLabels, newLabels) {
  const stateLabels = currentLabels.filter(l => l.name.startsWith('state:'));

  if (stateLabels.length === 0) {
    skip(`#${itemNumber} has no state:* labels to remove.`);
  } else {
    for (const label of stateLabels) {
      await github.rest.issues
        .removeLabel({ ...context.repo, issue_number: itemNumber, name: label.name })
        .catch(() => {});
      step(`Removed "${label.name}" from #${itemNumber}`);
    }
  }

  if (newLabels.length > 0) {
    await github.rest.issues.addLabels({
      ...context.repo,
      issue_number: itemNumber,
      labels: newLabels,
    });
    ok(`Applied [${newLabels.join(', ')}] to #${itemNumber}`);
  } else {
    skip(`No new state labels to apply to #${itemNumber}.`);
  }
}

// ─────────────────────────────────────────────────────────────────────────────

module.exports = {
  header, section, step, ok, skip, warn, fail, pass, failExit,
  notice, warning, error, errorAt,
  extractIssueNumber, getIssue, getCurrentLabels, applyStateLabels,
};
