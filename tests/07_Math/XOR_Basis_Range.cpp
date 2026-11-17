// Test for 07_Math/XOR_Basis_Range.cpp against brute force over every subset of a[l..r].
// Usage pattern under test: add a[0..r] in order, then query the suffix starting at l (offline by r).
// Every public method is checked: add, rank_from, max_xor_from, can_make_from, distinct_count_from, count.
#include "../../07_Math/XOR_Basis_Range.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(2025);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (int it = 0; it < 6000; it++) {
        int n = rng() % 10 + 1;
        int bits = vector<int>{2, 3, 5, 8, 20, 62}[rng() % 6];
        vector<ll> a(n);
        for (auto &x : a) x = (ll)(rng() & ((1ULL << bits) - 1));
        if (rng() % 3 == 0) a[rng() % n] = 0;
        XOR_Basis_Range<> b;
        for (int r = 0; r < n; r++) {
            b.add(a[r], r);
            chk("count", b.count == r + 1);
            for (int l = 0; l <= r; l++) {
                set<ll> vals;
                int len = r - l + 1;
                for (int m = 0; m < (1 << len); m++) {
                    ll x = 0;
                    for (int i = 0; i < len; i++)
                        if (m >> i & 1) x ^= a[l + i];
                    vals.insert(x);
                }
                int rk = 0;
                while ((1LL << rk) < (ll)vals.size()) rk++;
                chk("rank_from", b.rank_from(l) == rk);
                chk("distinct_count_from", b.distinct_count_from(l) == (ll)vals.size());
                chk("max_xor_from", b.max_xor_from(l) == *vals.rbegin());
                for (int q = 0; q < 4; q++) {
                    ll x = q % 2 ? *next(vals.begin(), rng() % vals.size()) : (ll)(rng() & ((1ULL << bits) - 1));
                    chk("can_make_from", b.can_make_from(x, l) == (bool)vals.count(x));
                }
            }
        }
    }
    // CF 1100F shape: n = q = 5e5, values < 2^20, queries sorted by r
    {
        int n = 500000, q = 500000;
        vector<int> a(n);
        for (auto &x : a) x = rng() % (1 << 20);
        vector<array<int, 2>> qs(q);
        for (auto &[l, r] : qs) {
            l = rng() % n, r = rng() % n;
            if (l > r) swap(l, r);
        }
        vector<int> order(q);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) { return qs[i][1] < qs[j][1]; });
        auto t0 = clock();
        XOR_Basis_Range<int, 20> b;
        long long s = 0;
        int r = -1;
        for (int i : order) {
            while (r < qs[i][1]) ++r, b.add(a[r], r);
            s += b.max_xor_from(qs[i][0]);
        }
        printf("n=q=5e5 offline: %.3fs (checksum %lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
