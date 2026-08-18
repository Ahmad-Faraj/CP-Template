// Brute-force test for:
//   Data_Structure/DSU_Weighted.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace DW {
#include "../Data_Structure/DSU_Weighted.cpp"
}
int main() {
    mt19937_64 gen(4242);
    int uniteFail = 0, diffFail = 0, sizeFail = 0, compFail = 0, parityFail = 0, depthFail = 0;
    int cases = 0, contradictions = 0, accepted = 0;

    for (int it = 0; it < 6000; it++) {
        int n = 2 + (int)(gen() % 8), q = 1 + (int)(gen() % 20);
        DW::DSU_Weighted<ll> d(n);
        // the reference: an explicit value per node, plus which nodes are pinned together
        vector<ll> val(n + 1, 0);
        vector<int> grp(n + 1);
        iota(grp.begin(), grp.end(), 0);
        cases++;

        for (int step = 0; step < q; step++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
            ll w = (ll)(gen() % 11) - 5;
            // what the reference says: consistent iff u,v are unrelated, or already differ by exactly w
            bool refOk = (grp[u] != grp[v]) || (val[v] - val[u] == w);
            bool got = d.unite(u, v, w);
            if (got != refOk) uniteFail++;
            refOk ? accepted++ : contradictions++;
            if (refOk && grp[u] != grp[v]) { // merge in the reference: shift all of v's group
                ll shift = (val[u] + w) - val[v];
                int gu = grp[u], gv = grp[v];
                for (int x = 1; x <= n; x++)
                    if (grp[x] == gv) val[x] += shift, grp[x] = gu;
            }
            // every pair must agree with the reference, every time
            for (int a = 1; a <= n; a++)
                for (int b = 1; b <= n; b++) {
                    bool sameRef = grp[a] == grp[b];
                    if (d.same(a, b) != sameRef) compFail++;
                    if (sameRef && d.diff(a, b) != val[b] - val[a]) diffFail++;
                }
            for (int a = 1; a <= n; a++) {
                int cnt = 0;
                for (int b = 1; b <= n; b++) cnt += grp[a] == grp[b];
                if (d.size(a) != cnt) sizeFail++;
            }
            set<int> groups(grp.begin() + 1, grp.end());
            if (d.count_components() != (int)groups.size()) compFail++;
        }
    }

    // parity use: w in {0,1} must answer "same side or opposite" - i.e. bipartiteness, online
    for (int it = 0; it < 3000; it++) {
        int n = 2 + (int)(gen() % 7), q = 1 + (int)(gen() % 12);
        DW::DSU_Parity d(n);
        vector<array<int, 3>> kept;
        for (int step = 0; step < q; step++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n), w = (int)(gen() % 2);
            bool got = d.unite(u, v, w);
            if (got) kept.push_back({u, v, w});
            // brute force: is the kept set of parity constraints satisfiable, plus this new one?
            vector<array<int, 3>> trial = kept;
            if (!got) trial.push_back({u, v, w});
            bool sat = false;
            for (int mask = 0; mask < (1 << n) && !sat; mask++) {
                bool ok = true;
                for (auto &c : trial)
                    if ((((mask >> (c[0] - 1)) & 1) ^ ((mask >> (c[1] - 1)) & 1)) != c[2]) ok = false;
                if (ok) sat = true;
            }
            if (got != sat) parityFail++; // accepted iff adding it keeps the system satisfiable
        }
    }

    // the chain that defeats the original: unite(1,2), unite(2,3), ... then one find
    {
        int n = 200000;
        DW::DSU_Weighted<ll> d(n);
        for (int i = 1; i < n; i++) d.unite(i, i + 1, 1);
        if (d.diff(1, n) != n - 1) depthFail++;
        if (d.size(1) != n) depthFail++;
        if (!d.same(1, n)) depthFail++;
    }

    printf("DSU_Weighted  (%d random scenarios)\n", cases);
    printf("  unite verdict vs reference : %d failures  (%d accepted, %d contradictions)\n", uniteFail, accepted, contradictions);
    printf("  diff(u,v) vs reference     : %d failures\n", diffFail);
    printf("  same / component count     : %d failures\n", compFail);
    printf("  component size             : %d failures\n", sizeFail);
    printf("  parity vs exhaustive 2-col : %d failures\n", parityFail);
    printf("  chain of 200000 unites     : %d failures\n", depthFail);
}
