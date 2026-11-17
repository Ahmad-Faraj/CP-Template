#include <bits/stdc++.h>
using namespace std;

// o(N) - N can be smaller with smooth functions
// 1e4 (smooth) : Polys, sin, cos, exp, Const, linear, Short interval,
// 1e6 (curvature) : log, tan, 1/x, Long interval

const int N = 1'000'000;

double f(double x) { return x * x + x + 1; }

double f_rad(double x) { return sin(x); }

double f_deg(double x) { return sin(x * M_PI / 180.0); }

double simpson_integration(double a, double b) {
    double h = (b - a) / N;
    double s = f(a) + f(b);
    for (int i = 1; i < N; ++i) {
        double x = a + h * i;
        s += f(x) * ((i & 1) ? 4.0 : 2.0);
    }
    return s * (h / 3.0);
}

int main() {
    double result = simpson_integration(1.0, 3.0);
    cout << fixed << setprecision(9) << result << "\n";
}
