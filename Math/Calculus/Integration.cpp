// Integration: the area under a curve between two points, by Simpson's rule or by adaptive refinement.
// Use when: "the area of this region", an expected value over a continuous range, a curved geometric boundary.
// Handles: any callable f, a reversed interval (the sign flips), and adaptive refinement where the curve bends sharply.
// Time: simpson O(steps) | adaptive O(depth) calls, concentrated where the curve is not smooth
// Indexing: not applicable
// Note: Simpson needs an EVEN step count and it is forced to one here. Smooth curves need ~1e4 steps, sharp ones ~1e6.

#include <bits/stdc++.h>
using namespace std;

template <typename F> double simpson(F f, double a, double b, int steps = 100000) {
    if (a == b) return 0.0;
    if (steps % 2) steps++; // Simpson's rule is undefined for an odd step count
    double h = (b - a) / steps;
    double total = f(a) + f(b);
    for (int i = 1; i < steps; i++) total += f(a + h * i) * ((i & 1) ? 4.0 : 2.0);
    return total * h / 3.0;
}

// one Simpson estimate over [a, b], reusing values the caller already has
template <typename F> double simpson_once(F f, double a, double b, double fa, double fm, double fb) {
    return (b - a) / 6.0 * (fa + 4.0 * fm + fb);
}

template <typename F>
double adaptive_helper(F f, double a, double b, double eps, double whole, double fa, double fm, double fb, int depth) {
    double m = (a + b) / 2.0;
    double lm = (a + m) / 2.0, rm = (m + b) / 2.0;
    double flm = f(lm), frm = f(rm);
    double left = simpson_once(f, a, m, fa, flm, fm);
    double right = simpson_once(f, m, b, fm, frm, fb);
    if (depth <= 0 || fabs(left + right - whole) <= 15.0 * eps) return left + right + (left + right - whole) / 15.0;
    return adaptive_helper(f, a, m, eps / 2.0, left, fa, flm, fm, depth - 1) +
           adaptive_helper(f, m, b, eps / 2.0, right, fm, frm, fb, depth - 1);
}

// refines only where the curve needs it, so a sharp bend costs far less than a uniform grid
template <typename F> double adaptive_simpson(F f, double a, double b, double eps = 1e-9, int max_depth = 50) {
    if (a == b) return 0.0;
    double m = (a + b) / 2.0;
    double fa = f(a), fm = f(m), fb = f(b);
    double whole = simpson_once(f, a, b, fa, fm, fb);
    return adaptive_helper(f, a, b, eps, whole, fa, fm, fb, max_depth);
}

template <typename F> double trapezoid(F f, double a, double b, int steps = 100000) {
    double h = (b - a) / steps, total = (f(a) + f(b)) / 2.0;
    for (int i = 1; i < steps; i++) total += f(a + h * i);
    return total * h;
}

// the area between two curves over [a, b]
template <typename F, typename G> double area_between(F upper, G lower, double a, double b, int steps = 100000) {
    return simpson([&](double x) { return upper(x) - lower(x); }, a, b, steps);
}

// Standard problem: integrate f(x) = x^2 + x + 1 over [a, b]
void solve() {
    double a, b;
    cin >> a >> b;
    auto f = [](double x) { return x * x + x + 1; };
    cout << fixed << setprecision(9) << adaptive_simpson(f, a, b) << '\n';
}
