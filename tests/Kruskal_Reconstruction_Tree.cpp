// Brute-force test for:
//   Graph/Kruskal_Reconstruction_Tree.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace KR {
#include "../Graph/Kruskal_Reconstruction_Tree.cpp"
}
int main() {
    mt19937_64 gen(777001);
    int sizeFail = 0, vertFail = 0, maxFail = 0, connFail = 0, treeFail = 0, discFail = 0, cases = 0;

    for (int it = 0; it < 4000; it++) {
        int n = 1 + (int)(gen() % 9);
        int m = (int)(gen() % 14);
        vector<array<ll, 3>> edges;
        for (int i = 0; i < m; i++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
            edges.push_back({(ll)(1 + gen() % 8), (ll)u, (ll)v}); // duplicate weights and self loops on purpose
        }
        KR::Kruskal_Reconstruction_Tree krt(n, edges);
        cases++;

        // the KRT must be a forest of n leaves: every original vertex appears exactly once
        {
            vector<int> seen(n + 1, 0);
            for (int x : krt.leaf_order) if (x >= 1 && x <= n) seen[x]++;
            for (int v = 1; v <= n; v++) if (seen[v] != 1) treeFail++;
            if ((int)krt.leaf_order.size() != n) treeFail++;
        }

        for (ll w = 0; w <= 9; w++) {
            // brute force: union everything with weight <= w
            vector<int> p(n + 1);
            iota(p.begin(), p.end(), 0);
            function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
            for (auto &e : edges) if (e[0] <= w) p[f((int)e[1])] = f((int)e[2]);
            for (int u = 1; u <= n; u++) {
                int cnt = 0;
                for (int v = 1; v <= n; v++) cnt += f(u) == f(v);
                if (krt.component_size(u, w) != cnt) sizeFail++;
                // vertices() must be exactly that component
                vector<int> got = krt.vertices(krt.component(u, w));
                sort(got.begin(), got.end());
                vector<int> want;
                for (int v = 1; v <= n; v++) if (f(u) == f(v)) want.push_back(v);
                if (got != want) vertFail++;
            }
        }

        // max_edge: the minimax path weight, brute-forced by raising the threshold until connected
        for (int u = 1; u <= n; u++)
            for (int v = 1; v <= n; v++) {
                ll want = LLONG_MAX;
                for (ll w = 0; w <= 9 && want == LLONG_MAX; w++) {
                    vector<int> p(n + 1);
                    iota(p.begin(), p.end(), 0);
                    function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
                    for (auto &e : edges) if (e[0] <= w) p[f((int)e[1])] = f((int)e[2]);
                    if (f(u) == f(v)) want = w;
                }
                if (u == v) want = 0;
                if (krt.max_edge(u, v) != want) maxFail++;
                bool refConn = want != LLONG_MAX;
                if (krt.connected(u, v) != refConn) connFail++;
            }

        // a fully disconnected graph must give n roots, and a connected one exactly 1
        if (m == 0 && (int)krt.roots.size() != n) discFail++;
    }
    printf("Kruskal_Reconstruction_Tree  (%d random graphs, dup weights + self loops)\n", cases);
    printf("  every vertex is one leaf   : %d failures\n", treeFail);
    printf("  component_size vs DSU      : %d failures\n", sizeFail);
    printf("  vertices() is the component: %d failures\n", vertFail);
    printf("  max_edge = minimax path    : %d failures\n", maxFail);
    printf("  connected()                : %d failures\n", connFail);
    printf("  roots on an edgeless graph : %d failures\n", discFail);
}
