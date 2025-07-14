#include <bits/stdc++.h>
using namespace std;

const double h = 1e-6;

// Example function: f(x) = x^2 + 3x + 2
double f(double x) { return x * x + 3 * x + 2; }

// First derivative: f'(x) ≈ (f(x + h) - f(x - h)) / (2h)
double first_derivative(double x) { return (f(x + h) - f(x - h)) / (2.0 * h); }

// Second derivative: f''(x) ≈ (f(x + h) - 2f(x) + f(x - h)) / (h^2)
double second_derivative(double x) { return (f(x + h) - 2.0 * f(x) + f(x - h)) / (h * h); }

int main() {
    double x;
    cin >> x;
    cout << fixed << setprecision(10);
    cout << first_derivative(x) << '\n';
    return 0;
}
