// Integration: definite integral of any function over [a, b] by composite Simpson's rule.
// Use when: "area under the curve", a curved region's area, expected value over a continuous range.
// Usage: simpson_integration([](double x) { return sin(x); }, 0, M_PI);  // 2.0; n = 1e6 steps
// Handles: a > b (the sign flips), a == b (0), an odd n (rounded up to even).
// Time: O(n) calls to f
// Indexing: not applicable
// Note: n ~ 1e4 is plenty for smooth f (polynomials, sin, cos, exp, short interval); use ~1e6 for
//       log, tan, 1/x or long intervals. f must be finite on all of [a, b] (both ends are used).

#include <bits/stdc++.h>
using namespace std;

template <class F> double simpson_integration(F f, double a, double b, int n = 1'000'000) {
    if (n % 2) n++;
    double h = (b - a) / n;
    double s = f(a) + f(b);
    for (int i = 1; i < n; ++i) s += f(a + h * i) * ((i & 1) ? 4.0 : 2.0);
    return s * (h / 3.0);
}
