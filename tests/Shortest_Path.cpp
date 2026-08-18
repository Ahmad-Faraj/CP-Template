// Brute-force test for:
//   Graph/Shortest_Path/Bellman_Ford.cpp
//   Graph/Shortest_Path/Bellman_Ford_SPFA.cpp
//   Graph/Shortest_Path/Floyd.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

// One harness for all three shortest-path templates. Each is namespaced so the three
// can coexist, and each is checked against a slow but obviously-correct reference.
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace BF {
#include "../Graph/Shortest_Path/Bellman_Ford.cpp"
}
namespace SP {
#include "../Graph/Shortest_Path/Bellman_Ford_SPFA.cpp"
}
namespace FW {
#include "../Graph/Shortest_Path/Floyd.cpp"
}

// ---- reference: distances allowing negative edges, by n rounds of exhaustive relaxation ----
// Nodes are marked unbounded if a round n..2n still improves them.
struct Ref {
    int n;
    vector<array<ll, 3>> e;
    static const ll BIG = (ll)4e18 / 4;
    Ref(int n) : n(n) {}
    void add(int u, int v, ll w) { e.push_back({(ll)u, (ll)v, w}); }
    // returns dist, plus a flag per node: 0 normal, 1 unreachable, 2 unbounded below
    pair<vector<ll>, vector<int>> run(int src) {
        vector<ll> d(n + 1, BIG);
        d[src] = 0;
        for (int i = 0; i < n - 1; i++)
            for (auto &x : e)
                if (d[x[0]] < BIG && d[x[0]] + x[2] < d[x[1]]) d[x[1]] = d[x[0]] + x[2];
        // find seeds that still improve, then spread reachability
        vector<char> bad(n + 1, 0);
        for (auto &x : e)
            if (d[x[0]] < BIG && d[x[0]] + x[2] < d[x[1]]) bad[x[1]] = 1;
        for (int rounds = 0; rounds <= n + 2; rounds++)
            for (auto &x : e)
                if (bad[x[0]]) bad[x[1]] = 1;
        vector<int> flag(n + 1, 0);
        for (int i = 1; i <= n; i++) flag[i] = bad[i] ? 2 : (d[i] >= BIG ? 1 : 0);
        return {d, flag};
    }
};

