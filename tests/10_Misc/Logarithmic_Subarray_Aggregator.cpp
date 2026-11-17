// Test for 10_Misc/Logarithmic_Subarray_Aggregator.cpp against O(n^2) enumeration of all subarrays.
// The one public function, subarray_counts, is checked with gcd, AND, OR, on ll and int, with
// zeros, duplicates, n = 0 and n = 1, plus a values-only check of every achievable value.
#include "../../10_Misc/Logarithmic_Subarray_Aggregator.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(4751);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

template <typename T, typename Op> map<T, ll> brute(const vector<T> &a, Op op) {
    map<T, ll> res;
    for (size_t l = 0; l < a.size(); l++) {
        T cur = a[l];
        for (size_t r = l; r < a.size(); r++) {
            if (r > l) cur = op(cur, a[r]);
            res[cur]++;
        }
    }
    return res;
}

int main() {
    auto G = [](ll x, ll y) { return __gcd(x, y); };
    auto A = [](ll x, ll y) { return x & y; };
    auto O = [](ll x, ll y) { return x | y; };
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 25;
        ll mx = vector<ll>{1, 2, 8, 64, 1000, (ll)1e9, (ll)1e18}[rng() % 7];
        vector<ll> a(n);
        for (auto &x : a) x = rng() % (mx + 1);
        if (n && rng() % 3 == 0) // powers of two / duplicates make long runs
            for (auto &x : a) x = 1LL << (rng() % 5);
        auto g = subarray_counts(a, G), an = subarray_counts(a, A), o = subarray_counts(a, O);
        chk("subarray_counts(gcd)", g == brute(a, G));
        chk("subarray_counts(and)", an == brute(a, A));
        chk("subarray_counts(or)", o == brute(a, O));
        ll tot = 0;
        for (auto &[v, c] : g) tot += c;
        chk("total = n(n+1)/2", tot == (ll)n * (n + 1) / 2);
        // achievable values, as a set built with no counting
        set<ll> vals;
        for (int l = 0; l < n; l++)
            for (int r = l; r < n; r++) {
                ll cur = 0;
                for (int i = l; i <= r; i++) cur = __gcd(cur, a[i]);
                vals.insert(cur);
            }
        set<ll> got;
        for (auto &[v, c] : g) got.insert(v);
        chk("distinct values(gcd)", got == vals);
    }
    for (int it = 0; it < 5000; it++) { // int element type
        int n = rng() % 20;
        vector<int> a(n);
        for (auto &x : a) x = rng() % 50;
        auto op = [](int x, int y) { return __gcd(x, y); };
        chk("subarray_counts<int>(gcd)", subarray_counts(a, op) == brute(a, op));
    }
    { // all equal: one value, n(n+1)/2 subarrays (count exceeds int)
        int n = 200000;
        vector<ll> a(n, 6);
        auto t0 = clock();
        auto g = subarray_counts(a, G);
        chk("all equal large", g.size() == 1 && g[6] == (ll)n * (n + 1) / 2);
        vector<ll> b(n);
        for (auto &x : b) x = rng() % (ll)1e18 + 1;
        auto h = subarray_counts(b, G);
        printf("n=2e5 x2 gcd counts: %.3fs (%zu distinct)\n", double(clock() - t0) / CLOCKS_PER_SEC,
               h.size());
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
