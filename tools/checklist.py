#!/usr/bin/env python3
"""Regenerate VERIFICATION.md: every template in the repo, and exactly what backs it.

Reads verify/results.json (written by tools/verify.py) and walks the tree, so the
checklist cannot drift from what was actually run.
"""
import json, pathlib, collections, re

ROOT = pathlib.Path(__file__).resolve().parent.parent
RES = ROOT / 'verify' / 'results.json'
NOTES = ROOT / 'verify' / 'notes.tsv'  # template -> why it has no judge problem

BADGE = {'AC': '**JUDGE**', 'WA': 'FAIL', 'TLE': 'TLE', 'CE': 'BUILD FAIL',
         'RE': 'RUNTIME', 'ERROR': 'HARNESS ERROR', 'NO-TESTS': 'NO DATA'}


def main():
    results = json.loads(RES.read_text(encoding='utf-8')) if RES.exists() else {}
    by_template = collections.defaultdict(list)
    for name, r in results.items():
        by_template[r['template']].append((name, r))

    notes = {}
    if NOTES.exists():
        for line in NOTES.read_text(encoding='utf-8').splitlines():
            if line.strip() and not line.startswith('#'):
                parts = line.split('\t')
                notes[parts[0]] = parts[1] if len(parts) > 1 else ''

    # which templates a brute-force test actually includes
    tested = collections.defaultdict(list)
    for t in sorted((ROOT / 'tests').glob('*.cpp')):
        for inc in re.findall(r'#include "\.\./([^"]+)"', t.read_text(encoding='utf-8')):
            tested[inc].append(t.stem)

    dirs = collections.defaultdict(list)
    for f in sorted(ROOT.rglob('*.cpp')):
        rel = f.relative_to(ROOT).as_posix()
        if rel.startswith(('other/', 'verify/', 'tests/')):
            continue
        top = rel.split('/')[0]
        if len(rel.split('/')) > 2:
            top = '/'.join(rel.split('/')[:2])
        dirs[top].append(rel)

    judged = sum(1 for t in dirs for rel in dirs[t]
                 if any(r['status'] == 'AC' and not (r.get('limit') and r['slowest'] > r['limit'])
                        for _, r in by_template.get(rel, [])))
    total = sum(len(v) for v in dirs.values())

    out = []
    out.append('# Verification status\n')
    out.append('What actually backs each template. Regenerate with `py tools/checklist.py`;\n'
               'the data comes from `verify/results.json`, which `tools/verify.py` writes.\n')
    out.append('| | meaning |')
    out.append('| --- | --- |')
    out.append('| **JUDGE** | passes a real judge\'s full system tests, run locally against the judge\'s own checker |')
    out.append('| brute force | a test in `tests/` compares it to an independent reference over randomised inputs |')
    out.append('| UNGUARDED | neither - nothing here re-checks it |')
    out.append('| FAIL / TLE | the judge data rejects it - see the note |')
    out.append("| OVER LIMIT | every case is CORRECT, but the slowest run exceeded the limit here |")
    out.append('')
    out.append("Times below are the slowest single case on the machine that ran this, against the"
               " judge's stated limit. That machine is not the judge - read a thin margin as a warning.")
    out.append("")
    out.append('**%d of %d templates carry a judge verdict.** The rest stand on brute-force agreement,\n'
               'which catches wrong answers but cannot establish a time bound.\n' % (judged, total))

    for d in sorted(dirs):
        out.append('## %s\n' % d)
        out.append('| Template | Status | Evidence |')
        out.append('| --- | --- | --- |')
        for rel in dirs[d]:
            name = rel.split('/')[-1]
            runs = by_template.get(rel, [])
            ac = [r for _, r in runs if r['status'] == 'AC']
            over = [r for r in ac if r.get('limit') and r['slowest'] > r['limit']]
            if rel in tested and ac:
                pass
            if ac:
                ev = '; '.join('[%s](%s) %d/%d cases, %.2fs of %.0fs' %
                               (r['url'].rsplit('/', 1)[-1], r['url'], r['passed'], r['cases'],
                                r['slowest'], r.get('limit') or 0)
                               for r in ac)
                # correct on every case, but slower here than the judge allows
                status = 'OVER LIMIT' if over and len(over) == len(ac) else BADGE['AC']
            elif runs:
                r = runs[0][1]
                status = BADGE.get(r['status'], r['status'])
                ev = '[%s](%s) %d/%d cases' % (r['url'].rsplit('/', 1)[-1], r['url'], r['passed'], r['cases'])
            elif rel in tested:
                status = 'brute force'
                ev = 'tests/' + tested[rel][0] + '.cpp' + (
                    ' - ' + notes[rel] if rel in notes else '')
            else:
                status = 'UNGUARDED'
                ev = notes.get(rel, 'no judge problem and no brute-force test')
            out.append('| [%s](%s) | %s | %s |' % (name, rel, status, ev))
        out.append('')

    unguarded = sum(1 for d in dirs for rel in dirs[d]
                    if rel not in tested and not by_template.get(rel))
    out.append("")
    out.append("**%d templates are UNGUARDED** - no judge problem and no brute-force test." % unguarded)
    out.append("")
    (ROOT / 'VERIFICATION.md').write_text('\n'.join(out) + '\n', encoding='utf-8', newline='\n')
    print('VERIFICATION.md: %d judged / %d templates' % (judged, total))


if __name__ == '__main__':
    main()
