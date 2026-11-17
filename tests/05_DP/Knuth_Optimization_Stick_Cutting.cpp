// Test for 05_DP/Knuth_Optimization_Stick_Cutting.cpp against (a) trying every order of the cuts
// for n <= 6 and (b) the plain O(n^3) interval DP for larger n.
// Checks: stick_cutting with sorted and unsorted cuts, n = 0, n = 1, UVA 10003 samples, lengths up
// to 1e9, and an n = 2000 timing run.
#include "../../05_DP/Knuth_Optimization_Stick_Cutting.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(10003);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

ll by_orders(ll len, vector<ll> cuts) { // every permutation of cut order, pay current piece length
    sort(cuts.begin(), cuts.end());
    ll best = LLONG_MAX;
    do {
        set<ll> pts{0, len};
        ll s = 0;
        for (ll c : cuts) {
            auto it = pts.upper_bound(c);
            s += *it - *prev(it);
            pts.insert(c);
        }
        best = min(best, s);
    } while (next_permutation(cuts.begin(), cuts.end()));
    return best;
}

ll cubic(ll len, vector<ll> cuts) {
    sort(cuts.begin(), cuts.end());
    cuts.insert(cuts.begin(), 0);
    cuts.push_back(len);
    int m = cuts.size();
    vector<vector<ll>> dp(m, vector<ll>(m, 0));
    for (int s = 2; s < m; s++)
        for (int i = 0; i + s < m; i++) {
            int j = i + s;
            dp[i][j] = LLONG_MAX;
            for (int k = i + 1; k < j; k++) dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j]);
            dp[i][j] += cuts[j] - cuts[i];
        }
    return dp[0][m - 1];
}

vector<ll> rand_cuts(int n, ll len) { // n distinct cuts in (0, len), shuffled
    set<ll> s;
    while ((int)s.size() < n) s.insert(rng() % (len - 1) + 1);
    vector<ll> v(s.begin(), s.end());
    shuffle(v.begin(), v.end(), rng);
    return v;
}

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 7;
        ll len = n + 1 + rng() % vector<ll>{1, 5, 50, 1000000000}[rng() % 4];
        auto c = rand_cuts(n, len);
        chk("stick_cutting(vs every cut order)", stick_cutting(len, c) == by_orders(len, c));
    }
    for (int it = 0; it < 3000; it++) {
        int n = rng() % 60;
        ll len = n + 1 + rng() % vector<ll>{1, 100, 1000000000}[rng() % 3];
        auto c = rand_cuts(n, len);
        chk("stick_cutting(vs O(n^3) dp)", stick_cutting(len, c) == cubic(len, c));
    }
    chk("stick_cutting(n = 0)", stick_cutting(10, {}) == 0);
    chk("stick_cutting(n = 1)", stick_cutting(10, {3}) == 10);
    chk("stick_cutting(UVA sample 1)", stick_cutting(100, {25, 50, 75}) == 200);
    chk("stick_cutting(UVA sample 2)", stick_cutting(10, {4, 5, 7, 8}) == 22);
    {
        int n = 2000;
        auto c = rand_cuts(n, 1000000000);
        auto t0 = clock();
        ll r = stick_cutting(1000000000, c);
        double t = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("n=2000 cuts: %.3fs (ans=%lld)\n", t, r);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-36s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
