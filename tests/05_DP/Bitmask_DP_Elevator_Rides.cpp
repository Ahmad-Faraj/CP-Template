// Test for 05_DP/Bitmask_DP_Elevator_Rides.cpp against an O(3^n) set-partition DP: best[mask] =
// 1 + min over groups g containing mask's lowest person with weight(g) <= x of best[mask ^ g].
// Checks: elevator_rides on random small n, n = 0, n = 1, everyone in one ride, everyone alone,
// and weights near the CSES limit 1e9.
#include "../../05_DP/Bitmask_DP_Elevator_Rides.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(1653);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int brute(const vector<ll> &w, ll x) {
    int n = w.size(), full = (1 << n) - 1;
    vector<ll> sum(1 << n, 0);
    for (int m = 1; m <= full; m++) {
        int j = __builtin_ctz(m);
        sum[m] = sum[m & (m - 1)] + w[j];
    }
    vector<int> best(1 << n, INT_MAX);
    best[0] = 0;
    for (int m = 1; m <= full; m++) {
        int low = m & -m;
        for (int g = m; g; g = (g - 1) & m)
            if ((g & low) && sum[g] <= x && best[m ^ g] != INT_MAX)
                best[m] = min(best[m], best[m ^ g] + 1);
    }
    return best[full];
}

int main() {
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 11;
        ll x = vector<ll>{1, 2, 5, 10, 100, 1000000000}[rng() % 6];
        vector<ll> w(n);
        for (auto &v : w) v = rng() % x + 1;
        if (n && rng() % 4 == 0) w[rng() % n] = w[rng() % n]; // duplicates
        chk("elevator_rides(random)", elevator_rides(w, x) == brute(w, x));
    }
    chk("elevator_rides(n=0)", elevator_rides({}, 5) == 0);
    for (ll x = 1; x <= 10; x++)
        for (ll a = 1; a <= x; a++) chk("elevator_rides(n=1)", elevator_rides({a}, x) == 1);
    chk("elevator_rides(all fit)", elevator_rides(vector<ll>(12, 1), 12) == 1);
    chk("elevator_rides(all alone)", elevator_rides(vector<ll>(12, 7), 7) == 12);
    // CSES limit
    chk("elevator_rides(1e9 pair)", elevator_rides({1000000000, 1000000000}, 1000000000) == 2);
    for (int it = 0; it < 2000; it++) {
        int n = rng() % 9 + 1;
        ll x = 1000000000;
        vector<ll> w(n);
        for (auto &v : w) v = x - rng() % 3;
        chk("elevator_rides(near 1e9)", elevator_rides(w, x) == brute(w, x));
    }
    {
        vector<ll> w(20);
        for (auto &v : w) v = rng() % 1000000000 + 1;
        auto t0 = clock();
        int r = elevator_rides(w, 1000000000);
        printf("n=20 (CSES max): %.3fs (rides=%d)\n", double(clock() - t0) / CLOCKS_PER_SEC, r);
        w.resize(15);
        chk("elevator_rides(n=15)", elevator_rides(w, 1000000000) == brute(w, 1000000000));
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
