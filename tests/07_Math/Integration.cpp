// Test for 07_Math/Integration.cpp against closed-form integrals (numeric, so with tolerances).
// Every public function is checked: simpson_integration with the default n and with explicit n
// (even, odd, small), reversed intervals, a == b, and a stateful lambda.
#include "../../07_Math/Integration.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(23);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
double rnd(double lo, double hi) { return lo + (hi - lo) * (rng() >> 11) * (1.0 / (1ULL << 53)); }

struct Case {
    string name;
    function<double(double)> f, F; // F' = f
    double lo, hi;
};

int main() {
    vector<Case> cs = {
        {"quad", [](double x) { return x * x + x + 1; }, [](double x) { return x * x * x / 3 + x * x / 2 + x; }, -50, 50},
        {"quartic", [](double x) { return x * x * x * x; }, [](double x) { return pow(x, 5) / 5; }, -10, 10},
        {"sin", [](double x) { return sin(x); }, [](double x) { return -cos(x); }, -20, 20},
        {"exp", [](double x) { return exp(x); }, [](double x) { return exp(x); }, -5, 5},
        {"inv", [](double x) { return 1 / x; }, [](double x) { return log(x); }, 0.01, 100},
        {"log", [](double x) { return log(x); }, [](double x) { return x * log(x) - x; }, 0.01, 100},
        {"sin_deg", [](double x) { return sin(x * M_PI / 180); },
         [](double x) { return -cos(x * M_PI / 180) * 180 / M_PI; }, -720, 720},
    };
    double worst_def = 0, worst_small = 0;
    for (auto &c : cs)
        for (int it = 0; it < 60; it++) {
            double a = rnd(c.lo, c.hi), b = rnd(c.lo, c.hi);
            if (it == 0) a = c.lo, b = c.hi;
            double want = c.F(b) - c.F(a), scale = max(1.0, fabs(want));
            double got = simpson_integration(c.f, a, b);
            worst_def = max(worst_def, fabs(got - want) / scale);
            chk("simpson_integration", fabs(got - want) <= 1e-9 * scale);
            chk("simpson_integration(reversed)", fabs(simpson_integration(c.f, b, a) + want) <= 1e-9 * scale);
            bool smooth = c.name != "inv" && c.name != "log";
            if (smooth) { // n = 1e4 is enough for smooth f on a short range
                double g = simpson_integration(c.f, a, b, 10000);
                worst_small = max(worst_small, fabs(g - want) / scale);
                chk("simpson_integration(n=1e4)", fabs(g - want) <= 1e-6 * scale);
                chk("simpson_integration(odd n)", fabs(simpson_integration(c.f, a, b, 10001) - want) <= 1e-6 * scale);
            }
        }
    // Simpson is exact for cubics, even with n = 2 (and odd n = 1 is rounded up to 2)
    for (int it = 0; it < 2000; it++) {
        double p[4];
        for (double &v : p) v = rnd(-10, 10);
        auto f = [&](double x) { return ((p[3] * x + p[2]) * x + p[1]) * x + p[0]; };
        auto F = [&](double x) { return (((p[3] / 4 * x + p[2] / 3) * x + p[1] / 2) * x + p[0]) * x; };
        double a = rnd(-10, 10), b = rnd(-10, 10), want = F(b) - F(a);
        chk("simpson_integration(cubic,n=2)", fabs(simpson_integration(f, a, b, 2) - want) <= 1e-9 * max(1.0, fabs(want)));
        chk("simpson_integration(cubic,n=1)", fabs(simpson_integration(f, a, b, 1) - want) <= 1e-9 * max(1.0, fabs(want)));
        chk("simpson_integration(a==b)", simpson_integration(f, a, a, 100) == 0);
    }
    printf("worst relative error: default n %.2e, n = 1e4 %.2e\n", worst_def, worst_small);
    {
        auto t0 = clock();
        double s = 0;
        for (int i = 0; i < 20; i++) s += simpson_integration([](double x) { return sin(x) * x; }, 0, i);
        printf("20 integrations with n = 1e6: %.3fs (%.3f)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
