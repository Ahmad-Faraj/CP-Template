// Test for 07_Math/Extended_GCD.cpp against std::gcd, a brute search for any integer solution
// of a*x + b*y = c over a window, and __int128 identity checks for large inputs.
// Every public function is checked: extended_gcd, find_any_solution.
#include "../../07_Math/Extended_GCD.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(17);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

bool brute_solvable(ll a, ll b, ll c) {
    for (ll x = -200; x <= 200; x++) {
        ll r = c - a * x;
        if (b == 0 ? r == 0 : r % b == 0) return true;
    }
    return false;
}

int main() {
    for (ll a = 0; a <= 60; a++)
        for (ll b = 0; b <= 60; b++) {
            ll x, y, g = extended_gcd(a, b, x, y);
            chk("extended_gcd", g == std::gcd(a, b) && a * x + b * y == g);
        }
    for (int it = 0; it < 200000; it++) {
        ll a = (ll)(rng() >> 2), b = (ll)(rng() >> 2);
        if (it % 3 == 0) b = rng() % 1000;
        ll x, y, g = extended_gcd(a, b, x, y);
        bool ok = g == std::gcd(a, b) && (__int128)a * x + (__int128)b * y == g;
        if (a && b) ok &= abs(x) <= b && abs(y) <= a; // the classic size bound
        chk("extended_gcd(large)", ok);
    }
    for (ll a = -30; a <= 30; a++)
        for (ll b = -30; b <= 30; b++)
            for (ll c = -70; c <= 70; c += 1 + (rng() % 3)) {
                ll x0 = 7, y0 = 7, g = -1;
                bool got = find_any_solution(a, b, c, x0, y0, g);
                bool want = brute_solvable(a, b, c);
                bool ok = got == want && g == std::gcd(abs(a), abs(b));
                if (got) ok &= a * x0 + b * y0 == c;
                chk(a == 0 && b == 0 ? "find_any_solution(a=b=0)" : "find_any_solution", ok);
            }
    for (int it = 0; it < 200000; it++) { // |a|, |b|, |c| <= 1e9: the documented safe range
        ll a = (ll)(rng() % 2000000001) - 1000000000, b = (ll)(rng() % 2000000001) - 1000000000;
        ll g0 = std::gcd(abs(a), abs(b));
        ll c = it % 2 && g0 ? g0 * ((ll)(rng() % 2001) - 1000) : (ll)(rng() % 2000000001) - 1000000000;
        ll x0, y0, g;
        bool got = find_any_solution(a, b, c, x0, y0, g);
        bool want = g0 ? c % g0 == 0 : c == 0;
        bool ok = got == want && g == g0;
        if (got) ok &= (__int128)a * x0 + (__int128)b * y0 == c;
        chk("find_any_solution(1e9)", ok);
    }
    {
        auto t0 = clock();
        ll s = 0;
        for (int i = 0; i < 2000000; i++) {
            ll x, y, g;
            find_any_solution((ll)(rng() % 2000000001) - 1000000000, (ll)(rng() % 2000000001) - 1000000000,
                              (ll)(rng() % 1000000000), x, y, g);
            s += g;
        }
        printf("2e6 find_any_solution (1e9 values): %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s % 10);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
