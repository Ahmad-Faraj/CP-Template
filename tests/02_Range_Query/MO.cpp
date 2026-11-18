// Test for 02_Range_Query/MO.cpp against computing sum cnt(x)^2 * x of each range with a map.
// Every public method is checked: constructor, add_query, run (answer order), add, remove.
#include "../../02_Range_Query/MO.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(31);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

ll brute(const vector<ll> &a, int l, int r) {
    map<ll, ll> c;
    for (int i = l; i <= r; i++) c[a[i]]++;
    ll s = 0;
    for (auto [x, k] : c) s += k * k * x;
    return s;
}

int main() {
    { // CF 86D samples (1-based input, converted to 0-based)
        MO a({1, 2, 1}, 2);
        a.add_query(0, 1), a.add_query(0, 2);
        MO b({1, 1, 2, 2, 1, 3, 1, 1}, 3);
        b.add_query(1, 6), b.add_query(0, 5), b.add_query(1, 6);
        chk("samples", a.run() == vector<ll>{3, 6} && b.run() == vector<ll>{20, 20, 20});
    }
    { // no queries
        MO m({5}, 5);
        chk("run(q=0)", m.run().empty());
    }
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 15 + 1, mx = vector<int>{0, 1, 3, 10, 1000}[rng() % 5];
        vector<ll> a(n);
        for (auto &x : a) x = rng() % (mx + 1);
        MO m(a, mx);
        int q = rng() % 20;
        vector<pair<int, int>> qs;
        for (int i = 0; i < q; i++) {
            int l = rng() % n, r = rng() % n;
            if (l > r) swap(l, r);
            m.add_query(l, r), qs.push_back({l, r});
        }
        auto ans = m.run();
        bool ok = ans.size() == qs.size();
        for (int i = 0; ok && i < q; i++) ok = ans[i] == brute(a, qs[i].first, qs[i].second);
        chk("run", ok);
        // add / remove directly: after run, cur matches the last window; walk it by hand
        MO w(a, mx);
        bool ok2 = true;
        for (int i = 0; i < n; i++) w.add(i), ok2 &= w.cur == brute(a, 0, i);
        chk("add", ok2);
        ok2 = true;
        for (int i = 0; i < n; i++) w.remove(i), ok2 &= w.cur == (i + 1 < n ? brute(a, i + 1, n - 1) : 0);
        chk("remove", ok2 && w.cur == 0);
    }
    { // timing: n = q = 2e5, values up to 1e6 (CF 86D limits)
        int n = 200000, q = 200000, mx = 1000000;
        vector<ll> a(n);
        for (auto &x : a) x = rng() % mx + 1;
        MO m(a, mx);
        for (int i = 0; i < q; i++) {
            int l = rng() % n, r = rng() % n;
            if (l > r) swap(l, r);
            m.add_query(l, r);
        }
        auto t0 = clock();
        auto ans = m.run();
        ll s = 0;
        for (ll v : ans) s ^= v;
        printf("n=q=2e5 run: %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
