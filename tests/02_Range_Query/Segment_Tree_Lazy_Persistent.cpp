// Test for 02_Range_Query/Segment_Tree_Lazy_Persistent.cpp against a stored copy of every version.
// Every public method is checked: constructor, update (range add on any version), query (any
// version), and going back with roots.resize (SPOJ TTM operations C, Q, H, B).
#include "../../02_Range_Query/Segment_Tree_Lazy_Persistent.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(43);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (int it = 0; it < 15000; it++) {
        int n = rng() % 15 + 1;
        ll big = vector<ll>{5, 1000000000LL, 1000000000000LL}[rng() % 3];
        vector<ll> a(n);
        for (auto &x : a) x = (ll)(rng() % (2 * big + 1)) - big;
        PersistentSegTree<ll> t(n, a);
        vector<vector<ll>> vers = {a};
        for (int o = 0; o < 30; o++) {
            int kind = rng() % 5;
            int l = rng() % n + 1, r = rng() % n + 1;
            if (l > r) swap(l, r);
            if (kind <= 1) { // C on the newest, or on an older version (branching)
                int ver = kind == 0 ? (int)vers.size() - 1 : (int)(rng() % vers.size());
                ll v = (ll)(rng() % (2 * big + 1)) - big;
                t.update(ver, l, r, v);
                auto nv = vers[ver];
                for (int i = l; i <= r; i++) nv[i - 1] += v;
                vers.push_back(nv);
            } else if (kind <= 3) { // Q / H
                int ver = rng() % vers.size();
                ll s = 0;
                for (int i = l; i <= r; i++) s += vers[ver][i - 1];
                chk("query", t.query(ver, l, r) == s);
            } else { // B: back to version k
                int k = rng() % vers.size();
                t.roots.resize(k + 1), vers.resize(k + 1);
            }
        }
        bool ok = t.roots.size() == vers.size();
        for (int v = 0; ok && v < (int)vers.size(); v++)
            for (int i = 1; i <= n; i++) ok &= t.query(v, i, i) == vers[v][i - 1];
        chk("every version, every point", ok);
    }
    { // SPOJ TTM sample: 10 5; 1..10; Q 4 4 / Q 1 10 / Q 2 4 / C 3 6 3 / Q 2 4 -> 4 55 9 15
        vector<ll> a(10);
        iota(a.begin(), a.end(), 1);
        PersistentSegTree<ll> t(10, a);
        int cur = 0;
        bool ok = t.query(cur, 4, 4) == 4 && t.query(cur, 1, 10) == 55 && t.query(cur, 2, 4) == 9;
        t.update(cur, 3, 6, 3), cur++;
        ok &= t.query(cur, 2, 4) == 15;
        chk("samples", ok);
    }
    {
        int n = 100000;
        vector<ll> a(n);
        for (auto &x : a) x = rng() % 1000000000;
        PersistentSegTree<ll> t(n, a);
        auto t0 = clock();
        ll s = 0;
        for (int i = 0; i < 100000; i++) {
            int l = rng() % n + 1, r = rng() % n + 1;
            if (l > r) swap(l, r);
            if (i % 2) t.update((int)t.roots.size() - 1, l, r, rng() % 1000);
            else s ^= t.query(rng() % t.roots.size(), l, r);
        }
        printf("n=q=1e5 TTM mix: %.3fs (checksum %lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
