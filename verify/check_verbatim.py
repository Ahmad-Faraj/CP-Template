# Confirms every submission under verify/ still embeds its template byte for byte.
# Run after touching any template, so a submission never drifts from what it claims to prove.
#   python verify/check_verbatim.py

import hashlib
import io
import os
import sys

BEGIN = '// ---------------- BEGIN VERBATIM COPY OF '
END = '// ---------------- END VERBATIM COPY'

ROOT = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(ROOT)  # verify/ now lives inside the repo


def normalise(text):
    return text.replace('\r\n', '\n')


def check(submission):
    text = io.open(submission, encoding='utf-8').read()
    if BEGIN not in text or END not in text:
        return None, 'no verbatim markers'

    start = text.index(BEGIN)
    rel = text[start + len(BEGIN):text.index('\n', start)].strip().rstrip('-').strip()
    body_start = text.index('\n', start) + 1
    embedded = text[body_start:text.index(END)]

    source = os.path.join(REPO, *rel.replace('CP-Template/', '').split('/'))
    if not os.path.exists(source):
        return False, 'template not found: ' + source

    original = io.open(source, encoding='utf-8').read()
    same = normalise(original) == normalise(embedded)
    digest = hashlib.sha256(normalise(original).encode()).hexdigest()[:16]
    return same, rel + '  sha ' + digest


def main():
    failures = 0
    found = 0
    for folder, _, files in os.walk(ROOT):
        if os.sep + '.' in folder:
            continue  # .cache and .build are generated, never checked in
        for name in files:
            if name not in ('test_template.cpp', 'main.cpp'):
                continue
            found += 1
            path = os.path.join(folder, name)
            label = os.path.relpath(path, ROOT)
            same, detail = check(path)
            if same is None:
                print('  SKIP  %-40s %s' % (label, detail))
            elif same:
                print('  OK    %-40s %s' % (label, detail))
            else:
                print('  DRIFT %-40s %s' % (label, detail))
                failures += 1
    if not found:
        print('  no submission files found under verify/')
    print('\n%d checked, %d drifted' % (found, failures))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
