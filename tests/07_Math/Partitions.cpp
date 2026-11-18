// Test for 07_Math/Partitions.cpp against the O(n^2) coin-change DP and known values.
// Every public function is checked: partitions(n, mod) for several moduli, n = 0, exact values,
// and the ACPC 2025 H use (partitions with no part divisible by 4 = p * prod(1 - x^(4k))).
#include "../../07_Math/Partitions.cpp"

map<string, pair<long long, long long>> cov;
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

vector<ll> dp_count(int n, ll mod, int skip_mult = 0) { // parts 1..n, optionally skipping multiples
    vector<ll> d(n + 1, 0);
    d[0] = 1 % mod;
    for (int part = 1; part <= n; part++) {
        if (skip_mult && part % skip_mult == 0) continue;
        for (int s = part; s <= n; s++) d[s] = (d[s] + d[s - part]) % mod;
    }
    return d;
}

int main() {
    for (ll mod : {1000000007LL, 998244353LL, 2LL, 7LL, 1000000000000000003LL}) {
        int n = mod > 100 ? 2500 : 600;
        auto p = partitions(n, mod), d = dp_count(n, mod);
        bool ok = true;
        for (int i = 0; i <= n; i++) ok &= p[i] == d[i];
        chk("vs dp mod " + to_string(mod), ok);
    }
    {
        auto p = partitions(400, 4000000000000000000LL); // exact for i <= 380 (p(i) < 4e18)
        chk("p(5) = 7", p[5] == 7);
        chk("p(100) = 190569292", p[100] == 190569292);
        chk("p(200) = 3972999029388", p[200] == 3972999029388LL);
        bool ok = true;
        auto d = dp_count(400, 4000000000000000000LL);
        for (int i = 0; i <= 400; i++) ok &= p[i] == d[i];
        chk("vs dp mod 4e18 up to 400", ok);
    }
    {
        auto p = partitions(0, 1000000007);
        chk("n = 0", p.size() == 1 && p[0] == 1);
        chk("mod 1", partitions(10, 1)[7] == 0);
    }
    { // ACPC 2025 H: no part divisible by 4 = p(x) * prod (1 - x^(4k)), the product via pentagonal
        const ll M = 1000000007;
        int n = 3000;
        auto p = partitions(n, M);
        vector<ll> e(n + 1, 0); // prod (1 - x^(4k)) = sum (-1)^k x^(4 * k(3k-1)/2), k in Z
        for (ll k = -100; k <= 100; k++) {
            ll g = 4 * (k * (3 * k - 1) / 2);
            if (g <= n) e[g] = (e[g] + (k % 2 ? M - 1 : 1)) % M;
        }
        vector<ll> res(n + 1, 0);
        for (int i = 0; i <= n; i++)
            if (e[i])
                for (int j = 0; i + j <= n; j++) res[i + j] = (res[i + j] + e[i] * p[j]) % M;
        auto d = dp_count(n, M, 4);
        chk("no part divisible by 4 (ACPC 2025 H)", res == d && res[4] == 4);
    }
    {
        auto t0 = clock();
        auto p = partitions(500000, 1000000007);
        printf("n = 5e5: %.3fs (p[n] = %lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, p[500000]);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-40s checks=%-6lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
