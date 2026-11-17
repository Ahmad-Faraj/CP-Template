// Test for 05_DP/Digit_DP.cpp against direct counting.
// Every public function is checked: digit_dp (small ranges exhaustively, windows near 1e18, additivity).
#include "../../05_DP/Digit_DP.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(3);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

ll brute(ll l, ll r, ll K) {
    ll c = 0;
    for (ll x = l; x <= r; x++) {
        ll s = 0;
        for (ll y = x; y; y /= 10) s += y % 10;
        c += s % K == 0;
    }
    return c;
}

int main() {
    for (ll K = 1; K <= 12; K++)
        for (ll l = 0; l <= 250; l++)
            for (ll r = l; r <= 250; r += 3) chk("small", digit_dp(l, r, K) == brute(l, r, K));
    for (int it = 0; it < 3000; it++) {
        ll l = rng() % 1000000, r = l + rng() % 3000, K = rng() % 90 + 1;
        chk("medium", digit_dp(l, r, K) == brute(l, r, K));
    }
    for (int it = 0; it < 300; it++) {
        ll l = (ll)(rng() % 999999999999000000ULL) + 1, r = l + rng() % 2000, K = rng() % 90 + 1;
        if (rng() % 3 == 0) r = min<ll>(r, 999999999999999999LL), l = r - rng() % 1000;
        chk("near_1e18", digit_dp(l, r, K) == brute(l, r, K));
    }
    for (int it = 0; it < 2000; it++) {
        ll a = rng() % 1000000000000000000ULL, b = rng() % 1000000000000000000ULL, K = rng() % 90 + 1;
        if (a > b) swap(a, b);
        ll m = a + (b - a) / 2;
        chk("additive", digit_dp(a, b, K) == digit_dp(a, m, K) + (m < b ? digit_dp(m + 1, b, K) : 0));
    }
    chk("k=1 counts everything", digit_dp(0, 999999999999999999LL, 1) == 1000000000000000000LL);
    chk("single zero", digit_dp(0, 0, 7) == 1);
    {
        auto t0 = clock();
        unsigned long long s = 0;
        for (int i = 0; i < 1000; i++) s += digit_dp(1, 999999999999999999LL, 90);
        printf("1000 calls at r=1e18, k=90: %.3fs (checksum %llu)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
