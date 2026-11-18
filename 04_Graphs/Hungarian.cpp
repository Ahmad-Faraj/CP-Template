// Hungarian: min-cost maximum matching in a bipartite graph with edge costs (assignment problem).
// Use when: "assign workers to jobs at least total cost", n <= ~500, a dense cost matrix.
// Usage: Hungarian M(n1, n2); M.add_edge(u, v, cost); ll c = M.maximum_matching(); M.l[u];
// Handles: rectangular (pads to max(n1, n2)), missing edges, negative costs, multi-edges (min
//          kept).
// Time: O(n^3) with n = max(n1, n2)
// Indexing: 1-based both sides: left 1..n1, right 1..n2. l[u] = right partner (0 = none), r[v].
// Note: matches as many pairs as possible, then minimises cost (needs |cost| * n << 1e18).
// Max cost: add -cost, negate the answer. Max weight with ANY number of pairs: add_edge(u, v, 0)
// for every pair too, add -w for real edges, answer = -result (pairs with l[u] at a 0 edge are
// unmatched). Call maximum_matching() once.

#include <bits/stdc++.h>
using namespace std;

struct Hungarian {
    vector<vector<long long>> c;
    vector<long long> fx, fy, d;
    vector<int> l, r, arg, trace;
    queue<int> q;
    int start, finish, n;
    const long long inf = 1e18;
    Hungarian() {}
    Hungarian(int n1, int n2) : n(max(n1, n2)) {
        c.assign(n + 1, vector<long long>(n + 1, inf));
        fx.assign(n + 1, 0), fy.assign(n + 1, 0), d.assign(n + 1, 0);
        l.assign(n + 1, 0), r.assign(n + 1, 0), arg.assign(n + 1, 0), trace.assign(n + 1, 0);
    }
    void add_edge(int u, int v, long long cost) { c[u][v] = min(c[u][v], cost); }
    inline long long getC(int u, int v) { return c[u][v] - fx[u] - fy[v]; }
    void initBFS() {
        while (!q.empty()) q.pop();
        q.push(start);
        for (int i = 0; i <= n; ++i) trace[i] = 0;
        for (int v = 1; v <= n; ++v) {
            d[v] = getC(start, v);
            arg[v] = start;
        }
        finish = 0;
    }
    void findAugPath() {
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v = 1; v <= n; ++v)
                if (!trace[v]) {
                    long long w = getC(u, v);
                    if (!w) {
                        trace[v] = u;
                        if (!r[v]) {
                            finish = v;
                            return;
                        }
                        q.push(r[v]);
                    }
                    if (d[v] > w) {
                        d[v] = w;
                        arg[v] = u;
                    }
                }
        }
    }
    void subX_addY() {
        long long delta = inf;
        for (int v = 1; v <= n; ++v)
            if (trace[v] == 0 && d[v] < delta) delta = d[v];
        // Rotate
        fx[start] += delta;
        for (int v = 1; v <= n; ++v)
            if (trace[v]) {
                int u = r[v];
                fy[v] -= delta;
                fx[u] += delta;
            } else
                d[v] -= delta;
        for (int v = 1; v <= n; ++v)
            if (!trace[v] && !d[v]) {
                trace[v] = arg[v];
                if (!r[v]) {
                    finish = v;
                    return;
                }
                q.push(r[v]);
            }
    }
    void Enlarge() {
        do {
            int u = trace[finish];
            int nxt = l[u];
            l[u] = finish;
            r[finish] = u;
            finish = nxt;
        } while (finish);
    }
    // returns the min total cost over the maximum number of real (added) edges
    long long maximum_matching() {
        for (int u = 1; u <= n; ++u) {
            fx[u] = c[u][1];
            for (int v = 1; v <= n; ++v) fx[u] = min(fx[u], c[u][v]);
        }
        for (int v = 1; v <= n; ++v) {
            fy[v] = c[1][v] - fx[1];
            for (int u = 1; u <= n; ++u) fy[v] = min(fy[v], c[u][v] - fx[u]);
        }
        for (int u = 1; u <= n; ++u) {
            start = u;
            initBFS();
            while (!finish) {
                findAugPath();
                if (!finish) subX_addY();
            }
            Enlarge();
        }
        long long ans = 0;
        for (int i = 1; i <= n; ++i) {
            if (c[i][l[i]] != inf)
                ans += c[i][l[i]];
            else
                r[l[i]] = 0, l[i] = 0;
        }
        return ans;
    }
};
