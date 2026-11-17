#!/usr/bin/env bash
# Build and run every test in tests/ three ways: -O2, with `#define int long long`, and with sanitizers.
# Usage: tools/run_tests.sh [name-filter]      Exit code = number of failing tests.
set -u
root="$(cd "$(dirname "$0")/.." && pwd)"
build="${TMPDIR:-/tmp}/cp-template-tests"
mkdir -p "$build"
ulimit -s unlimited 2>/dev/null || true
export ASAN_OPTIONS=detect_leaks=0 # contest code never frees; leaks are not bugs here
fails=0
for t in $(find "$root/tests" -name '*.cpp' | sort); do
    name="$(basename "$(dirname "$t")")/$(basename "$t" .cpp)"
    [[ -n "${1:-}" && "$name" != *"$1"* ]] && continue
    exe="$build/${name//\//_}"
    status="PASS"
    # 1) plain -O2
    if ! g++ -std=c++17 -O2 -o "$exe.o2" "$t" 2>"$exe.err" || ! timeout 600 "$exe.o2" >"$exe.out" 2>&1; then
        status="FAIL(o2)"
    # 2) sanitizers
    elif ! g++ -std=c++17 -O1 -g -fsanitize=address,undefined -fno-sanitize-recover=all \
            -o "$exe.san" "$t" 2>"$exe.err" || ! timeout 900 "$exe.san" >"$exe.sanout" 2>&1; then
        status="FAIL(sanitize)"
    fi
    echo "$status  $name  $(grep -m1 'RESULT' "$exe.out" 2>/dev/null)"
    [[ "$status" != PASS ]] && fails=$((fails + 1))
done
# 3) every template compiles alone and under `#define int long long`
for f in $(find "$root" -path "$root/other" -prune -o -path "$root/archive" -prune -o -path "$root/tests" -prune \
           -o -name '*.cpp' -print | grep '/[0-9][0-9]_' | sort); do
    for mode in plain intll; do
        w="$build/wrap.cpp"
        { echo '#include <bits/stdc++.h>'; echo 'using namespace std;'
          [[ $mode == intll ]] && echo '#define int long long'
          echo "#include \"$f\""; echo 'signed main() {}'; } >"$w"
        g++ -std=c++17 -fsyntax-only "$w" 2>/dev/null || echo "NO-COMPILE($mode)  ${f#$root/}"
    done
done
echo "failing tests: $fails"
exit $fails
