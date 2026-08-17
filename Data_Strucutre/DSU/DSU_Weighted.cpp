
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define sz(x) (int)x.size()
#define nl '\n'

/*
    [1] Definition
    Weighted DSU stores relations between nodes.
    Use it when you have constraints like:
        value[v] - value[u] = w

    [2] Notes
    - Uses path compression.
    - Uses union by size.
    - weight[x] stores the difference to its parent.
    - query(u, v) returns value[v] - value[u].
*/

struct WeightedDSU {
    vector<int> parent, group;
    vector<int> weight;
    int comp;

    WeightedDSU(int n) {
        parent.assign(n + 1, 0);
        group.assign(n + 1, 0);
        weight.assign(n + 1, 0);

        for (int i = 1; i <= n; i++) {
            parent[i] = i;
            group[i] = 1;
            weight[i] = 0;
        }

        comp = n;
    }

    int merge(int a, int b) {
        return a + b;
    }

    pair<int, int> find(int x) {
        if (parent[x] == x) {
            return {x, 0};
        }

        auto [root, w] = find(parent[x]);

        weight[x] = merge(weight[x], w);
        parent[x] = root;

        return {root, weight[x]};
    }

    bool Union(int u, int v, ll w) {
        auto [lu, wu] = find(u);
        auto [lv, wv] = find(v);

        if (lu == lv) {
            return (wv - wu == w);
        }

        if (group[lu] < group[lv]) {
            swap(lu, lv);
            swap(u, v);
            swap(lu, lv);

            auto [nlu, nwu] = find(u);
            auto [nlv, nwv] = find(v);

            lu = nlu;
            lv = nlv;
            wu = nwu;
            wv = nwv;

            w = -w;
        }

        weight[lv] = wu - wv + w;

        group[lu] += group[lv];
        parent[lv] = lu;

        comp--;

        return true;
    }

    int query(int u, int v) {
        auto [lu, wu] = find(u);
        auto [lv, wv] = find(v);

        if (lv != lu) return INT_MIN;

        return wv - wu;
    }

    bool same(int u, int v) {
        return find(u).first == find(v).first;
    }

    int Size(int u) {
        return group[find(u).first];
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    WeightedDSU ds(n);
    vector<int> graph;

    for (int i = 1; i <= q; i++) {
        int u, v, d;
        cin >> u >> v >> d;

        // If connected, check if the new constraint is valid.
        if (ds.same(u, v)) {
            // Keep the edge only if it matches the known difference.
            if (ds.query(u, v) == d) {
                graph.push_back(i);
            }
        } else {
            // Different components: add the constraint and merge them.
            ds.Union(u, v, d);
            graph.push_back(i);
        }
    }

    // Print all valid constraints.
    for (int i = 0; i < graph.size(); i++) cout << graph[i] << " ";

    return 0;
}