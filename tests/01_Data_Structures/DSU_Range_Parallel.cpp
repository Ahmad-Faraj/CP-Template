// Test for 01_Data_Structures/DSU_Range_Parallel.cpp against a naive DSU doing len single unions.
// Every public method is checked: unite (with and without callback), find, same, size, components.
#include "../../01_Data_Structures/DSU_Range_Parallel.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(7);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

struct Naive {
    vector<int> lab; // class label per element, relabelled on merge
    Naive(int n) : lab(n) { iota(lab.begin(), lab.end(), 0); }
    void unite(int a, int b, int len) {
        for (int i = 0; i < len; i++) {
            int x = lab[a + i], y = lab[b + i];
            if (x != y)
                for (auto &v : lab)
                    if (v == y) v = x;
        }
    }
};

int main() {
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 40 + 1, q = rng() % 12;
        if (it % 50 == 0) n = rng() % 3 + 1;
        DSU_Range_Parallel d(n);
        Naive nv(n);
        vector<long long> val(n), orig(n);
        for (int i = 0; i < n; i++) val[i] = orig[i] = rng() % 1000;
        long long calls = 0;
        bool cb_ok = true;
        for (int t = 0; t < q; t++) {
            int len = rng() % (n + 1);
            if (rng() % 3 == 0) len = rng() % 4; // short ranges, incl. 0 and 1
            len = min(len, n);
            int a = rng() % (n - len + 1), b = rng() % (n - len + 1);
            if (rng() % 8 == 0) b = a;
            if (t % 2) {
                d.unite(a, b, len, [&](int keep, int gone) {
                    calls++;
                    // keep must be the current root, gone a former root (no longer its own root)
                    cb_ok &= d.find(keep) == keep && d.find(gone) == keep && gone != keep;
                    val[keep] += val[gone];
                });
            } else {
                int before = d.components;
                d.unite(a, b, len);
                calls += before - d.components; // keep val in sync: recompute below for no-cb
                for (int i = 0; i < n; i++) val[i] = 0;
                for (int i = 0; i < n; i++) val[d.find(i)] += orig[i];
            }
            nv.unite(a, b, len);
            // compare partitions
            bool ok = true, ok_same = true, ok_size = true;
            map<int, int> cnt;
            for (int i = 0; i < n; i++) cnt[nv.lab[i]]++;
            for (int i = 0; i < n; i++) {
                ok_size &= d.size(i) == cnt[nv.lab[i]];
                for (int j = 0; j < n; j++) {
                    bool s = nv.lab[i] == nv.lab[j];
                    ok &= (d.find(i) == d.find(j)) == s;
                    ok_same &= d.same(i, j) == s;
                }
            }
            chk("unite+find", ok);
            chk("same", ok_same);
            chk("size", ok_size);
            chk("components", d.components == (int)cnt.size());
            chk(t % 2 ? "unite(callback)" : "unite(no callback)", ok);
        }
        // callback: sums per class must match, and total calls = n - components
        bool ok = cb_ok;
        map<int, long long> sum;
        for (int i = 0; i < n; i++) sum[nv.lab[i]] += orig[i];
        for (int i = 0; i < n; i++) ok &= val[d.find(i)] == sum[nv.lab[i]];
        chk("callback", ok && calls == n - d.components);
    }
    {
        int n = 500000, q = 500000;
        auto t0 = clock();
        DSU_Range_Parallel d(n);
        long long calls = 0;
        for (int t = 0; t < q; t++) {
            int len = rng() % (n / 2) + 1;
            int a = rng() % (n - len + 1), b = rng() % (n - len + 1);
            d.unite(a, b, len, [&](int, int) { calls++; });
        }
        printf("n=q=5e5 random unites: %.3fs (components=%d, merges=%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, d.components, calls);
        chk("large:callback count", calls == n - d.components);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
