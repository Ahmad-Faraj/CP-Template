// Test for 07_Math/LDE_N_Vars.cpp against exhaustive search over a box of integer solutions
// (small cases) and a naive gcd loop (large cases); every returned solution is plugged back in.
// Every public function is checked: extended_euclid, find_any_solution(a, b, c, x, y, g) and
// find_any_solution(vector a, c), including zeros, negatives, all-zero a, n = 1 and trailing zeros.
#include "../../07_Math/LDE_N_Vars.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(83);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }

ll naive_gcd(ll a, ll b) { // by repeated subtraction-free Euclid on |a|, |b|
    a = a < 0 ? -a : a, b = b < 0 ? -b : b;
    while (b) {
        ll t = a % b;
        a = b, b = t;
    }
    return a;
}
bool box_solvable(const vector<ll> &a, ll c, int B) { // any x in [-B, B]^n
    int n = a.size();
    vector<ll> x(n, -B);
    if (n == 0) return c == 0;
    while (true) {
        __int128 s = 0;
        for (int i = 0; i < n; i++) s += (__int128)a[i] * x[i];
        if (s == c) return true;
        int i = 0;
        while (i < n && x[i] == B) x[i] = -B, i++;
        if (i == n) return false;
        x[i]++;
    }
}
bool valid(const vector<ll> &a, ll c, const vector<ll> &x) {
    if (x.size() != a.size()) return false;
    __int128 s = 0;
    for (size_t i = 0; i < a.size(); i++) s += (__int128)a[i] * x[i];
    return s == c;
}

int main() {
    // extended_euclid and the two-variable solver, small values against brute force
    for (ll a = -12; a <= 12; a++)
        for (ll b = -12; b <= 12; b++) {
            if (a >= 0 && b >= 0) {
                ll x, y, g = extended_euclid(a, b, x, y);
                chk("extended_euclid", g == naive_gcd(a, b) && a * x + b * y == g);
            }
            for (ll c = -30; c <= 30; c++) {
                ll x = 7, y = 7, g = 7;
                bool got = find_any_solution(a, b, c, x, y, g);
                bool want = box_solvable({a, b}, c, 45);
                chk("find_any_solution(a,b,c)", got == want && (!got || (a * x + b * y == c &&
                                                                            g == naive_gcd(a, b))));
            }
        }
    for (int it = 0; it < 100000; it++) {
        ll a = rnd(-1e9, 1e9), b = rnd(-1e9, 1e9), c = rnd(-1e9, 1e9);
        if (it % 4 == 0) c = naive_gcd(a, b) * rnd(-1000, 1000);
        if (it % 50 == 0) a = 0;
        ll x, y, g;
        bool got = find_any_solution(a, b, c, x, y, g);
        ll G = naive_gcd(a, b);
        bool want = G == 0 ? c == 0 : c % G == 0;
        chk("find_any_solution(a,b,c) big", got == want && (!got || (__int128)a * x + (__int128)b * y == c));
        if (a >= 0 && b >= 0) {
            ll gg = extended_euclid(a, b, x, y);
            chk("extended_euclid big", gg == G && (__int128)a * x + (__int128)b * y == G);
        }
    }
    // n variables: small, exhaustive box
    for (int it = 0; it < 6000; it++) {
        int n = rnd(0, 3);
        vector<ll> a(n);
        for (auto &v : a) v = rng() % 4 == 0 ? 0 : rnd(-6, 6);
        if (n >= 2 && rng() % 4 == 0) a.back() = 0; // trailing zeros crashed main's version
        ll c = rnd(-15, 15);
        auto x = find_any_solution(a, c);
        bool want = box_solvable(a, c, n == 3 ? 21 : 30);
        if (n == 0) chk("find_any_solution(n=0)", x.empty());
        else chk("find_any_solution(vec)", want ? valid(a, c, x) : x.empty());
    }
    // n = 4..5, values a bit larger: existence by gcd (each n = 4 case confirmed by a small box)
    for (int it = 0; it < 3000; it++) {
        int n = rnd(4, 5);
        vector<ll> a(n);
        for (auto &v : a) v = rng() % 5 == 0 ? 0 : rnd(-20, 20);
        ll c = rnd(-50, 50), G = 0;
        for (ll v : a) G = naive_gcd(G, v);
        bool want = G == 0 ? c == 0 : c % G == 0;
        if (n == 4 && it % 10 == 0) chk("oracle:box implies gcd", !box_solvable(a, c, 6) || want);
        auto x = find_any_solution(a, c);
        chk("find_any_solution(vec,n=4..5)", want ? valid(a, c, x) : x.empty());
    }
    // large: |a|, |c| <= 1e9, n up to 50, solution checked in __int128
    for (int it = 0; it < 20000; it++) {
        int n = rnd(1, 50);
        vector<ll> a(n);
        ll mult = vector<ll>{1, 2, 6, 1000}[rng() % 4];
        for (auto &v : a) v = rng() % 10 == 0 ? 0 : rnd(-1e9 / mult, 1e9 / mult) * mult;
        ll c = rnd(-1e9, 1e9), G = 0;
        for (ll v : a) G = naive_gcd(G, v);
        if (it % 2 && G) c = c / G * G;
        bool want = G == 0 ? c == 0 : c % G == 0;
        auto x = find_any_solution(a, c);
        chk("find_any_solution(vec) big", want ? valid(a, c, x) : x.empty());
    }
    // adversarial: suffix gcds grow to the right (a[i] = r * product of the first i primes), |a| <= 1e9
    const ll PRS[] = {2, 3, 5, 7, 11, 13, 17, 19, 23};
    for (int it = 0; it < 20000; it++) {
        int n = rnd(2, 10);
        vector<ll> a(n);
        for (int i = 0; i < n; i++) {
            ll G = 1;
            for (int k = 0; k < min<int>(i, 9); k++) G *= PRS[k];
            a[i] = rnd(1, 1e9 / G) * G * (rng() % 2 ? 1 : -1);
        }
        if (rng() % 2) reverse(a.begin(), a.end());
        ll c = rnd(-1e9, 1e9), G = 0;
        for (ll v : a) G = naive_gcd(G, v);
        if (it % 2) c = c / G * G;
        auto x = find_any_solution(a, c);
        chk("find_any_solution(vec) chains", c % G == 0 ? valid(a, c, x) : x.empty());
    }
    {
        auto t0 = clock();
        ll s = 0;
        for (int it = 0; it < 20; it++) {
            vector<ll> a(100000);
            for (auto &v : a) v = rnd(1, 1e9);
            auto x = find_any_solution(a, rnd(-1e9, 1e9));
            s += x.size();
        }
        printf("20 x find_any_solution(n = 1e5, |a| <= 1e9): %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
