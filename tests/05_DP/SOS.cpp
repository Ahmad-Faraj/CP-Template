// Test for 05_DP/SOS.cpp against summing f over every submask / supermask by direct enumeration.
// Checks: sum_over_subsets, sum_over_supersets (ll and int, negatives, B = 0..10), the CSES 1654
// recipe from the header against a pairwise count, and a B = 20 timing run.
#include "../../05_DP/SOS.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(1654);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
using ll = long long;

int main() {
    for (int it = 0; it < 3000; it++) {
        int B = rng() % 11, n = 1 << B;
        vector<ll> f(n);
        for (auto &v : f) v = (ll)(rng() % 2001) - 1000;
        vector<ll> sub = f, sup = f, wsub(n, 0), wsup(n, 0);
        for (int x = 0; x < n; x++)
            for (int y = 0; y < n; y++) {
                if ((y & x) == y) wsub[x] += f[y];
                if ((y & x) == x) wsup[x] += f[y];
            }
        sum_over_subsets(sub);
        sum_over_supersets(sup);
        chk("sum_over_subsets", sub == wsub);
        chk("sum_over_supersets", sup == wsup);
        vector<int> fi(n);
        for (int x = 0; x < n; x++) fi[x] = f[x];
        vector<int> si = fi, pi = fi;
        sum_over_subsets(si);
        sum_over_supersets(pi);
        bool ok1 = true, ok2 = true;
        for (int x = 0; x < n; x++) ok1 &= si[x] == wsub[x], ok2 &= pi[x] == wsup[x];
        chk("sum_over_subsets<int>", ok1);
        chk("sum_over_supersets<int>", ok2);
    }
    // CSES 1654 recipe: for each x count y with x|y == x, x&y == x, x&y != 0
    for (int it = 0; it < 300; it++) {
        int B = rng() % 9 + 1, n = rng() % 60 + 1, full = (1 << B) - 1;
        vector<int> a(n);
        for (auto &v : a) v = rng() % (1 << B);
        vector<ll> sub(1 << B, 0), sup(1 << B, 0);
        for (int v : a) sub[v]++, sup[v]++;
        sum_over_subsets(sub);
        sum_over_supersets(sup);
        bool ok = true;
        for (int x : a) {
            ll c1 = 0, c2 = 0, c3 = 0;
            for (int y : a) c1 += (x | y) == x, c2 += (x & y) == x, c3 += (x & y) != 0;
            ok &= sub[x] == c1 && sup[x] == c2 && n - sub[~x & full] == c3;
        }
        chk("CSES 1654 recipe", ok);
    }
    {
        vector<ll> f(1 << 20);
        for (auto &v : f) v = rng() % 1000;
        auto t0 = clock();
        sum_over_subsets(f);
        sum_over_supersets(f);
        double t = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("B=20 subsets + supersets: %.3fs (f[0]=%lld)\n", t, f[0]);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
