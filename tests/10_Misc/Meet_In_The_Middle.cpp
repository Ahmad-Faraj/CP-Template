// Test for 10_Misc/Meet_In_The_Middle.cpp against direct 2^n subset enumeration.
// Every public function is checked: subset_sums (as a multiset, any [l, r] including empty) and
// count_subsets_with_sum (n = 0..15, negatives, zeros, duplicates, x = 0, unreachable x).
#include "../../10_Misc/Meet_In_The_Middle.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(1628);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (int it = 0; it < 8000; it++) {
        int n = rng() % 16;
        ll lim = vector<ll>{1, 3, 10, 1000, (ll)1e12}[rng() % 5];
        bool neg = rng() % 2;
        vector<ll> a(n);
        for (auto &x : a) x = neg ? (ll)(rng() % (2 * lim + 1)) - lim : (ll)(rng() % (lim + 1));
        map<ll, ll> cnt; // oracle: every subset, directly
        for (int m = 0; m < (1 << n); m++) {
            ll s = 0;
            for (int i = 0; i < n; i++)
                if (m >> i & 1) s += a[i];
            cnt[s]++;
        }
        for (int q = 0; q < 6; q++) {
            ll x;
            if (q == 0) x = 0;
            else if (q % 2) x = next(cnt.begin(), rng() % cnt.size())->first; // reachable
            else x = (ll)(rng() % (4 * lim + 3)) - 2 * lim - 1;
            ll want = cnt.count(x) ? cnt[x] : 0;
            chk("count_subsets_with_sum", count_subsets_with_sum(a, x) == want);
        }
        int l = n ? rng() % n : 0, r = n ? l - 1 + rng() % (n - l + 1) : -1; // r = l-1: empty
        multiset<ll> want;
        for (int m = 0; m < (1 << (r - l + 1)); m++) {
            ll s = 0;
            for (int i = l; i <= r; i++)
                if (m >> (i - l) & 1) s += a[i];
            want.insert(s);
        }
        auto got = subset_sums(a, l, r);
        chk("subset_sums", multiset<ll>(got.begin(), got.end()) == want);
    }
    { // n = 40: all ones, answer C(40, 20); all zeros, x = 0 gives 2^40
        vector<ll> a(40, 1);
        auto t0 = clock();
        ll c = count_subsets_with_sum(a, 20);
        chk("n=40 binomial", c == 137846528820LL);
        chk("n=40 zeros", count_subsets_with_sum(vector<ll>(40, 0), 0) == (1LL << 40));
        vector<ll> b(40);
        for (auto &x : b) x = rng() % (ll)1e9;
        count_subsets_with_sum(b, (ll)1e10);
        printf("three n=40 runs: %.3fs\n", double(clock() - t0) / CLOCKS_PER_SEC);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
