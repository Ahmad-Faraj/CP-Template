// ============================================================================
// VERIFICATION SUBMISSION — CP-Template/Tree/Rerooting.cpp
//
// Target: CSES 1133, "Tree Distances II"   https://cses.fi/problemset/task/1133
//
// Everything between the markers below is Tree/Rerooting.cpp copied byte for byte.
// No logic was changed. Only the main() at the bottom was added, because the
// template ships solve() and no main() by design.
//
// This template is marked UNPROVEN: rerooting has no source in other/ or in any of
// the four reference libraries, so it was written from the standard formulation
// (tier 5). An ACCEPTED verdict here is what removes the warning.
// ============================================================================

// ---------------- BEGIN VERBATIM COPY OF Tree/Rerooting.cpp ----------
// Rerooting: computes a tree DP for EVERY node as the root, in one pass down and one back up.
// Use when: "for each node, the answer if the tree were rooted at it" - distance sums, farthest node, subtree counts.
// Handles: any tree, n = 1, weighted edges, and any DP expressed as identity / merge / add_edge / add_node.
// Time: O(n) merges, so O(n) for the usual DP
// Indexing: 1-based nodes; answer[v] is the value with v as the root, down[v] is v's subtree rooted at run()'s root
// Note: merge must be associative AND commutative - the up pass combines siblings in both directions.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// T is the DP value. Supply the four pieces before calling run():
//   identity()                    the value of an empty set of children
//   merge(a, b)                   combine two child contributions; associative and commutative
//   add_edge(value, child, par, w)  lift a finished child value across the edge to its parent
//   add_node(value, v)            fold in what the node itself contributes
template <typename T> struct Rerooting {
    int n;
    vector<vector<pair<int, ll>>> adj; // (neighbour, edge weight)
    vector<T> down, answer;
    function<T()> identity;
    function<T(T, T)> merge;
    function<T(T, int, int, ll)> add_edge;
    function<T(T, int)> add_node;

    Rerooting(int n) : n(n), adj(n + 1), down(n + 1), answer(n + 1) {}

    void add(int u, int v, ll w = 1) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void run(int root = 1) {
        vector<int> order, parent(n + 1, 0), stack_{root};
        vector<char> seen(n + 1, 0);
        order.reserve(n);
        seen[root] = 1;
        while (!stack_.empty()) { // iterative on purpose: a path of 2e5 nodes would recurse just as deep
            int u = stack_.back();
            stack_.pop_back();
            order.push_back(u);
            for (auto [v, w] : adj[u]) {
                (void)w;
                if (!seen[v]) seen[v] = 1, parent[v] = u, stack_.push_back(v);
            }
        }

        for (int i = (int)order.size() - 1; i >= 0; i--) { // down pass: children before parents
            int u = order[i];
            T acc = identity();
            for (auto [v, w] : adj[u])
                if (v != parent[u]) acc = merge(acc, add_edge(down[v], v, u, w));
            down[u] = add_node(acc, u);
        }

        vector<T> from_parent(n + 1);
        from_parent[root] = identity();
        vector<pair<int, ll>> kids;
        vector<T> prefix, suffix;
        for (int u : order) { // up pass: parents before children
            kids.clear(); // (child, weight) together, so the loops below never re-scan the adjacency
            for (auto [v, w] : adj[u])
                if (v != parent[u]) kids.push_back({v, w});
            int k = (int)kids.size();
            prefix.assign(k + 1, T()), suffix.assign(k + 1, T()); // so each child sees every sibling but not itself
            prefix[0] = suffix[k] = identity();
            for (int j = 0; j < k; j++)
                prefix[j + 1] = merge(prefix[j], add_edge(down[kids[j].first], kids[j].first, u, kids[j].second));
            for (int j = k - 1; j >= 0; j--)
                suffix[j] = merge(add_edge(down[kids[j].first], kids[j].first, u, kids[j].second), suffix[j + 1]);
            answer[u] = add_node(merge(from_parent[u], prefix[k]), u);
            for (int j = 0; j < k; j++) {
                T without = add_node(merge(merge(from_parent[u], prefix[j]), suffix[j + 1]), u);
                from_parent[kids[j].first] = add_edge(without, u, kids[j].first, kids[j].second);
            }
        }
    }
};

// Standard problem: CSES 1133 - for every node, the sum of distances to all other nodes
void solve() {
    int n;
    cin >> n;
    Rerooting<pair<ll, ll>> r(n); // the value is (nodes seen, total distance to them)
    r.identity = [] { return make_pair(0LL, 0LL); };
    r.merge = [](pair<ll, ll> a, pair<ll, ll> b) { return make_pair(a.first + b.first, a.second + b.second); };
    r.add_edge = [](pair<ll, ll> v, int, int, ll w) { return make_pair(v.first, v.second + v.first * w); };
    r.add_node = [](pair<ll, ll> v, int) { return make_pair(v.first + 1, v.second); };
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        r.add(u, v);
    }
    r.run(1);
    for (int v = 1; v <= n; v++) cout << r.answer[v].second << " \n"[v == n];
}
// ---------------- END VERBATIM COPY ----------------------------------------

// Added for submission only. Nothing above this line was modified.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
