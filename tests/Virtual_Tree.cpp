// Brute-force test for:
//   Tree/Virtual_Tree.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace VT {
#include "../Tree/Virtual_Tree.cpp"
}
int main() {
    mt19937_64 gen(90210);
    int lcaFail = 0, distFail = 0, steinerFail = 0, sizeFail = 0, ancFail = 0, structFail = 0;
    int reuseFail = 0, cases = 0;

    for (int it = 0; it < 3000; it++) {
        int n = 1 + (int)(gen() % 12);
        VT::Virtual_Tree g(n);
        vector<array<ll, 3>> edges;
        vector<vector<pair<int, ll>>> adj(n + 1);
        for (int v = 2; v <= n; v++) {
            int p = 1 + (int)(gen() % (v - 1));
            ll w = 1 + (ll)(gen() % 9);
            edges.push_back({(ll)p, (ll)v, w});
            g.add_edge(p, v, w);
            adj[p].push_back({v, w}), adj[v].push_back({p, w});
        }
        g.init(1);
        cases++;

        // ---- reference LCA and distance by walking parents explicitly
        vector<int> par(n + 1, 0), depth(n + 1, 0);
        vector<ll> rd(n + 1, 0);
        {
            vector<int> st{1};
            vector<char> seen(n + 1, 0);
            seen[1] = 1;
            while (!st.empty()) {
                int u = st.back(); st.pop_back();
                for (auto [v, w] : adj[u]) if (!seen[v]) seen[v] = 1, par[v] = u, depth[v] = depth[u] + 1, rd[v] = rd[u] + w, st.push_back(v);
            }
        }
        auto refLca = [&](int a, int b) { while (a != b) { if (depth[a] < depth[b]) swap(a, b); a = par[a]; } return a; };
        for (int a = 1; a <= n; a++)
            for (int b = 1; b <= n; b++) {
                if (g.lca(a, b) != refLca(a, b)) lcaFail++;
                if (g.dist(a, b) != rd[a] + rd[b] - 2 * rd[refLca(a, b)]) distFail++;
                bool anc = false;
                for (int x = b; ; x = par[x]) { if (x == a) { anc = true; break; } if (x == 1) break; }
                if (g.is_ancestor(a, b) != anc) ancFail++;
            }

        // ---- the virtual tree itself, over random marked sets
        for (int rep = 0; rep < 6; rep++) {
            int k = 1 + (int)(gen() % n);
            vector<int> marked;
            for (int i = 0; i < k; i++) marked.push_back(1 + (int)(gen() % n));
            vector<int> uniq = marked;
            sort(uniq.begin(), uniq.end());
            uniq.erase(unique(uniq.begin(), uniq.end()), uniq.end());

            vector<int> nodes = g.build(marked);

            // (a) O(k) size bound: at most 2k-1 nodes
            if ((int)nodes.size() > 2 * (int)uniq.size()) sizeFail++;
            // (b) every marked node is present
            for (int x : uniq) if (find(nodes.begin(), nodes.end(), x) == nodes.end()) structFail++;
            // (c) it is a tree: node count == edge count + 1, and the front is an ancestor of all
            int edgeCount = 0;
            for (int u : nodes) edgeCount += (int)g.vt[u].size();
            if (edgeCount + 1 != (int)nodes.size()) structFail++;
            for (int u : nodes) if (!g.is_ancestor(nodes.front(), u)) structFail++;
            // (d) every vt edge must be a real ancestor relation in the base tree
            for (int u : nodes) for (int v : g.vt[u]) if (!g.is_ancestor(u, v) || u == v) structFail++;

            // (e) the payload: total weight of the minimal connected subtree, against a brute force
            //     that marks every edge on every path from a marked node up to the overall LCA
            ll got = 0;
            for (int u : nodes) for (int v : g.vt[u]) got += g.dist(u, v);
            int top = uniq[0];
            for (int x : uniq) top = refLca(top, x);
            set<int> onPath;
            for (int x : uniq) for (int y = x; y != top; y = par[y]) onPath.insert(y);
            ll want = 0;
            for (int y : onPath) want += rd[y] - rd[par[y]];
            if (got != want) steinerFail++;
        }

        // ---- a second build must not inherit the first one's edges
        {
            g.build({1});
            vector<int> nodes = g.build({n});
            int total = 0;
            for (int u = 1; u <= n; u++) total += (int)g.vt[u].size();
            if (total != 0 || nodes.size() != 1 || nodes[0] != n) reuseFail++;
        }
    }
    printf("Virtual_Tree  (%d random trees)\n", cases);
    printf("  lca vs parent walk         : %d failures\n", lcaFail);
    printf("  dist vs reference          : %d failures\n", distFail);
    printf("  is_ancestor                : %d failures\n", ancFail);
    printf("  size <= 2k                 : %d failures\n", sizeFail);
    printf("  really a tree, marks kept  : %d failures\n", structFail);
    printf("  Steiner weight vs brute    : %d failures\n", steinerFail);
    printf("  rebuild clears the old one : %d failures\n", reuseFail);
}
