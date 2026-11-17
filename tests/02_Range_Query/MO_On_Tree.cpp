// Test for 02_Range_Query/MO_On_Tree.cpp against walking each path explicitly.
// Every public method is checked: constructor, add_query, read_queries, run, for values on nodes and on edges.
#include "../../02_Range_Query/MO_On_Tree.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(23);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

// distinct values on the path; on edges, the value of an edge is stored at its deeper endpoint
int brute(int u, int v, const vector<int> &par, const vector<int> &dep, const vector<int> &val, bool edge) {
    set<int> s;
    while (u != v) {
        if (dep[u] < dep[v]) swap(u, v);
        s.insert(val[u]), u = par[u];
    }
    if (!edge) s.insert(val[u]);
    return s.size();
}

int main() {
    for (int it = 0; it < 4000; it++) {
        int n = rng() % 25 + 1, q = rng() % 30 + 1, root = rng() % n + 1;
        int vals = rng() % 6 + 1;
        // random tree, random labels, rooted at root
        vector<int> perm(n);
        iota(perm.begin(), perm.end(), 1);
        shuffle(perm.begin(), perm.end(), rng);
        vector<vector<pair<int, int>>> G(n + 1);
        vector<array<int, 3>> edges;
        for (int i = 1; i < n; i++) {
            int a = perm[i], b = perm[rng() % i], w = rng() % vals;
            G[a].push_back({b, w}), G[b].push_back({a, w}), edges.push_back({a, b, w});
        }
        vector<int> par(n + 1, 0), dep(n + 1, 0), nodeval(n + 1), edgeval(n + 1, 0);
        for (int i = 1; i <= n; i++) nodeval[i] = rng() % vals;
        {
            vector<int> st{root}, seen(n + 1, 0);
            seen[root] = 1;
            while (!st.empty()) {
                int u = st.back();
                st.pop_back();
                for (auto [v, w] : G[u])
                    if (!seen[v]) seen[v] = 1, par[v] = u, dep[v] = dep[u] + 1, edgeval[v] = w, st.push_back(v);
            }
        }
        vector<pair<int, int>> qs(q);
        for (auto &[u, v] : qs) u = rng() % n + 1, v = rng() % n + 1;
        for (int edge = 0; edge < 2; edge++) {
            string t = edge ? "edges:" : "nodes:";
            bool via_cin = rng() % 2;
            vector<int> ans;
            if (edge) {
                MoTree<int, true> mo(n, q, G, vector<int>(), root);
                if (via_cin) {
                    stringstream ss;
                    for (auto [u, v] : qs) ss << u << ' ' << v << '\n';
                    auto *old = cin.rdbuf(ss.rdbuf());
                    mo.read_queries();
                    cin.rdbuf(old);
                } else
                    for (auto [u, v] : qs) mo.add_query(u, v);
                ans = mo.run();
            } else {
                MoTree<int> mo(n, q, G, nodeval, root);
                if (via_cin) {
                    stringstream ss;
                    for (auto [u, v] : qs) ss << u << ' ' << v << '\n';
                    auto *old = cin.rdbuf(ss.rdbuf());
                    mo.read_queries();
                    cin.rdbuf(old);
                } else
                    for (auto [u, v] : qs) mo.add_query(u, v);
                ans = mo.run();
            }
            bool ok = (int)ans.size() == q;
            for (int i = 0; ok && i < q; i++)
                ok &= ans[i] == brute(qs[i].first, qs[i].second, par, dep, edge ? edgeval : nodeval, edge);
            chk(t + (via_cin ? "read_queries+run" : "add_query+run"), ok);
        }
    }
    {
        vector<vector<pair<int, int>>> G(2);
        MoTree<int> mo(1, 0, G, vector<int>{0, 0});
        chk("run(no queries)", mo.run().empty());
    }
    {
        int n = 100000, q = 100000;
        vector<vector<pair<int, int>>> G(n + 1);
        for (int i = 2; i <= n; i++) {
            int p = rng() % (i - 1) + 1;
            G[i].push_back({p, 0}), G[p].push_back({i, 0});
        }
        vector<int> V(n + 1);
        for (int i = 1; i <= n; i++) V[i] = rng() % 50000;
        MoTree<int> mo(n, q, G, V);
        for (int i = 0; i < q; i++) mo.add_query(rng() % n + 1, rng() % n + 1);
        auto t0 = clock();
        auto a = mo.run();
        printf("n=q=1e5: %.3fs (a[0]=%d)\n", double(clock() - t0) / CLOCKS_PER_SEC, a[0]);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
