// Test for 01_Data_Structures/DSU_Rollback.cpp against recomputing components from the live edges.
// Every public method is checked: join (incl. no-op joins), rollback (incl. k beyond the stack),
// find, same, get_size, get_sets, and the stk.size() checkpoint pattern from the Usage line.
#include "../../01_Data_Structures/DSU_Rollback.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(7);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

// component label of every node 1..n, by BFS over the live edges
vector<int> comps(int n, const vector<pair<int, int>> &es) {
    vector<vector<int>> g(n + 1);
    for (auto [u, v] : es) g[u].push_back(v), g[v].push_back(u);
    vector<int> c(n + 1, 0);
    int k = 0;
    for (int s = 1; s <= n; s++) {
        if (c[s]) continue;
        c[s] = ++k;
        vector<int> st{s};
        while (!st.empty()) {
            int u = st.back();
            st.pop_back();
            for (int v : g[u])
                if (!c[v]) c[v] = k, st.push_back(v);
        }
    }
    return c;
}

void compare(DSU &d, int n, const vector<pair<int, int>> &es) {
    auto c = comps(n, es);
    int k = n ? *max_element(c.begin() + 1, c.end()) : 0;
    chk("get_sets", d.get_sets() == k);
    for (int u = 1; u <= n; u++) {
        int sz = 0;
        for (int v = 1; v <= n; v++) sz += c[v] == c[u];
        chk("get_size", d.get_size(u) == sz);
        int r = d.find(u);
        chk("find", r >= 1 && r <= n && d.parent[r] == r && c[r] == c[u]);
        for (int v = 1; v <= n; v++) chk("same", d.same(u, v) == (c[u] == c[v]));
    }
}

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 8 + 1;
        DSU d(n);
        vector<pair<int, int>> es;  // live joins (no-ops included, as a self pair)
        vector<size_t> checkpoints; // stk.size() snapshots
        int ops = rng() % 25;
        for (int o = 0; o < ops; o++) {
            int t = rng() % 10;
            if (t < 6) {
                int u = rng() % n + 1, v = rng() % n + 1;
                d.join(u, v);
                es.push_back({u, v});
                chk("join", d.stk.size() == es.size());
            } else if (t < 8) {
                int k = rng() % 6; // may exceed the stack: must just empty it
                d.rollback(k);
                for (int i = 0; i < k && !es.empty(); i++) es.pop_back();
                chk("rollback", d.stk.size() == es.size());
            } else if (t == 8) {
                checkpoints.push_back(d.stk.size());
            } else if (!checkpoints.empty()) {
                size_t cp = checkpoints.back();
                checkpoints.pop_back();
                if (cp > d.stk.size()) continue; // checkpoint already rolled past
                d.rollback((int)(d.stk.size() - cp));
                es.resize(cp);
                chk("rollback(checkpoint)", d.stk.size() == cp);
            }
            compare(d, n, es);
        }
        d.rollback(1000); // full undo returns to n singletons
        es.clear();
        chk("rollback(all)", d.get_sets() == n && d.stk.empty());
        compare(d, n, es);
    }
    { // n = 1 edge case
        DSU d(1);
        d.join(1, 1);
        chk("n=1", d.get_sets() == 1 && d.get_size(1) == 1 && d.stk.size() == 1);
        d.rollback(1);
        chk("n=1", d.get_sets() == 1 && d.stk.empty());
    }
    { // timing: offline-connectivity sized workload
        int n = 300000;
        DSU d(n);
        auto t0 = clock();
        long long s = 0;
        for (int r = 0; r < 10; r++) {
            for (int i = 0; i < 300000; i++) d.join(rng() % n + 1, rng() % n + 1);
            for (int i = 0; i < 300000; i++) s += d.same(rng() % n + 1, rng() % n + 1);
            d.rollback(300000);
        }
        chk("timing:restored", d.get_sets() == n);
        printf("3e6 joins + 3e6 same + 3e6 rollbacks, n=3e5: %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
