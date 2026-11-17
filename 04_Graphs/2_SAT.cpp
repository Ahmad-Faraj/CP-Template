// 2-SAT: decide if 2-literal clauses over boolean variables can all hold, and give an assignment.
// Use when: two states per item, pairwise rules: "at least one of", "not both", "if a then b".
// Usage: Two_SAT s(vars); s.addOR(a, 1, b, 0); if (s.ok()) use s.res[a];
// Handles: OR, XOR, implication (add), forcing x_a = f via addOR(a, f, a, f); ok() can be re-run.
// Time: O(vars + clauses)
// Indexing: 0-based variables; af / bf = the value the literal asks for (1 = x, 0 = !x)
// Note: dfs1 / dfs2 are recursive over 2 * vars nodes, so long implication chains need a big stack.

#include <bits/stdc++.h>
using namespace std;

struct Two_SAT {
    int n; // literal nodes: 2v = (x_v true), 2v + 1 = (x_v false)
    vector<vector<int>> g, gt;
    vector<bool> vis, res; // res[v] = value of x_v, valid after ok() returns true
    vector<int> comp;
    stack<int> ts;

    Two_SAT(int vars = 0) : n(vars << 1), g(n), gt(n) {}

    void addOR(int a, bool af, int b, bool bf) { // (x_a == af) OR (x_b == bf)
        a += a + (af ^ 1);
        b += b + (bf ^ 1);
        g[a ^ 1].push_back(b); // !a => b
        g[b ^ 1].push_back(a); // !b => a
        gt[b].push_back(a ^ 1);
        gt[a].push_back(b ^ 1);
    }
    void addXOR(int a, bool af, int b, bool bf) { // exactly one of (x_a == af), (x_b == bf)
        addOR(a, af, b, bf);
        addOR(a, !af, b, !bf);
    }
    void _add(int a, bool af, int b, bool bf) {
        a += a + (af ^ 1);
        b += b + (bf ^ 1);
        g[a].push_back(b);
        gt[b].push_back(a);
    }
    void add(int a, bool af, int b, bool bf) { // if x_a == af then x_b must be bf
        _add(a, af, b, bf);
        _add(b, !bf, a, !af);
    }

    void dfs1(int u) {
        vis[u] = true;
        for (int v : g[u])
            if (!vis[v]) dfs1(v);
        ts.push(u);
    }
    void dfs2(int u, int c) {
        comp[u] = c;
        for (int v : gt[u])
            if (comp[v] == -1) dfs2(v, c);
    }
    bool ok() { // Kosaraju; comp ids come out in topological order
        vis.assign(n, false);
        comp.assign(n, -1);
        for (int i = 0; i < n; ++i)
            if (!vis[i]) dfs1(i);
        int scc = 0;
        while (!ts.empty()) {
            int u = ts.top();
            ts.pop();
            if (comp[u] == -1) dfs2(u, scc++);
        }
        res.assign(n / 2, false);
        for (int i = 0; i < n; i += 2) {
            if (comp[i] == comp[i + 1]) return false;
            res[i / 2] = (comp[i] > comp[i + 1]);
        }
        return true;
    }
};