int main() {
    mt19937 gen(4477);
    int bfFail = 0, spFail = 0, fwFail = 0, negFlagFail = 0, spNegFail = 0;
    int bfPathFail = 0, fwPathFail = 0, apspFail = 0, addFail = 0;
    int checked = 0, negGraphs = 0;

    for (int it = 0; it < 4000; it++) {
        int n = 1 + (int)(gen() % 7);
        int m = (int)(gen() % 12);
        bool allowNeg = (it % 2 == 0);
        BF::Bellman_Ford<ll> bf(n);
        SP::SPFA<ll> sp(n);
        FW::Floyd<ll> fw(n);
        Ref ref(n);
        vector<array<ll, 3>> edges;
        for (int i = 0; i < m; i++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
            ll w = allowNeg ? (ll)(gen() % 21) - 10 : (ll)(gen() % 15);
            bf.add_edge(u, v, w);
            sp.add_edge(u, v, w);
            fw.add_edge(u, v, w);
            ref.add(u, v, w);
            edges.push_back({(ll)u, (ll)v, w});
        }
        fw.build();

        int src = 1 + (int)(gen() % n);
        auto [rd, rflag] = ref.run(src);
        bool anyNeg = false;
        for (int i = 1; i <= n; i++)
            if (rflag[i] == 2) anyNeg = true;
        if (anyNeg) negGraphs++;

        vector<ll> bd = bf.shortest_path(src);
        for (int v = 1; v <= n; v++) {
            if (rflag[v] == 2) {
                if (!BF::Bellman_Ford<ll>::is_neg(bd[v])) negFlagFail++;
            } else if (rflag[v] == 1) {
                if (!BF::Bellman_Ford<ll>::is_inf(bd[v])) bfFail++;
            } else if (bd[v] != rd[v])
                bfFail++;
        }
        checked++;

        // SPFA: when there is no negative cycle it must match exactly
        vector<ll> sd = sp.shortest_path(src);
        if (!anyNeg) {
            if (sp.neg_cycle) spNegFail++;
            for (int v = 1; v <= n; v++) {
                if (rflag[v] == 1) {
                    if (!SP::SPFA<ll>::is_inf(sd[v])) spFail++;
                } else if (sd[v] != rd[v])
                    spFail++;
            }
        } else if (!sp.neg_cycle)
            spNegFail++; // a reachable negative cycle must be reported

        // Floyd: all pairs, compared against Bellman Ford from every source
        if (!fw.has_negative_cycle()) {
            for (int s = 1; s <= n; s++) {
                BF::Bellman_Ford<ll> one(n);
                for (auto &x : edges) one.add_edge((int)x[0], (int)x[1], x[2]);
                vector<ll> od = one.shortest_path(s);
                for (int t = 1; t <= n; t++) {
                    bool oInf = BF::Bellman_Ford<ll>::is_inf(od[t]);
                    bool fInf = FW::Floyd<ll>::is_inf(fw.get(s, t));
                    if (oInf != fInf) { apspFail++; continue; }
                    if (!oInf && od[t] != fw.get(s, t)) apspFail++;
                }
            }
        } else if (!anyNeg) {
            // Floyd sees cycles anywhere; the reference only sees those reachable from src.
            // So a Floyd-detected cycle unreachable from src is not a contradiction.
        }

        // paths must be real walks with the right total weight
        auto edgeWeight = [&](int a, int b) {
            ll best = Ref::BIG;
            for (auto &x : edges)
                if (x[0] == a && x[1] == b) best = min(best, x[2]);
            return best;
        };
        for (int t = 1; t <= n; t++) {
            vector<int> p = bf.path(src, t);
            if (rflag[t] == 0) {
                if (p.empty() || p.front() != src || p.back() != t) { bfPathFail++; continue; }
                ll tot = 0;
                for (size_t i = 0; i + 1 < p.size(); i++) {
                    ll w = edgeWeight(p[i], p[i + 1]);
                    if (w >= Ref::BIG) { bfPathFail++; break; }
                    tot += w;
                }
                if (tot != rd[t]) bfPathFail++;
            } else if (!p.empty())
                bfPathFail++;
        }
        if (!fw.has_negative_cycle()) {
            for (int s = 1; s <= n; s++)
                for (int t = 1; t <= n; t++) {
                    vector<int> p = fw.path(s, t);
                    if (FW::Floyd<ll>::is_inf(fw.get(s, t))) {
                        if (!p.empty()) fwPathFail++;
                        continue;
                    }
                    if (p.empty() || p.front() != s || p.back() != t) { fwPathFail++; continue; }
                    ll tot = 0;
                    for (size_t i = 0; i + 1 < p.size(); i++) {
                        ll w = edgeWeight(p[i], p[i + 1]);
                        if (w >= Ref::BIG) { fwPathFail++; break; }
                        tot += w;
                    }
                    if (tot != fw.get(s, t)) fwPathFail++;
                }
        }
    }

    // ---- add_edge_after_build must match a full rebuild ----
    for (int it = 0; it < 500; it++) {
        int n = 2 + (int)(gen() % 6), m = (int)(gen() % 10);
        vector<array<ll, 3>> edges;
        FW::Floyd<ll> inc(n);
        for (int i = 0; i < m; i++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
            ll w = (ll)(gen() % 15);
            edges.push_back({(ll)u, (ll)v, w});
            inc.add_edge(u, v, w);
        }
        inc.build();
        int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
        ll w = (ll)(gen() % 15);
        inc.add_edge_after_build(u, v, w);
        FW::Floyd<ll> full(n);
        for (auto &x : edges) full.add_edge((int)x[0], (int)x[1], x[2]);
        full.add_edge(u, v, w);
        full.build();
        for (int a = 1; a <= n; a++)
            for (int b = 1; b <= n; b++)
                if (inc.get(a, b) != full.get(a, b)) addFail++;
    }

    // ---- fixed cases ----
    {
        // a plain negative edge that Dijkstra would get wrong
        BF::Bellman_Ford<ll> b(3);
        b.add_edge(1, 2, 5);
        b.add_edge(2, 3, -10);
        b.add_edge(1, 3, 1);
        vector<ll> d = b.shortest_path(1);
        if (d[3] != -5) bfFail++;
        // a negative cycle NOT reachable from the source must not poison the source's answers
        BF::Bellman_Ford<ll> c(5);
        c.add_edge(1, 2, 3);
        c.add_edge(3, 4, -5);
        c.add_edge(4, 3, -5); // negative cycle among 3,4 - unreachable from 1
        vector<ll> cd = c.shortest_path(1);
        if (cd[2] != 3) bfFail++;
        if (!BF::Bellman_Ford<ll>::is_inf(cd[3])) bfFail++;
        if (c.has_negative_cycle(1)) negFlagFail++;
        if (!c.has_negative_cycle(3)) negFlagFail++;
    }

    cout << "Shortest_Path: " << checked << " random graphs (" << negGraphs << " with a reachable negative cycle)\n";
    cout << "  Bellman_Ford distances        : " << bfFail << " failures\n";
    cout << "  Bellman_Ford NEG marking      : " << negFlagFail << " failures\n";
    cout << "  Bellman_Ford path is a walk   : " << bfPathFail << " failures\n";
    cout << "  SPFA distances (no neg cycle) : " << spFail << " failures\n";
    cout << "  SPFA negative-cycle verdict   : " << spNegFail << " failures\n";
    cout << "  Floyd all pairs vs Bellman    : " << apspFail << " failures\n";
    cout << "  Floyd path is a walk          : " << fwPathFail << " failures\n";
    cout << "  Floyd add_edge_after_build    : " << addFail << " failures\n";
}
