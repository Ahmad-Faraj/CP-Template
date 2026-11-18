// Test for 07_Math/LDE_All_Solutions.cpp against counting (x, y) by a double loop over the box,
// and, for wide boxes, by walking x over the solution progression found by a brute residue search.
// Every public function is checked: extended_gcd, find_any_solution, shift_solution,
// find_all_solutions (incl. a = 0, b = 0, a = b = 0, empty boxes, negative coefficients).
#include "../../07_Math/LDE_All_Solutions.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(23);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rr(ll lo, ll hi) { return lo + (ll)(rng() % (hi - lo + 1)); }

int main() {
    for (int it = 0; it < 300000; it++) {
        ll a = rr(-12, 12), b = rr(-12, 12), c = rr(-60, 60);
        if (it % 50 == 0) a = 0;
        if (it % 70 == 0) b = 0;
        ll minx = rr(-25, 25), maxx = rr(-25, 25), miny = rr(-25, 25), maxy = rr(-25, 25);
        if (it % 4) {
            if (minx > maxx) swap(minx, maxx);
            if (miny > maxy) swap(miny, maxy);
        }
        ll cnt = 0;
        for (ll x = minx; x <= maxx; x++)
            for (ll y = miny; y <= maxy; y++) cnt += a * x + b * y == c;
        string tag = (a == 0 || b == 0) ? "find_all_solutions(zero coef)"
                     : (minx > maxx || miny > maxy) ? "find_all_solutions(empty box)" : "find_all_solutions";
        chk(tag, find_all_solutions(a, b, c, minx, maxx, miny, maxy) == cnt);
        ll x0, y0, g;
        bool got = find_any_solution(a, b, c, x0, y0, g);
        bool want = false;
        for (ll x = -60; x <= 60 && !want; x++) {
            ll r = c - a * x;
            want = b == 0 ? r == 0 : r % b == 0;
        }
        chk("find_any_solution", got == want && g == std::gcd(abs(a), abs(b)) && (!got || a * x0 + b * y0 == c));
        if (got && g) {
            ll x = x0, y = y0, k = rr(-1000, 1000);
            shift_solution(x, y, a / g, b / g, k);
            chk("shift_solution", a * x + b * y == c && x == x0 + k * (b / g));
        }
    }
    for (ll a = 0; a <= 40; a++)
        for (ll b = 0; b <= 40; b++) {
            ll x, y, g = extended_gcd(a, b, x, y);
            chk("extended_gcd", g == std::gcd(a, b) && a * x + b * y == g);
        }
    for (int it = 0; it < 400; it++) { // wide boxes, coefficients up to 1e6
        ll a = rr(-1000000, 1000000), b = rr(-1000000, 1000000);
        if (!a || !b) continue;
        ll g = std::gcd(abs(a), abs(b)), c = g * rr(-1000000, 1000000) + (it % 5 == 0 ? 1 : 0);
        ll minx = rr(-100000000, 0), maxx = rr(0, 100000000), miny = rr(-100000000, 0), maxy = rr(0, 100000000);
        ll cnt = 0, step = abs(b / g), xs = LLONG_MIN;
        for (ll x = 0; x < step; x++)
            if ((c - a * x) % b == 0) { xs = x; break; }
        if (xs != LLONG_MIN) {
            ll x = xs + ((minx - xs) / step - 1) * step;
            while (x < minx) x += step;
            for (; x <= maxx; x += step) {
                ll y = (c - a * x) / b;
                cnt += miny <= y && y <= maxy;
            }
        }
        chk("find_all_solutions(wide box)", find_all_solutions(a, b, c, minx, maxx, miny, maxy) == cnt);
    }
    {
        auto t0 = clock();
        ll s = 0;
        for (int i = 0; i < 1000000; i++)
            s += find_all_solutions(rr(-1e9, 1e9), rr(-1e9, 1e9), rr(-1e9, 1e9), -1e9, 1e9, -1e9, 1e9);
        printf("1e6 find_all_solutions (1e9 values): %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s % 10);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
