#include "../../../core.h"
/*
 * Topic: Min-Cost Max-Flow (MCMF) via SPFA
 * Description: Finds min-cost max-flow using SPFA to handle negative costs.
 *              Fast but vulnerable to worst-case graphs.
 *
 * Important Facts:
 * - 0-based indexing for nodes by default.
 * - Time Complexity: O(F * V * E)
 * - Space Complexity: O(V + E)
 */
using T = long long;

struct MCMF_SPFA {
    struct edge {
        int u, v;
        T cap, cost;
        int id;

        // Time Complexity: O(1)
        // Space Complexity: O(1)
        // Initializes an edge.
        edge(int _u, int _v, T _cap, T _cost, int _id) : u(_u), v(_v), cap(_cap), cost(_cost), id(_id) {}
    };

    int n, s, t, mxid;
    T flow, cost;
    vector<vector<int>> g;
    vector<edge> e;
    vector<T> d, flow_through;
    vector<int> par;

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Default constructor.
    MCMF_SPFA() {}

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initializes the MCMF network with _n nodes.
    MCMF_SPFA(int _n) {
        n = _n + 10;
        g.assign(n, vector<int>());
        mxid = 0;
    }

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Adds a directed or undirected edge with given capacity and cost.
    void add_edge(int u, int v, T cap, T _cost, int id = -1, bool directed = true) {
        g[u].push_back((int)e.size());
        e.push_back(edge(u, v, cap, _cost, id));
        g[v].push_back((int)e.size());
        e.push_back(edge(v, u, 0, -_cost, -1));
        mxid = max(mxid, id);
        if (!directed) add_edge(v, u, cap, _cost, -1, true);
    }

    // Time Complexity: O(V * E) worst case
    // Space Complexity: O(V)
    // Finds shortest augmenting path using SPFA.
    bool spfa() {
        par.assign(n, -1);
        d.assign(n, inf);
        vector<bool> in_q(n, false);
        queue<int> q;

        d[s] = 0;
        q.push(s);
        in_q[s] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in_q[u] = false;

            for (int i = 0; i < (int)g[u].size(); i++) {
                int id = g[u][i];
                int v = e[id].v;
                T cap = e[id].cap;
                T w = e[id].cost;

                if (cap > 0 && d[u] + w < d[v]) {
                    d[v] = d[u] + w;
                    par[v] = id;
                    if (!in_q[v]) {
                        q.push(v);
                        in_q[v] = true;
                    }
                }
            }
        }
        return d[t] != inf;
    }

    // Time Complexity: O(V)
    // Space Complexity: O(V) for recursion stack
    // Pushes flow along the shortest path found by SPFA.
    T send_flow(int v, T cur) {
        if (par[v] == -1) return cur;
        int id = par[v];
        int u = e[id].u;
        T w = e[id].cost;
        T f = send_flow(u, min(cur, e[id].cap));
        cost += f * w;
        e[id].cap -= f;
        e[id ^ 1].cap += f;
        return f;
    }

    // returns {maxflow, mincost}
    // Time Complexity: O(F * V * E) worst case, F is max flow
    // Space Complexity: O(V)
    // Solves the Min-Cost Max-Flow problem from source _s to sink _t.
    pair<T, T> solve(int _s, int _t, T goal = inf) {
        s = _s;
        t = _t;
        flow = 0, cost = 0;

        while (flow < goal && spfa()) {
            flow += send_flow(t, goal - flow);
        }

        flow_through.assign(mxid + 10, 0);
        for (int u = 0; u < n; u++) {
            for (auto v : g[u]) {
                if (e[v].id >= 0) flow_through[e[v].id] = e[v ^ 1].cap;
            }
        }
        return {flow, cost};
    }

    // Time Complexity: O(E)
    // Space Complexity: O(V)
    // Extracts matching pairs for bipartite matching where edges have id >= 0.
    vector<pair<int, int>> get_bipartite_matching() {
        vector<pair<int, int>> matches;
        for (int i = 0; i < (int)e.size(); i += 2) {
            if (e[i].id >= 0 && e[i ^ 1].cap > 0) {
                matches.push_back({e[i].u, e[i].v});
            }
        }
        return matches;
    }
};

// Takes n workers and n tasks costs (input) and gives minimum cost to assign all tasks (output)
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    if (!(cin >> n)) return 0;

    // Create MCMF with 2*n + 2 nodes
    // Source = 0, Sink = 2*n + 1
    // Workers = 1 to n
    // Tasks = n + 1 to 2*n
    MCMF_SPFA F(2 * n + 2);
    int source = 0, sink = 2 * n + 1;
    for (int i = 1; i <= n; i++) {
        F.add_edge(source, i, 1, 0);
        F.add_edge(i + n, sink, 1, 0);
        for (int j = 1; j <= n; j++) {
            T cost;
            cin >> cost;
            F.add_edge(i, j + n, 1, cost, (i - 1) * n + (j - 1));
        }
    }
    auto ans = F.solve(source, sink);
    cout << ans.second << "\n";
    auto pairs = F.get_bipartite_matching();
    for (auto p : pairs) {
        cout << p.first << " " << p.second - n << "\n";
    }

    return 0;
}