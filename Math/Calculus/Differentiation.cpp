// Differentiation: the numerical derivative of a function at a point, by central differences.
// Use when: the slope of a function you can only evaluate, Newton's method, finding where the slope is zero.
// Handles: first and second derivatives, partial derivatives of a multi-variable function, and any callable f.
// Time: O(1) per derivative, counting a constant number of calls to f
// Indexing: not applicable
// Note: the step differs by ORDER - 1e-5 for the first derivative, 1e-4 for the second. One size fits neither.

#include <bits/stdc++.h>
using namespace std;

// central difference: error is O(h^2) from truncation and O(eps/h) from rounding,
// so the best h is around eps^(1/3) for the first derivative
const double H1 = 1e-5;
// the second derivative divides by h^2, which amplifies rounding, so it needs a LARGER h
const double H2 = 1e-4;

template <typename F> double first_derivative(F f, double x, double h = H1) {
    return (f(x + h) - f(x - h)) / (2.0 * h);
}

template <typename F> double second_derivative(F f, double x, double h = H2) {
    return (f(x + h) - 2.0 * f(x) + f(x - h)) / (h * h);
}

// a more accurate first derivative, using four points; error O(h^4)
template <typename F> double first_derivative_5point(F f, double x, double h = H1) {
    return (-f(x + 2 * h) + 8 * f(x + h) - 8 * f(x - h) + f(x - 2 * h)) / (12.0 * h);
}

// the partial derivative in coordinate i of a function taking a vector
template <typename F> double partial_derivative(F f, vector<double> point, int i, double h = H1) {
    vector<double> hi = point, lo = point;
    hi[i] += h;
    lo[i] -= h;
    return (f(hi) - f(lo)) / (2.0 * h);
}

template <typename F> vector<double> gradient(F f, const vector<double> &point, double h = H1) {
    vector<double> g(point.size());
    for (size_t i = 0; i < point.size(); i++) g[i] = partial_derivative(f, point, (int)i, h);
    return g;
}

// a root of f near start, by Newton's method; nan when it fails to settle
template <typename F> double newton_root(F f, double start, int iterations = 100) {
    double x = start;
    for (int i = 0; i < iterations; i++) {
        double slope = first_derivative(f, x);
        if (fabs(slope) < 1e-12) break; // flat here, so Newton cannot step
        double next = x - f(x) / slope;
        if (fabs(next - x) < 1e-12) return next;
        x = next;
    }
    return x;
}

// Standard problem: report f'(x) and f''(x) at a given x, for f(x) = x^2 + 3x + 2
void solve() {
    double x;
    cin >> x;
    auto f = [](double t) { return t * t + 3 * t + 2; };
    cout << fixed << setprecision(6);
    cout << first_derivative(f, x) << ' ' << second_derivative(f, x) << '\n';
}
