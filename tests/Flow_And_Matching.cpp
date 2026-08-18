// Brute-force test for:
//   Graph/Flow_And_Matching/Bipartite_Matching.cpp
//   Graph/Flow_And_Matching/Bipartite_Matching_Hopcroft_Karp.cpp
//   Graph/Flow_And_Matching/Dinic.cpp
//   Graph/Flow_And_Matching/Minimum_Cost_Maximum_Flow.cpp
//   Misc/Huffman_Coding.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace BM {
#include "../Graph/Flow_And_Matching/Bipartite_Matching.cpp"
}
namespace HK {
#include "../Graph/Flow_And_Matching/Bipartite_Matching_Hopcroft_Karp.cpp"
}
namespace DN {
#include "../Graph/Flow_And_Matching/Dinic.cpp"
}
namespace MC {
#include "../Graph/Flow_And_Matching/Minimum_Cost_Maximum_Flow.cpp"
}
namespace HU {
#include "../Graph/Flow_And_Matching/Hungarian.cpp"
}

int main() {
    mt19937 gen(31337);
    int matchFail = 0, hkFail = 0, pairFail = 0, coverFail = 0, misFail = 0;
    int flowCutFail = 0, flowRefFail = 0, consFail = 0, pathFail = 0, biFlowFail = 0;
    int mcmfFail = 0, mcmfGoalFail = 0, hunFail = 0, hunMaxFail = 0, hunRectFail = 0;
    int mChecked = 0, fChecked = 0, cChecked = 0, hChecked = 0;

    // ---------- bipartite matching against exhaustive search ----------
    for (int it = 0; it < 3000; it++) {
        int n = 1 + (int)(gen() % 5), m = 1 + (int)(gen() % 5);
        vector<pair<int, int>> es;
        BM::Bipartite_Matching bm(n, m);
        HK::Hopcroft_Karp hk(n, m);
        set<pair<int, int>> uniq;
        int k = (int)(gen() % 10);
        for (int i = 0; i < k; i++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % m);
            bm.add_edge(u, v);
            hk.add_edge(u, v);
            es.push_back({u, v});
            uniq.insert({u, v});
        }
        int got = bm.maximum_matching(), gotHK = hk.maximum_matching();
        mChecked++;

        // brute force: every subset of left nodes assigned injectively to the right
        int best = 0;
        vector<int> perm(m);
        iota(perm.begin(), perm.end(), 1);
        // try every injection from left to right by brute-force over assignments
        function<void(int, int, int)> rec = [&](int u, int usedMask, int cnt) {
            if (u > n) {
                best = max(best, cnt);
                return;
            }
            rec(u + 1, usedMask, cnt); // leave u unmatched
            for (int v = 1; v <= m; v++)
                if (uniq.count({u, v}) && !((usedMask >> v) & 1)) rec(u + 1, usedMask | (1 << v), cnt + 1);
        };
        rec(1, 0, 0);
        if (got != best) matchFail++;
        if (gotHK != best) hkFail++;

        // the reported pairs must be a valid matching of exactly that size
        {
            auto pr = bm.pairs();
            if ((int)pr.size() != got) pairFail++;
            set<int> L, R;
            for (auto [u, v] : pr) {
                if (!uniq.count({u, v})) pairFail++;
                if (!L.insert(u).second || !R.insert(v).second) pairFail++;
            }
            auto pr2 = hk.pairs();
            if ((int)pr2.size() != gotHK) pairFail++;
            set<int> L2, R2;
            for (auto [u, v] : pr2) {
                if (!uniq.count({u, v})) pairFail++;
                if (!L2.insert(u).second || !R2.insert(v).second) pairFail++;
            }
        }

        // Konig: the cover must have matching size and touch every edge
        {
            auto [cl, cr] = bm.minimum_vertex_cover();
            if ((int)(cl.size() + cr.size()) != got) coverFail++;
            vector<char> inL(n + 1, 0), inR(m + 1, 0);
            for (int x : cl) inL[x] = 1;
            for (int x : cr) inR[x] = 1;
            for (auto [u, v] : uniq)
                if (!inL[u] && !inR[v]) coverFail++; // an uncovered edge
            // the independent set is the complement and must have no internal edge
            auto [il, ir] = bm.maximum_independent_set();
            if ((int)(il.size() + ir.size()) != n + m - got) misFail++;
            vector<char> sL(n + 1, 0), sR(m + 1, 0);
            for (int x : il) sL[x] = 1;
            for (int x : ir) sR[x] = 1;
            for (auto [u, v] : uniq)
                if (sL[u] && sR[v]) misFail++;
        }

        // matching via max flow must agree
        {
            DN::Dinic f(n + m + 2);
            int S = 0, T = n + m + 1;
            for (int u = 1; u <= n; u++) f.add_edge(S, u, 1);
            for (int v = 1; v <= m; v++) f.add_edge(n + v, T, 1);
            for (auto [u, v] : uniq) f.add_edge(u, n + v, 1);
            if (f.max_flow(S, T) != best) biFlowFail++;
        }
    }

    // ---------- max flow: value equals the reported cut, and matches a brute-force min cut ----------
    for (int it = 0; it < 1500; it++) {
        int n = 2 + (int)(gen() % 6);
        int m = (int)(gen() % 12);
        vector<array<ll, 3>> es;
        DN::Dinic f(n + 1);
        for (int i = 0; i < m; i++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
            ll c = 1 + (ll)(gen() % 8);
            f.add_edge(u, v, c, i);
            es.push_back({(ll)u, (ll)v, c});
        }
        ll got = f.max_flow(1, n);
        fChecked++;

        // brute-force min cut: try every subset containing s and not t
        ll bestCut = LLONG_MAX;
        for (int mask = 0; mask < (1 << n); mask++) {
            if (!((mask >> 0) & 1)) continue;          // node 1 must be on the s side
            if ((mask >> (n - 1)) & 1) continue;       // node n must not be
            ll cut = 0;
            for (auto &e : es) {
                bool su = (mask >> ((int)e[0] - 1)) & 1, sv = (mask >> ((int)e[1] - 1)) & 1;
                if (su && !sv) cut += e[2];
            }
            bestCut = min(bestCut, cut);
        }
        if (bestCut != LLONG_MAX && got != bestCut) flowRefFail++;

        // the cut it reports must have exactly the max-flow capacity
        {
            auto side = f.min_cut_side();
            ll cap = 0;
            for (auto &e : es)
                if (side[(int)e[0]] && !side[(int)e[1]]) cap += e[2];
            if (cap != got) flowCutFail++;
            if (!side[1] || side[n]) flowCutFail++; // s inside, t outside
            auto cutEdges = f.min_cut_edges();
            ll cap2 = 0;
            for (auto [u, v] : cutEdges) {
                (void)u;
                (void)v;
                cap2++;
            }
            if (cap2 == 0 && got > 0) flowCutFail++;
        }

        // conservation: per-node inflow equals outflow, except at s and t
        {
            vector<ll> net(n + 2, 0);
            for (int i = 0; i < m; i++) {
                ll fl = f.flow_on(i);
                if (fl < 0 || fl > es[i][2]) consFail++; // within capacity
                net[(int)es[i][0]] -= fl;
                net[(int)es[i][1]] += fl;
            }
            for (int v = 2; v < n; v++)
                if (net[v] != 0) consFail++;
            if (net[n] != got) consFail++;
        }
    }

    // ---------- unit-capacity path decomposition ----------
    for (int it = 0; it < 500; it++) {
        int n = 2 + (int)(gen() % 5), m = (int)(gen() % 10);
        DN::Dinic f(n + 1);
        for (int i = 0; i < m; i++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
            f.add_edge(u, v, 1, i);
        }
        ll got = f.max_flow(1, n);
        auto paths = f.decompose_paths();
        if ((ll)paths.size() != got) pathFail++;
        for (auto &p : paths) {
            if (p.empty() || p.front() != 1 || p.back() != n) pathFail++;
        }
        // decompose_paths must not destroy the flow: a second call gives the same answer
        auto again = f.decompose_paths();
        if (again.size() != paths.size()) pathFail++;
    }

    // ---------- min cost max flow: against brute force on tiny graphs ----------
    for (int it = 0; it < 1200; it++) {
        int n = 2 + (int)(gen() % 4);
        int m = (int)(gen() % 8);
        MC::Minimum_Cost_Maximum_Flow mc(n + 1);
        DN::Dinic dn(n + 1);
        vector<array<ll, 4>> es;
        for (int i = 0; i < m; i++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
            ll cap = 1 + (ll)(gen() % 3), cost = (ll)(gen() % 9);
            mc.add_edge(u, v, cap, cost, i);
            dn.add_edge(u, v, cap, i);
            es.push_back({(ll)u, (ll)v, cap, cost});
        }
        auto [flow, cost] = mc.run(1, n);
        // the flow value must equal plain max flow
        if (flow != dn.max_flow(1, n)) mcmfFail++;
        // conservation and capacity, and the cost must match the per-edge flows
        {
            vector<ll> net(n + 2, 0);
            ll recomputed = 0;
            for (int i = 0; i < m; i++) {
                ll fl = mc.flow_on(i);
                if (fl < 0 || fl > es[i][2]) mcmfFail++;
                net[(int)es[i][0]] -= fl;
                net[(int)es[i][1]] += fl;
                recomputed += fl * es[i][3];
            }
            for (int v = 2; v < n; v++)
                if (net[v] != 0) mcmfFail++;
            if (net[n] != flow) mcmfFail++;
            if (recomputed != cost) mcmfFail++;
        }
        // a goal below the max must be met exactly, and cost must not decrease with more flow
        if (flow >= 1) {
            MC::Minimum_Cost_Maximum_Flow half(n + 1);
            for (int i = 0; i < m; i++) half.add_edge((int)es[i][0], (int)es[i][1], es[i][2], es[i][3], i);
            auto [f2, c2] = half.run(1, n, 1);
            if (f2 != 1) mcmfGoalFail++;
            if (c2 > cost) mcmfGoalFail++; // one unit cannot cost more than all units
        }
        cChecked++;
    }

    // ---------- Hungarian: against brute-force permutations, and against MCMF ----------
    for (int it = 0; it < 1500; it++) {
        int n = 1 + (int)(gen() % 6);
        vector<vector<ll>> c(n + 1, vector<ll>(n + 1, 0));
        HU::Hungarian h(n, n);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) {
                ll x = (ll)(gen() % 20) - 5; // negatives included
                c[i][j] = x;
                h.set_cost(i, j, x);
            }
        ll got = h.minimum_cost();
        hChecked++;
        // brute force over all permutations
        vector<int> perm(n);
        iota(perm.begin(), perm.end(), 1);
        ll best = LLONG_MAX, worst = LLONG_MIN;
        do {
            ll sum = 0;
            for (int i = 1; i <= n; i++) sum += c[i][perm[i - 1]];
            best = min(best, sum);
            worst = max(worst, sum);
        } while (next_permutation(perm.begin(), perm.end()));
        if (got != best) hunFail++;
        // the reported assignment must be a permutation with exactly that cost
        {
            auto pr = h.pairs();
            if ((int)pr.size() != n) hunFail++;
            set<int> seenI, seenJ;
            ll sum = 0;
            for (auto [i, j] : pr) {
                if (!seenI.insert(i).second || !seenJ.insert(j).second) hunFail++;
                sum += c[i][j];
            }
            if (sum != got) hunFail++;
        }
        // maximum_cost must find the worst permutation
        {
            HU::Hungarian mx(n, n);
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= n; j++) mx.set_cost(i, j, c[i][j]);
            if (mx.maximum_cost() != worst) hunMaxFail++;
        }
    }

    // ---------- Hungarian on a rectangular matrix, checked against MCMF ----------
    for (int it = 0; it < 400; it++) {
        int r = 1 + (int)(gen() % 4), cc = r + (int)(gen() % 3); // more jobs than workers
        vector<vector<ll>> c(r + 1, vector<ll>(cc + 1, 0));
        HU::Hungarian h(r, cc);
        for (int i = 1; i <= r; i++)
            for (int j = 1; j <= cc; j++) {
                ll x = (ll)(gen() % 15);
                c[i][j] = x;
                h.set_cost(i, j, x);
            }
        ll got = h.minimum_cost();
        // MCMF model: source -> workers (cap 1), workers -> jobs (cap 1, cost), jobs -> sink (cap 1)
        MC::Minimum_Cost_Maximum_Flow mc(r + cc + 2);
        int S = 0, T = r + cc + 1;
        for (int i = 1; i <= r; i++) mc.add_edge(S, i, 1, 0);
        for (int j = 1; j <= cc; j++) mc.add_edge(r + j, T, 1, 0);
        for (int i = 1; i <= r; i++)
            for (int j = 1; j <= cc; j++) mc.add_edge(i, r + j, 1, c[i][j]);
        auto [fl, cost] = mc.run(S, T);
        if (fl != r) hunRectFail++;
        if (cost != got) hunRectFail++;
    }

    cout << "Flow_And_Matching\n";
    cout << "  Kuhn matching vs exhaustive     : " << matchFail << " failures (" << mChecked << " graphs)\n";
    cout << "  Hopcroft-Karp agrees with Kuhn  : " << hkFail << " failures\n";
    cout << "  reported pairs are a matching   : " << pairFail << " failures\n";
    cout << "  Konig minimum vertex cover      : " << coverFail << " failures\n";
    cout << "  maximum independent set         : " << misFail << " failures\n";
    cout << "  matching via max flow agrees    : " << biFlowFail << " failures\n";
    cout << "  max flow vs brute-force min cut : " << flowRefFail << " failures (" << fChecked << " graphs)\n";
    cout << "  reported cut has flow capacity  : " << flowCutFail << " failures\n";
    cout << "  flow conservation and capacity  : " << consFail << " failures\n";
    cout << "  unit-capacity path decomposition: " << pathFail << " failures\n";
    cout << "  MCMF flow, cost, conservation   : " << mcmfFail << " failures (" << cChecked << " graphs)\n";
    cout << "  MCMF flow goal honoured         : " << mcmfGoalFail << " failures\n";
    cout << "  Hungarian vs all permutations   : " << hunFail << " failures (" << hChecked << " matrices)\n";
    cout << "  Hungarian maximum_cost          : " << hunMaxFail << " failures\n";
    cout << "  Hungarian rectangular vs MCMF   : " << hunRectFail << " failures\n";
}
