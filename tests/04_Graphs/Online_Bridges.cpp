// Test for 04_Graphs/Online_Bridges.cpp: after EVERY added edge, recompute bridges from scratch
// (remove each edge, check connectivity with a fresh DSU) and compare. Every public method is
// checked: add_edge (multi-edges, self loops, 0- and 1-based), count_bridges, is_bridge (on added
// edges, and on any pair = "different 2-edge-connected components"), find_2ecc, find_cc, plus the
// component-size invariant behind the small-to-large re-rooting.
#include "../../04_Graphs/Online_Bridges.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(37);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

struct D {
    vector<int> p;
    D(int n) : p(n) { iota(p.begin(), p.end(), 0); }
    int f(int x) { return p[x] == x ? x : p[x] = f(p[x]); }
    void u(int a, int b) { p[f(a)] = f(b); }
};

int main() {
    for (int it = 0; it < 6000; it++) {
        int n = rng() % 8 + 1, m = rng() % 16, base = rng() % 2;
        OnlineBridges ob(n + base);
        vector<pair<int, int>> es;
        for (int k = 0; k < m; k++) {
            int a = rng() % n, b = rng() % n;
            es.push_back({a, b});
            ob.add_edge(a + base, b + base);
            int E = es.size();
            // brute: edge i is a bridge iff removing it disconnects its ends
            vector<bool> br(E);
            int cnt = 0;
            for (int i = 0; i < E; i++) {
                D d(n);
                for (int j = 0; j < E; j++)
                    if (j != i) d.u(es[j].first, es[j].second);
                br[i] = d.f(es[i].first) != d.f(es[i].second);
                cnt += br[i];
            }
            chk(base ? "count_bridges (1-based)" : "count_bridges (0-based)", ob.count_bridges() == cnt);
            bool ok = true;
            for (int i = 0; i < E; i++) ok &= ob.is_bridge(es[i].first + base, es[i].second + base) == br[i];
            chk("is_bridge (added edges)", ok);
            // 2ecc = components after deleting bridges; cc = components of all edges
            D two(n), cc(n);
            for (int i = 0; i < E; i++) {
                cc.u(es[i].first, es[i].second);
                if (!br[i]) two.u(es[i].first, es[i].second);
            }
            vector<int> size(n, 0);
            for (int v = 0; v < n; v++) size[cc.f(v)]++;
            bool ok2 = true, ok3 = true, ok4 = true;
            for (int a = 0; a < n; a++) {
                ok4 &= ob.dsu_cc_size[ob.find_cc(a + base)] == size[cc.f(a)];
                for (int b = 0; b < n; b++) {
                    bool same2 = two.f(a) == two.f(b), same = cc.f(a) == cc.f(b);
                    ok2 &= ob.is_bridge(a + base, b + base) == !same2;
                    ok2 &= (ob.find_2ecc(a + base) == ob.find_2ecc(b + base)) == same2;
                    ok3 &= (ob.find_cc(a + base) == ob.find_cc(b + base)) == same;
                }
            }
            chk("is_bridge / find_2ecc (all pairs)", ok2);
            chk("find_cc (all pairs)", ok3);
            chk("cc size invariant", ok4);
        }
    }
    // samples: Library Checker two_edge_connected_components example_00 (K=1), example_02 (K=1),
    // example_01 checked against the from-scratch brute force
    {
        auto comps = [](int n, vector<pair<int, int>> es) {
            OnlineBridges ob(n);
            for (auto [a, b] : es) ob.add_edge(a, b);
            set<int> s;
            for (int v = 0; v < n; v++) s.insert(ob.find_2ecc(v));
            return (int)s.size();
        };
        vector<pair<int, int>> e1 = {{4, 5},  {8, 7},  {12, 3}, {3, 10}, {1, 5},  {10, 2}, {0, 0},
                                     {11, 4}, {2, 12}, {9, 1},  {9, 0},  {7, 8},  {7, 6},  {9, 1},
                                     {8, 2},  {12, 10}, {11, 0}, {8, 6}, {3, 2},  {5, 9},  {4, 11}};
        int E = e1.size(), n = 13;
        D two(n);
        for (int i = 0; i < E; i++) {
            D d(n);
            for (int j = 0; j < E; j++)
                if (j != i) d.u(e1[j].first, e1[j].second);
            if (d.f(e1[i].first) == d.f(e1[i].second)) two.u(e1[i].first, e1[i].second);
        }
        set<int> s;
        for (int v = 0; v < n; v++) s.insert(two.f(v));
        bool ok = comps(4, {{0, 2}, {0, 1}, {3, 0}, {2, 1}, {2, 3}}) == 1;
        ok &= comps(2, {{0, 1}, {1, 0}}) == 1;
        ok &= comps(n, e1) == (int)s.size();
        chk("samples", ok);
    }
    {
        int n = 200000, m = 400000;
        OnlineBridges ob(n);
        auto t0 = clock();
        long long sum = 0;
        for (int i = 0; i + 1 < n; i += 2) ob.add_edge(i, i + 1); // many pairs, then chains of pairs
        for (int len = 2; len < n; len *= 2)
            for (int i = 0; i + len < n; i += 2 * len) ob.add_edge(i + len - 1, i + len);
        for (int i = 0; i < m; i++) ob.add_edge(rng() % n, rng() % n), sum += ob.count_bridges();
        printf("n=2e5, path merges + 4e5 random edges: %.3fs (sum=%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, sum);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-34s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
