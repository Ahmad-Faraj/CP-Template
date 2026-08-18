// Brute-force test for:
//   Tree/Euler_Tour.cpp
//   Tree/Diameter.cpp
//   Tree/Centroid_Decomposition.cpp
//   Tree/Tree_Isomorphism.cpp
// Compares each against an INDEPENDENT reference over randomised trees.
// Run with: py tools/test.py

#include "harness.h"
using namespace std;
using ll = long long;

namespace ET {
#include "../Tree/Euler_Tour.cpp"
}
namespace DI {
#include "../Tree/Diameter.cpp"
}
namespace CD {
#include "../Tree/Centroid_Decomposition.cpp"
}
namespace TI {
#include "../Tree/Tree_Isomorphism.cpp"
}

int main() {
    name("Tree family");
    mt19937_64 gen(11223344);
    ll etRange = 0, etSum = 0, etUpd = 0, diamFail = 0, pathFail = 0, farFail = 0;
    ll cdDepth = 0, cdCycle = 0, cdCover = 0, isoSame = 0, isoRoot = 0;
    ll cases = 0;

    for (int it = 0; it < 3000; it++) {
        int n = 1 + (int)(gen() % 14);
        vector<pair<int, int>> edges;
        vector<vector<int>> adj(n + 1);
        vector<int> par(n + 1, 0);
        for (int v = 2; v <= n; v++) {
            int p = 1 + (int)(gen() % (v - 1));
            par[v] = p;
            edges.push_back({p, v});
            adj[p].push_back(v), adj[v].push_back(p);
        }
        cases++;

        // subtree membership by walking parents, so the reference shares nothing with the tour
        auto in_subtree = [&](int root, int x) {
            for (int y = x; y; y = par[y])
                if (y == root) return true;
            return false;
        };

        // ---------- Euler_Tour: a subtree must be one contiguous range, and its sum must match ----------
        {
            ET::EulerTour et(n);
            for (auto pr : edges) et.add_edge(pr.first, pr.second);
            et.build(1);
            vector<ll> val(n + 1);
            for (int v = 1; v <= n; v++) val[v] = (ll)(gen() % 100) - 50;
            ET::SegTree seg(n);
            seg.build(et.flatten(val));
            for (int u = 1; u <= n; u++) {
                ll want = 0;
                int members = 0;
                for (int x = 1; x <= n; x++)
                    if (in_subtree(u, x)) want += val[x], members++;
                if (et.finish[u] - et.start[u] + 1 != members) etRange++;
                if (seg.query(et.start[u], et.finish[u]) != want) etSum++;
            }
            int target = 1 + (int)(gen() % n);
            seg.update(et.start[target], val[target] + 1000);
            ll before = 0;
            for (int v = 1; v <= n; v++) before += val[v];
            if (seg.query(et.start[1], et.finish[1]) != before + 1000) etUpd++;
        }

        // ---------- Diameter: against all-pairs BFS ----------
        {
            DI::Tree_Diameter td(n);
            for (auto pr : edges) td.add_edge(pr.first, pr.second);
            vector<vector<int>> d(n + 1, vector<int>(n + 1, -1));
            for (int s = 1; s <= n; s++) {
                d[s][s] = 0;
                vector<int> q{s};
                for (size_t i = 0; i < q.size(); i++)
                    for (int y : adj[q[i]])
                        if (d[s][y] < 0) d[s][y] = d[s][q[i]] + 1, q.push_back(y);
            }
            int best = 0;
            for (int a = 1; a <= n; a++)
                for (int b = 1; b <= n; b++) best = max(best, d[a][b]);
            array<int, 3> got = td.diameter();
            if (got[0] != best) diamFail++;
            if (d[got[1]][got[2]] != best) diamFail++; // the endpoints must really be that far apart
            vector<int> p = td.path();
            if ((int)p.size() != best + 1) pathFail++;
            for (size_t i = 0; i + 1 < p.size(); i++)
                if (d[p[i]][p[i + 1]] != 1) pathFail++; // consecutive nodes must be adjacent
            int src = 1 + (int)(gen() % n), f = td.farthest_from(src);
            int want = 0;
            for (int b = 1; b <= n; b++) want = max(want, d[src][b]);
            if (d[src][f] != want) farFail++;
        }

        // ---------- Centroid decomposition: the promise is O(log n) depth and a real tree ----------
        {
            CD::centroid_decomposition cd(n);
            for (auto pr : edges) cd.add_edge(pr.first, pr.second);
            cd.decompose(1, -1);
            int limit = 1;
            while ((1 << limit) <= n) limit++;
            int roots = 0;
            for (int v = 1; v <= n; v++) {
                if (cd.par[v] == -1) roots++;
                int steps = 0;
                for (int x = v; x != -1 && steps <= n + 1; x = cd.par[x]) steps++;
                if (steps > n + 1) cdCycle++; // following parents must terminate
                if (steps > limit + 2) cdDepth++;
            }
            if (roots != 1) cdCover++;
        }

        // ---------- Tree isomorphism: relabelling must not change the canonical form ----------
        {
            vector<int> perm(n + 1);
            iota(perm.begin(), perm.end(), 0);
            shuffle(perm.begin() + 1, perm.end(), gen);
            vector<vector<int>> relabelled(n + 1);
            for (auto pr : edges) {
                relabelled[perm[pr.first]].push_back(perm[pr.second]);
                relabelled[perm[pr.second]].push_back(perm[pr.first]);
            }
            if (TI::canonical_unrooted(adj, n) != TI::canonical_unrooted(relabelled, n)) isoSame++;
            if (TI::canonical_rooted(adj, 1) != TI::canonical_rooted(relabelled, perm[1])) isoRoot++;
        }
    }

    // a path and a star of the same size are NOT isomorphic - the form must say so
    ll isoDiff = 0;
    for (int n = 4; n <= 40; n++) {
        vector<vector<int>> path(n + 1), star(n + 1);
        for (int v = 2; v <= n; v++) {
            path[v - 1].push_back(v), path[v].push_back(v - 1);
            star[1].push_back(v), star[v].push_back(1);
        }
        if (TI::canonical_unrooted(path, n) == TI::canonical_unrooted(star, n)) isoDiff++;
    }

    check("Euler_Tour subtree is one range", etRange, cases);
    check("Euler_Tour subtree sum", etSum, cases);
    check("Euler_Tour point update", etUpd, cases);
    check("Diameter length and endpoints", diamFail, cases);
    check("Diameter path is a real walk", pathFail, cases);
    check("farthest_from", farFail, cases);
    check("centroid depth is O(log n)", cdDepth, cases);
    check("centroid parents terminate", cdCycle, cases);
    check("centroid tree has one root", cdCover, cases);
    check("isomorphism: relabel is invariant", isoSame, cases);
    check("isomorphism: rooted invariant", isoRoot, cases);
    check("isomorphism: path is not a star", isoDiff, 37);
    return report();
}
