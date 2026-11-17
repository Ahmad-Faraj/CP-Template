// Test for 03_Trees/Euler_Tour.cpp against naive parent walks (forests included).
// Every public method is checked: add_edge, build(root) (and rebuild), start / finish / order,
// is_ancestor, flatten, and a subtree-sum use case against walking the subtree.
#include "../../03_Trees/Euler_Tour.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(5);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 12 + 1;
        Euler_Tour E(n);
        vector<vector<int>> g(n + 1);
        bool forest = rng() % 3 == 0;
        for (int v = 2; v <= n; v++) {
            if (forest && rng() % 3 == 0) continue;
            int u = rng() % (v - 1) + 1;
            if (rng() % 2) E.add_edge(u, v);
            else E.add_edge(v, u);
            g[u].push_back(v), g[v].push_back(u);
        }
        int root = rng() % n + 1;
        // oracle parents: BFS from root, then from every unvisited node in increasing order
        vector<int> par(n + 1, 0), seen(n + 1, 0);
        auto grow = [&](int r) {
            vector<int> q = {r};
            seen[r] = 1;
            for (int i = 0; i < (int)q.size(); i++)
                for (int v : g[q[i]])
                    if (!seen[v]) seen[v] = 1, par[v] = q[i], q.push_back(v);
        };
        grow(root);
        for (int v = 1; v <= n; v++)
            if (!seen[v]) grow(v);
        auto anc = [&](int u, int v) { // u is an ancestor of v (or v itself)
            for (; v; v = par[v])
                if (v == u) return true;
            return false;
        };
        for (int rep = 0; rep < 2; rep++) { // build twice: a rebuild must reset
            E.build(root);
            bool ok = E.start[root] == 1;
            vector<int> pos(n + 2, 0);
            for (int u = 1; u <= n; u++)
                ok &= E.start[u] >= 1 && E.start[u] <= n && !pos[E.start[u]]++;
            for (int u = 1; u <= n; u++) ok &= E.order[E.start[u]] == u;
            chk("build start/order", ok);
            ok = true;
            for (int u = 1; u <= n; u++) {
                int cnt = 0;
                for (int v = 1; v <= n; v++) {
                    bool in = E.start[u] <= E.start[v] && E.start[v] <= E.finish[u];
                    ok &= in == anc(u, v);
                    cnt += anc(u, v);
                }
                ok &= E.finish[u] - E.start[u] + 1 == cnt;
            }
            chk("subtree range", ok);
        }
        bool ok = true;
        for (int u = 1; u <= n; u++)
            for (int v = 1; v <= n; v++) ok &= E.is_ancestor(u, v) == anc(u, v);
        chk("is_ancestor", ok);
        vector<long long> val(n + 1);
        for (int u = 1; u <= n; u++) val[u] = (long long)(rng() % 2000001) - 1000000;
        auto flat = E.flatten(val);
        ok = (int)flat.size() == n + 1;
        for (int u = 1; u <= n; u++) ok &= flat[E.start[u]] == val[u];
        vector<long long> pre(n + 1, 0);
        for (int i = 1; i <= n; i++) pre[i] = pre[i - 1] + flat[i];
        for (int u = 1; u <= n; u++) {
            long long want = 0;
            for (int v = 1; v <= n; v++)
                if (anc(u, v)) want += val[v];
            ok &= pre[E.finish[u]] - pre[E.start[u] - 1] == want;
        }
        chk("flatten + subtree sum", ok);
    }
    for (int shape = 0; shape < 2; shape++) { // timing: random tree and a path (deep recursion)
        int n = 200000;
        Euler_Tour E(n);
        for (int v = 2; v <= n; v++) E.add_edge(shape ? v - 1 : (int)(rng() % (v - 1)) + 1, v);
        auto t0 = clock();
        E.build(1);
        double sec = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("n=2e5 %s: %.3fs (finish[1]=%d)\n", shape ? "path" : "random", sec, E.finish[1]);
        chk("build start/order", E.finish[1] == n);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-22s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
