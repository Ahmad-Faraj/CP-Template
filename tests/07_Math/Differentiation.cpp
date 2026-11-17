// Test for 07_Math/Differentiation.cpp against closed-form derivatives (numeric, so with tolerances).
// Every public function is checked: first_derivative and second_derivative, with the default step
// and with an explicit step, on polynomials, sin, exp, log, sqrt and x^3 at |x| up to 1e3.
#include "../../07_Math/Differentiation.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(11);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
double rnd(double lo, double hi) { return lo + (hi - lo) * (rng() >> 11) * (1.0 / (1ULL << 53)); }
bool close(double got, double want, double rel) { return fabs(got - want) <= rel * max(1.0, fabs(want)); }

struct Case {
    string name;
    function<double(double)> f, d1, d2;
    double lo, hi;
};

int main() {
    vector<Case> cs = {
        {"poly", [](double x) { return x * x + 3 * x + 2; }, [](double x) { return 2 * x + 3; },
         [](double) { return 2.0; }, -1000, 1000},
        {"cubic", [](double x) { return x * x * x - 5 * x; }, [](double x) { return 3 * x * x - 5; },
         [](double x) { return 6 * x; }, -1000, 1000},
        {"sin", [](double x) { return sin(x); }, [](double x) { return cos(x); },
         [](double x) { return -sin(x); }, -10, 10},
        {"exp", [](double x) { return exp(x); }, [](double x) { return exp(x); },
         [](double x) { return exp(x); }, -5, 5},
        {"log", [](double x) { return log(x); }, [](double x) { return 1 / x; },
         [](double x) { return -1 / (x * x); }, 0.5, 100},
        {"sqrt", [](double x) { return sqrt(x); }, [](double x) { return 0.5 / sqrt(x); },
         [](double x) { return -0.25 / (x * sqrt(x)); }, 0.5, 100},
    };
    double worst1 = 0, worst2 = 0;
    for (auto &c : cs)
        for (int it = 0; it < 20000; it++) {
            double x = it == 0 ? c.lo : it == 1 ? c.hi : rnd(c.lo, c.hi);
            double g1 = first_derivative(c.f, x), w1 = c.d1(x);
            double g2 = second_derivative(c.f, x), w2 = c.d2(x);
            // documented error model: truncation (relative) + rounding ~ eps * |f| / h (or / h^2)
            double fx = fabs(c.f(x));
            double t1 = 1e-7 * max(1.0, fabs(w1)) + 1e-15 * fx / 1e-6;
            double t2 = 1e-5 * max(1.0, fabs(w2)) + 1e-15 * fx / 1e-8;
            worst1 = max(worst1, fabs(g1 - w1) / t1), worst2 = max(worst2, fabs(g2 - w2) / t2);
            chk("first_derivative", fabs(g1 - w1) <= t1);
            chk("second_derivative", fabs(g2 - w2) <= t2);
            double h1 = 1e-5, h2 = 1e-3; // explicit steps: truncation grows as h^2
            chk("first_derivative(h)", fabs(first_derivative(c.f, x, h1) - w1) <=
                                           1e-5 * max(1.0, fabs(w1)) + 1e-15 * fx / h1);
            chk("second_derivative(h)", fabs(second_derivative(c.f, x, h2) - w2) <=
                                            1e-3 * max(1.0, fabs(w2)) + 1e-15 * fx / (h2 * h2));
        }
    // a plain function pointer and a stateful lambda
    double k = 2.5;
    auto g = [&](double x) { return k * x * x; };
    chk("first_derivative(lambda)", close(first_derivative(g, 3.0), 15.0, 1e-8));
    chk("second_derivative(lambda)", close(second_derivative(g, 3.0), 5.0, 1e-6));
    double (*fp)(double) = [](double x) { return x * x * x; };
    chk("first_derivative(fnptr)", close(first_derivative(fp, 2.0), 12.0, 1e-8));
    chk("second_derivative(fnptr)", close(second_derivative(fp, 2.0), 12.0, 1e-6));
    printf("worst error / tolerance: first %.3f, second %.3f\n", worst1, worst2);
    {
        auto t0 = clock();
        double s = 0;
        for (int i = 0; i < 10000000; i++) s += first_derivative([](double x) { return sin(x); }, i * 1e-7);
        printf("1e7 first_derivative calls: %.3fs (%.3f)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
