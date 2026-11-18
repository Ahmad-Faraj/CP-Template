// Test for 02_Range_Query/Segment_Tree_Persistent.cpp against a stored copy of every version.
// Every public method is checked: constructor, build, update, update_version, query, get, copy,
// on a 1..n array and on the sparse default range [-1e9, 1e9]; k-th smallest via counts.
#include "../../02_Range_Query/Segment_Tree_Persistent.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(41);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (int it = 0; it < 6000; it++) { // dense: build over 1..n
        int n = rng() % 12 + 1;
        vector<ll> a(n);
        for (auto &x : a) x = (ll)(rng() % 2000000001) - 1000000000;
        PST<ll> t(1, n);
        t.build(a);
        vector<vector<ll>> vers = {vector<ll>(n, 0), a}; // naive copy per version
        for (int o = 0; o < 30; o++) {
            int kind = rng() % 4;
            if (kind == 0) {
                int i = rng() % n + 1;
                ll v = (ll)(rng() % 2000000001) - 1000000000;
                t.update(i, v);
                auto nv = vers.back();
                nv[i - 1] = v;
                vers.push_back(nv);
            } else if (kind == 1) {
                int ver = rng() % vers.size();
                t.copy(ver);
                vers.push_back(vers[ver]);
            } else if (kind == 2) {
                int ver = rng() % vers.size(), i = rng() % n + 1;
                ll v = rng() % 100;
                t.update_version(i, v, ver);
                vers[ver][i - 1] = v;
            } else {
                int ver = rng() % vers.size(), l = rng() % n + 1, r = rng() % n + 1;
                if (l > r) swap(l, r);
                ll s = 0;
                for (int i = l; i <= r; i++) s += vers[ver][i - 1];
                chk("dense:query", t.query(l, r, ver) == s);
            }
        }
        bool ok = (int)t.roots.size() == (int)vers.size();
        for (int v = 0; ok && v < (int)vers.size(); v++)
            for (int i = 1; i <= n; i++) ok &= t.get(v, i) == vers[v][i - 1];
        chk("dense:get(every version)", ok);
    }
    for (int it = 0; it < 4000; it++) { // sparse: default range, negative coordinates, T = int
        PST<int> t; // [-1e9, 1e9]
        vector<map<ll, int>> vers = {{}};
        vector<ll> keys;
        for (int k = 0; k < 6; k++) keys.push_back((ll)(rng() % 2000000001) - 1000000000);
        keys.push_back(-1000000000), keys.push_back(1000000000);
        for (int o = 0; o < 25; o++) {
            if (rng() % 2) {
                ll i = keys[rng() % keys.size()];
                int v = rng() % 1000;
                t.update(i, v);
                auto nv = vers.back();
                nv[i] = v;
                vers.push_back(nv);
            } else {
                int ver = rng() % vers.size();
                ll l = keys[rng() % keys.size()], r = keys[rng() % keys.size()];
                if (l > r) swap(l, r);
                long long s = 0;
                for (auto [k, v] : vers[ver])
                    if (l <= k && k <= r) s += v;
                chk("sparse:query", t.query(l, r, ver) == s);
            }
        }
    }
    for (int it = 0; it < 3000; it++) { // classic use: k-th smallest in a[l..r] via count versions
        int n = rng() % 15 + 1, V = rng() % 10 + 1;
        vector<int> a(n);
        for (auto &x : a) x = rng() % V + 1;
        PST<int> t(1, V); // version i = counts of a[0..i-1]
        vector<int> cnt(V + 1, 0);
        for (int i = 0; i < n; i++) t.update(a[i], ++cnt[a[i]]);
        int l = rng() % n + 1, r = rng() % n + 1;
        if (l > r) swap(l, r);
        vector<int> sub(a.begin() + l - 1, a.begin() + r);
        sort(sub.begin(), sub.end());
        int k = rng() % sub.size() + 1, lo = 1, hi = V; // smallest x with count(<= x) >= k
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (t.query(1, mid, r) - t.query(1, mid, l - 1) >= k) hi = mid;
            else lo = mid + 1;
        }
        chk("kth_smallest(usage)", lo == sub[k - 1]);
    }
    {
        int n = 200000;
        PST<ll> t(1, n);
        vector<ll> a(n);
        for (auto &x : a) x = rng() % 1000000000;
        t.build(a);
        auto t0 = clock();
        ll s = 0;
        for (int i = 0; i < 200000; i++) {
            t.update(rng() % n + 1, rng() % 1000000000);
            int l = rng() % n + 1, r = rng() % n + 1;
            if (l > r) swap(l, r);
            s ^= t.query(l, r, rng() % t.roots.size());
        }
        printf("n=q=2e5 update+query: %.3fs (checksum %lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
