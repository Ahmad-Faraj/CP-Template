// Brute-force test for:
//   Graph/MST_Kruskal.cpp
//   Graph/Depth_First_Search.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace MK {
#include "../Graph/MST_Kruskal.cpp"
}
namespace DF {
#include "../Graph/Depth_First_Search.cpp"
}

int main() {
    mt19937 gen(6543);
    int costFail = 0, spanFail = 0, treeFail = 0, maxFail = 0, idFail = 0, cutFail = 0;
    int compFail = 0, cycUFail = 0, cycDFail = 0, bipFail = 0, topoFail = 0, subFail = 0, ancFail = 0;
    int checked = 0;

    for (int it = 0; it < 3000; it++) {
        int n = 1 + (int)(gen() % 8);
        int m = (int)(gen() % 13);
        vector<array<ll, 3>> es;
        MK::MST_Kruskal mst(n);
        for (int i = 0; i < m; i++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
            ll w = (ll)(gen() % 30) - 5; // negatives included
            es.push_back({(ll)u, (ll)v, w});
            mst.add_edge(u, v, w);
        }
        ll got = mst.build();
        checked++;

        // reference MST: brute force over every subset of edges when m is small,
        // otherwise Prim from scratch on the dense matrix
        int comps;
        {
            vector<int> p(n + 1);
            iota(p.begin(), p.end(), 0);
            function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
            for (auto &e : es) p[f((int)e[0])] = f((int)e[1]);
            set<int> r;
            for (int i = 1; i <= n; i++) r.insert(f(i));
            comps = (int)r.size();
        }
        if (mst.is_spanning() != (comps == 1)) spanFail++;
        if (mst.component_count() != comps) compFail++;

        if (m <= 13) { // exhaustive: the cheapest acyclic spanning subgraph of the same shape
            ll best = LLONG_MAX;
            int wantEdges = n - comps;
            for (int mask = 0; mask < (1 << m); mask++) {
                if (__builtin_popcount(mask) != wantEdges) continue;
                vector<int> p(n + 1);
                iota(p.begin(), p.end(), 0);
                function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
                bool ok = true;
                ll sum = 0;
                for (int i = 0; i < m && ok; i++)
                    if (mask >> i & 1) {
                        int a = f((int)es[i][0]), b = f((int)es[i][1]);
                        if (a == b) ok = false; // would make a cycle
                        else p[a] = b, sum += es[i][2];
                    }
                if (!ok) continue;
                set<int> r;
                for (int i = 1; i <= n; i++) r.insert(f(i));
                if ((int)r.size() != comps) continue;
                best = min(best, sum);
            }
            if (best != LLONG_MAX && best != got) costFail++;
            if ((int)mst.edge_ids().size() != wantEdges) idFail++;
        }

        // the chosen edges must form a forest with exactly n - comps edges, no repeats
        {
            set<int> seen;
            vector<int> p(n + 1);
            iota(p.begin(), p.end(), 0);
            function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
            ll sum = 0;
            for (int id : mst.edge_ids()) {
                if (!seen.insert(id).second) treeFail++;
                int a = f((int)es[id][0]), b = f((int)es[id][1]);
                if (a == b) treeFail++; // a cycle among chosen edges
                p[a] = b;
                sum += es[id][2];
            }
            if (sum != got) treeFail++;
            auto adjT = mst.tree();
            int ends = 0;
            for (int i = 1; i <= n; i++) ends += (int)adjT[i].size();
            if (ends != 2 * (int)mst.edge_ids().size()) treeFail++;
        }

        // maximum spanning tree: negate every weight, run the minimum, and the costs must mirror
        {
            MK::MST_Kruskal mx(n), mn(n);
            for (auto &e : es) mx.add_edge((int)e[0], (int)e[1], e[2]), mn.add_edge((int)e[0], (int)e[1], -e[2]);
            if (mx.build(true) != -mn.build(false)) maxFail++;
        }

        // ---------- traversal, undirected ----------
        {
            DF::Depth_First_Search g(n, false);
            for (auto &e : es) g.add_edge((int)e[0], (int)e[1]);
            g.run();
            if (g.component_count() != comps) compFail++;
            // a cycle exists iff some component has more edges than nodes - 1
            bool wantCyc = (int)es.size() > n - comps;
            if (g.has_cycle() != wantCyc) cycUFail++;
            // bipartite by brute-force 2-colouring over all assignments
            bool wantBip = false;
            if (n <= 8) {
                for (int mask = 0; mask < (1 << n) && !wantBip; mask++) {
                    bool ok = true;
                    for (auto &e : es)
                        if (((mask >> ((int)e[0] - 1)) & 1) == ((mask >> ((int)e[1] - 1)) & 1)) { ok = false; break; }
                    if (ok) wantBip = true;
                }
                if (g.is_bipartite() != wantBip) bipFail++;
            }
            // subtree sizes must sum correctly and ancestors must be consistent
            for (int u = 1; u <= n; u++) {
                int cnt = 0;
                for (int v = 1; v <= n; v++)
                    if (g.is_ancestor(u, v)) cnt++;
                if (cnt != g.subtree[u]) subFail++;
            }
            for (int u = 1; u <= n; u++)
                for (int v = 1; v <= n; v++)
                    if (g.is_ancestor(u, v) && !g.same_component(u, v)) ancFail++;
        }

        // ---------- traversal, directed ----------
        {
            DF::Depth_First_Search g(n, true);
            for (auto &e : es) g.add_edge((int)e[0], (int)e[1]);
            g.run();
            // a directed cycle exists iff no topological order covers all nodes (Kahn)
            vector<int> indeg(n + 1, 0);
            vector<vector<int>> a(n + 1);
            for (auto &e : es) a[(int)e[0]].push_back((int)e[1]), indeg[(int)e[1]]++;
            queue<int> q;
            for (int i = 1; i <= n; i++)
                if (!indeg[i]) q.push(i);
            int placed = 0;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                placed++;
                for (int v : a[u])
                    if (--indeg[v] == 0) q.push(v);
            }
            bool wantCyc = placed != n;
            if (g.has_cycle() != wantCyc) cycDFail++;
            vector<int> topo = g.topological_order();
            if (wantCyc) {
                if (!topo.empty()) topoFail++;
            } else {
                if ((int)topo.size() != n) topoFail++;
                vector<int> pos(n + 1, -1);
                for (int i = 0; i < (int)topo.size(); i++) pos[topo[i]] = i;
                for (auto &e : es)
                    if (e[0] != e[1] && pos[(int)e[0]] > pos[(int)e[1]]) topoFail++; // every edge must go forward
            }
        }
    }

    // ---------- a hand-checked MST ----------
    {
        MK::MST_Kruskal m(5);
        m.add_edge(1, 2, 3);
        m.add_edge(2, 3, 5);
        m.add_edge(2, 4, 2);
        m.add_edge(3, 5, 7);
        m.add_edge(4, 5, 3);
        m.add_edge(1, 4, 1);
        ll c = m.build();
        // taken: 1-4(1), 2-4(2), 4-5(3), 2-3(5) = 11
        if (c != 11 || !m.is_spanning()) cutFail++;
        cout << "  hand-checked MST: cost " << c << " (want 11), spanning " << m.is_spanning() << '\n';
    }

    cout << "MST_Kruskal + Depth_First_Search: " << checked << " random graphs\n";
    cout << "  MST cost vs exhaustive subsets : " << costFail << " failures\n";
    cout << "  is_spanning / component_count  : " << spanFail << " failures\n";
    cout << "  chosen edges form a forest     : " << treeFail << " failures\n";
    cout << "  edge_ids count                 : " << idFail << " failures\n";
    cout << "  maximum spanning tree mirrors  : " << maxFail << " failures\n";
    cout << "  hand-checked case              : " << cutFail << " failures\n";
    cout << "  component count (traversal)    : " << compFail << " failures\n";
    cout << "  undirected cycle detection     : " << cycUFail << " failures\n";
    cout << "  directed cycle detection       : " << cycDFail << " failures\n";
    cout << "  bipartite vs brute 2-colouring : " << bipFail << " failures\n";
    cout << "  topological order valid        : " << topoFail << " failures\n";
    cout << "  subtree sizes vs is_ancestor   : " << subFail << " failures\n";
    cout << "  is_ancestor within component   : " << ancFail << " failures\n";
}
