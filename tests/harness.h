// Shared reporting for the brute-force tests. Every test prints one line per property it
// checked with the number of DISAGREEMENTS, never a pass/fail claim, and exits non-zero if
// any of them is not zero.
//
// A test's job is to disagree with the template using an INDEPENDENT reference - a brute
// force, a different algorithm, or the structure's own promise. A test that reimplements
// the template and compares it to itself proves nothing.
#pragma once
#include <bits/stdc++.h>

namespace harness {
inline int &total_failures() {
    static int n = 0;
    return n;
}
inline std::string &title() {
    static std::string t;
    return t;
}

inline void name(const std::string &t) { title() = t; }

// one checked property: what was compared, how many disagreements, over how many cases
inline void check(const std::string &what, long long failures, long long cases = -1) {
    total_failures() += (failures != 0);
    if (cases >= 0)
        std::printf("  %-42s %6lld failures  (%lld cases)\n", what.c_str(), failures, cases);
    else
        std::printf("  %-42s %6lld failures\n", what.c_str(), failures);
}

inline int report() {
    std::printf("%s: %s\n", title().c_str(), total_failures() ? "FAILED" : "ok");
    return total_failures() ? 1 : 0;
}
} // namespace harness

using harness::check;
using harness::name;
using harness::report;
