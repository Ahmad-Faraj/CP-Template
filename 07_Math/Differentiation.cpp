// Differentiation: numerical 1st and 2nd derivative of a function at a point.
// Use when: slope / rate of change of a function you can only evaluate, Newton steps, flat points.
// Usage: auto f = [](double x) { return x * x; };  first_derivative(f, x); second_derivative(f, x);
// Time: O(1): two or three calls to f
// Indexing: not applicable
// Note: the 2nd derivative needs a LARGER step h (default 1e-4; 1st uses 1e-6). Accuracy:
//       |err| <= 1e-7 |f'| + 1e-9 |f| (1st) and <= 1e-5 |f''| + 1e-7 |f| (2nd).

#include <bits/stdc++.h>
using namespace std;

template <class F> double first_derivative(F f, double x, double h = 1e-6) {
    return (f(x + h) - f(x - h)) / (2.0 * h);
}

template <class F> double second_derivative(F f, double x, double h = 1e-4) {
    return (f(x + h) - 2.0 * f(x) + f(x - h)) / (h * h);
}
