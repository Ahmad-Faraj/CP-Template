// Brute-force test for:
//   Tree/LCA.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace L {
#include "../Tree/LCA.cpp"
}
int main() {
    mt19937_64 gen(24680);
    int lcaFail=0, depFail=0, kthFail=0, distFail=0, checked=0;
    for (int it = 0; it < 8000; it++) {
        int n = 1 + (int)(gen() % 30);
        vector<int> par(n + 1, 0);
        L::LCA t(n);
        for (int i = 2; i <= n; i++) {
            par[i] = (it % 3 == 0) ? i - 1 : 1 + (int)(gen() % (i - 1)); // sometimes a pure path
            t.add_edge(par[i], i);
        }
        t.build(1);
        checked++;
        // brute force: walk up the parent chain
        vector<int> depth(n + 1, 0);
        for (int i = 2; i <= n; i++) depth[i] = depth[par[i]] + 1;
        auto brute = [&](int a, int b) {
            while (depth[a] > depth[b]) a = par[a];
            while (depth[b] > depth[a]) b = par[b];
            while (a != b) a = par[a], b = par[b];
            return a;
        };
        for (int a = 1; a <= n; a++) {
            if (t.dep[a] != depth[a]) depFail++;
            for (int b = 1; b <= n; b++) {
                int want = brute(a, b);
                if (t.get_lca(a, b) != want) lcaFail++;
                if (t.dist(a, b) != depth[a] + depth[b] - 2*depth[want]) distFail++;
            }
            for (int k = 0; k <= depth[a] + 1; k++) {
                int w = a; bool ok = true;
                for (int i = 0; i < k; i++) { if (w == 1 && depth[w] == 0 && i < k) { } w = par[w]; }
                int got = t.kth_ancestor(a, k);
                if (k > depth[a]) { if (got != -1) kthFail++; }
                else { int x = a; for (int i = 0; i < k; i++) x = par[x]; if (got != x) kthFail++; }
                (void)w; (void)ok;
            }
        }
    }
    printf("LCA after making the traversal iterative: %d trees (a third of them paths)\n", checked);
    printf("  get_lca vs walking parents : %d failures\n", lcaFail);
    printf("  depth                      : %d failures\n", depFail);
    printf("  kth_ancestor               : %d failures\n", kthFail);
    printf("  dist                       : %d failures\n", distFail);
}
