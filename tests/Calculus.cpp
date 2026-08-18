// Brute-force test for:
//   Math/Calculus/Differentiation.cpp
//   Math/Calculus/Integration.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;

namespace DI {
#include "../Math/Calculus/Differentiation.cpp"
}
namespace IN {
#include "../Math/Calculus/Integration.cpp"
}

int main() {
    // Every check compares against the CLOSED FORM, computed by hand, not against another
    // numerical method. Tolerances reflect what each scheme can actually deliver.
    int d1Fail = 0, d2Fail = 0, d5Fail = 0, gradFail = 0, newtonFail = 0;
    int simpFail = 0, adaptFail = 0, trapFail = 0, betweenFail = 0, edgeFail = 0;
    int d1Checked = 0, intChecked = 0;

    struct Case {
        const char *name;
        function<double(double)> f, df, d2f, F; // f, f', f'', and an antiderivative
    };
    vector<Case> cases = {
        {"x^2+3x+2", [](double x) { return x * x + 3 * x + 2; }, [](double x) { return 2 * x + 3; },
         [](double) { return 2.0; }, [](double x) { return x * x * x / 3 + 1.5 * x * x + 2 * x; }},
        {"sin", [](double x) { return sin(x); }, [](double x) { return cos(x); }, [](double x) { return -sin(x); },
         [](double x) { return -cos(x); }},
        {"exp", [](double x) { return exp(x); }, [](double x) { return exp(x); }, [](double x) { return exp(x); },
         [](double x) { return exp(x); }},
        {"x^4-2x", [](double x) { return x * x * x * x - 2 * x; }, [](double x) { return 4 * x * x * x - 2; },
         [](double x) { return 12 * x * x; }, [](double x) { return x * x * x * x * x / 5 - x * x; }},
    };

    for (auto &c : cases) {
        for (double x = -2.0; x <= 2.0; x += 0.05) {
            double got1 = DI::first_derivative(c.f, x);
            if (fabs(got1 - c.df(x)) > 1e-5 * max(1.0, fabs(c.df(x)))) d1Fail++;
            double got5 = DI::first_derivative_5point(c.f, x);
            if (fabs(got5 - c.df(x)) > 1e-6 * max(1.0, fabs(c.df(x)))) d5Fail++;
            double got2 = DI::second_derivative(c.f, x);
            if (fabs(got2 - c.d2f(x)) > 1e-3 * max(1.0, fabs(c.d2f(x)))) d2Fail++;
            d1Checked++;
        }
        // integrals against the antiderivative
        for (double a = -1.5; a <= 1.5; a += 0.25)
            for (double b = a; b <= 1.5; b += 0.5) {
                double want = c.F(b) - c.F(a);
                if (fabs(IN::simpson(c.f, a, b, 10000) - want) > 1e-7 * max(1.0, fabs(want))) simpFail++;
                if (fabs(IN::adaptive_simpson(c.f, a, b) - want) > 1e-7 * max(1.0, fabs(want))) adaptFail++;
                if (fabs(IN::trapezoid(c.f, a, b, 200000) - want) > 1e-5 * max(1.0, fabs(want))) trapFail++;
                intChecked++;
            }
    }

    // gradient of f(x, y) = x^2 * y + 3y, whose gradient is (2xy, x^2 + 3)
    {
        auto f = [](const vector<double> &v) { return v[0] * v[0] * v[1] + 3 * v[1]; };
        for (double x = -2; x <= 2; x += 0.5)
            for (double y = -2; y <= 2; y += 0.5) {
                vector<double> g = DI::gradient(f, vector<double>{x, y});
                if (fabs(g[0] - 2 * x * y) > 1e-4 * max(1.0, fabs(2 * x * y))) gradFail++;
                if (fabs(g[1] - (x * x + 3)) > 1e-4 * max(1.0, fabs(x * x + 3))) gradFail++;
            }
    }

    // Newton: roots of x^2 - 2 are +/- sqrt(2); of cos(x) is pi/2
    {
        auto f = [](double x) { return x * x - 2; };
        if (fabs(DI::newton_root(f, 1.0) - sqrt(2.0)) > 1e-8) newtonFail++;
        if (fabs(DI::newton_root(f, -1.0) + sqrt(2.0)) > 1e-8) newtonFail++;
        auto g = [](double x) { return cos(x); };
        if (fabs(DI::newton_root(g, 1.0) - acos(-1.0) / 2) > 1e-8) newtonFail++;
    }

    // area between curves: x^2 and x over [0, 1] is 1/2 - 1/3 = 1/6, with x above
    {
        auto up = [](double x) { return x; };
        auto lo = [](double x) { return x * x; };
        if (fabs(IN::area_between(up, lo, 0.0, 1.0) - 1.0 / 6.0) > 1e-9) betweenFail++;
    }

    // edge cases
    {
        auto f = [](double x) { return x * x; };
        if (IN::simpson(f, 2.0, 2.0) != 0.0) edgeFail++;             // empty interval
        if (IN::adaptive_simpson(f, 2.0, 2.0) != 0.0) edgeFail++;
        // reversed interval flips the sign
        double fwd = IN::simpson(f, 0.0, 1.0, 10000), rev = IN::simpson(f, 1.0, 0.0, 10000);
        if (fabs(fwd + rev) > 1e-9) edgeFail++;
        // an ODD step count must still work, since Simpson forces it even
        if (fabs(IN::simpson(f, 0.0, 3.0, 999) - 9.0) > 1e-6) edgeFail++;
        // a sharply curved function where adaptive should still hit the closed form
        auto sharp = [](double x) { return 1.0 / (x + 1.5); };
        double want = log(2.5) - log(0.5); // integral of 1/(x+1.5) over [-1, 1]
        if (fabs(IN::adaptive_simpson(sharp, -1.0, 1.0) - want) > 1e-8) adaptFail++;
    }

    cout << "Calculus\n";
    cout << "  first_derivative vs closed form  : " << d1Fail << " failures (" << d1Checked << " points)\n";
    cout << "  first_derivative_5point          : " << d5Fail << " failures\n";
    cout << "  second_derivative vs closed form : " << d2Fail << " failures\n";
    cout << "  gradient of a 2-variable function: " << gradFail << " failures\n";
    cout << "  newton_root                      : " << newtonFail << " failures\n";
    cout << "  simpson vs antiderivative        : " << simpFail << " failures (" << intChecked << " intervals)\n";
    cout << "  adaptive_simpson                 : " << adaptFail << " failures\n";
    cout << "  trapezoid                        : " << trapFail << " failures\n";
    cout << "  area_between                     : " << betweenFail << " failures\n";
    cout << "  edge cases (empty, reversed, odd): " << edgeFail << " failures\n";
}
