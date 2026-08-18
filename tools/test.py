#!/usr/bin/env python3
"""Compile and run every brute-force test in tests/.

Each test includes its template directly and compares it against an independent
reference over randomised inputs. Exit code is the number of failing tests.
"""
import argparse, pathlib, re, subprocess, sys, time

ROOT = pathlib.Path(__file__).resolve().parent.parent
TESTS = ROOT / 'tests'
BUILD = TESTS / '.build'


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--only', help='substring of the test name')
    ap.add_argument('--quiet', action='store_true', help='one line per test, not per property')
    a = ap.parse_args()
    BUILD.mkdir(parents=True, exist_ok=True)
    files = sorted(f for f in TESTS.glob('*.cpp') if not a.only or a.only.lower() in f.stem.lower())
    bad, slow = [], []
    for f in files:
        exe = BUILD / (f.stem + '.exe')
        c = subprocess.run(['g++', '-std=c++17', '-O2', '-Wall', '-Wl,--stack,268435456',
                            '-o', str(exe), str(f)], capture_output=True, text=True)
        if c.returncode != 0:
            print('BUILD FAIL  %s' % f.stem)
            for line in c.stderr.strip().splitlines()[:8]:
                print('    ' + line)
            bad.append(f.stem)
            continue
        t0 = time.time()
        try:
            p = subprocess.run([str(exe)], capture_output=True, text=True, timeout=600)
        except subprocess.TimeoutExpired:
            print('TIMEOUT     %s' % f.stem)
            bad.append(f.stem)
            continue
        dt = time.time() - t0
        if dt > 30:
            slow.append((f.stem, dt))
        # the tests report counts rather than exiting non-zero, so read the counts
        nonzero = [int(m) for m in re.findall(r'(\d+)\s+(?:failures|mismatch)', p.stdout)]
        failed = p.returncode != 0 or any(n > 0 for n in nonzero) or not nonzero
        if failed:
            bad.append(f.stem)
        if a.quiet:
            print('%-4s %-42s %5.1fs%s' % ('FAIL' if failed else 'ok', f.stem, dt,
                  '' if nonzero else '   (no failure counts printed)'))
        else:
            sys.stdout.write(p.stdout)
            if failed:
                sys.stdout.write(p.stderr[:400])
    print('\n%d tests | %d failing%s' % (len(files), len(bad), (': ' + ', '.join(bad)) if bad else ''))
    for n, d in slow:
        print('  slow: %s took %.0fs' % (n, d))
    return len(bad)


if __name__ == '__main__':
    sys.exit(main())
