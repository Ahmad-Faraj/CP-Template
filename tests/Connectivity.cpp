// Brute-force test for:
//   Graph/Connectivity/Strongly_Connected_Components.cpp
//   Graph/Connectivity/Bridges.cpp
//   Graph/Connectivity/Articulation_Points.cpp
//   Graph/Connectivity/Bridges_Online.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

// Connectivity suite. The references remove an edge or a vertex and recount components -
// that is the definition, so nothing here shares logic with the low-link templates.
#include <bits/stdc++.h>
using namespace std;

namespace SCC {
#include "../Graph/Connectivity/Strongly_Connected_Components.cpp"
}
namespace BR {
#include "../Graph/Connectivity/Bridges.cpp"
}
namespace AP {
#include "../Graph/Connectivity/Articulation_Points.cpp"
}
namespace OB {
#include "../Graph/Connectivity/Bridges_Online.cpp"
}

// count connected components of an undirected edge list over nodes 1..n, skipping banned
int count_components(int n, const vector<pair<int, int>> &e, int skipEdge = -1, int skipNode = -1) {
    vector<int> p(n + 1);
    iota(p.begin(), p.end(), 0);
    function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
    for (int i = 0; i < (int)e.size(); i++) {
        if (i == skipEdge) continue;
        auto [u, v] = e[i];
        if (u == skipNode || v == skipNode) continue;
        p[f(u)] = f(v);
    }
    set<int> roots;
    for (int i = 1; i <= n; i++)
        if (i != skipNode) roots.insert(f(i));
    return (int)roots.size();
}

int main() {
    mt19937 gen(8123);
    int sccFail = 0, condFail = 0, ancFail = 0;
    int brFail = 0, br2eccFail = 0, brTreeFail = 0;
    int apFail = 0, apPiecesFail = 0;
    int obFail = 0, ob2eccFail = 0;
    int checked = 0, multiCases = 0;

    for (int it = 0; it < 3000; it++) {
        int n = 1 + (int)(gen() % 8);
        int m = (int)(gen() % 12);
        bool forceMulti = (it % 3 == 0);

        // ---------- directed: SCC ----------
        {
            SCC::Strongly_Connected_Components s(n);
            vector<pair<int, int>> de;
            for (int i = 0; i < m; i++) {
                int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
                s.add_edge(u, v);
                de.push_back({u, v});
            }
            s.run();
            // reference: u,v in the same SCC iff u reaches v and v reaches u
            vector<vector<char>> reach(n + 1, vector<char>(n + 1, 0));
            for (int i = 1; i <= n; i++) reach[i][i] = 1;
            for (auto [u, v] : de) reach[u][v] = 1;
            for (int k = 1; k <= n; k++)
                for (int i = 1; i <= n; i++)
                    for (int j = 1; j <= n; j++)
                        if (reach[i][k] && reach[k][j]) reach[i][j] = 1;
            for (int u = 1; u <= n; u++)
                for (int v = 1; v <= n; v++) {
                    bool want = reach[u][v] && reach[v][u];
                    if (s.same_component(u, v) != want) sccFail++;
                }
            // condensation must be acyclic, and ids must be in reverse topological order
            auto dag = s.condensation();
            for (int c = 1; c <= s.component_count(); c++)
                for (int d : dag[c])
                    if (d >= c) condFail++; // every edge must go to a strictly lower id
            // is_ancestor must agree with the DFS-forest reachability it encodes
            for (int u = 1; u <= n; u++)
                for (int v = 1; v <= n; v++)
                    if (s.is_ancestor(u, v) && !reach[u][v]) ancFail++;
        }

        // ---------- undirected: bridges, articulation points, online bridges ----------
        vector<pair<int, int>> ue;
        BR::Bridges b(n);
        AP::Articulation_Points ap(n);
        OB::Bridges_Online ob(n);
        for (int i = 0; i < m; i++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
            ue.push_back({u, v});
            if (forceMulti && i > 0 && (gen() % 2)) { // duplicate the previous edge
                ue.back() = ue[i - 1];
                multiCases++;
            }
            b.add_edge(ue.back().first, ue.back().second);
            ap.add_edge(ue.back().first, ue.back().second);
            ob.add_edge(ue.back().first, ue.back().second);
        }
        b.run();
        ap.run();
        checked++;

        int base = count_components(n, ue);

        // a bridge is an edge whose removal raises the component count
        set<int> wantBridgeIdx;
        for (int i = 0; i < (int)ue.size(); i++)
            if (count_components(n, ue, i) > base) wantBridgeIdx.insert(i);
        if ((int)wantBridgeIdx.size() != b.bridge_count()) brFail++;
        // and each reported bridge must be one of those edges
        {
            multiset<pair<int, int>> reported;
            for (auto [u, v] : b.bridge_list) reported.insert({min(u, v), max(u, v)});
            multiset<pair<int, int>> want;
            for (int i : wantBridgeIdx) want.insert({min(ue[i].first, ue[i].second), max(ue[i].first, ue[i].second)});
            if (reported != want) brFail++;
        }
        // same-component test on the full edge list
        auto same_comp = [&](int u, int v) {
            vector<int> p(n + 1);
            iota(p.begin(), p.end(), 0);
            function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
            for (auto [a, c] : ue) p[f(a)] = f(c);
            return f(u) == f(v);
        };

        // two-edge-connected: u,v connected and no single edge removal separates them
        for (int u = 1; u <= n; u++)
            for (int v = 1; v <= n; v++) {
                if (!same_comp(u, v)) continue;
                bool separable = false;
                for (int i = 0; i < (int)ue.size() && !separable; i++) {
                    vector<int> p(n + 1);
                    iota(p.begin(), p.end(), 0);
                    function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
                    for (int j = 0; j < (int)ue.size(); j++)
                        if (j != i) p[f(ue[j].first)] = f(ue[j].second);
                    if (f(u) != f(v)) separable = true;
                }
                bool want2 = !separable;
                if ((b.component_of(u) == b.component_of(v)) != want2) br2eccFail++;
                if (ob.two_edge_connected(u, v) != want2) ob2eccFail++;
            }
        // the online structure must reach the same bridge count as the offline one
        if (ob.bridge_count() != b.bridge_count()) obFail++;
        // the bridge tree must be a forest: components - 1 edges per connected piece
        {
            auto tree = b.bridge_tree();
            int edgeEnds = 0;
            for (int c = 1; c <= b.component_count(); c++) edgeEnds += (int)tree[c].size();
            if (edgeEnds != 2 * b.bridge_count()) brTreeFail++;
        }

        // v is a cut vertex exactly when deleting it splits v's own component into 2+ pieces
        for (int v = 1; v <= n; v++) {
            vector<int> mine;
            for (int u = 1; u <= n; u++)
                if (u != v && same_comp(u, v)) mine.push_back(u);
            bool want = false;
            int pieceCount = 1;
            if (!mine.empty()) {
                vector<int> q(n + 1);
                iota(q.begin(), q.end(), 0);
                function<int(int)> g = [&](int x) { return q[x] == x ? x : q[x] = g(q[x]); };
                for (auto [a, c] : ue)
                    if (a != v && c != v) q[g(a)] = g(c);
                set<int> pieces;
                for (int u : mine) pieces.insert(g(u));
                pieceCount = (int)pieces.size();
                want = pieceCount > 1;
            }
            if (ap.is_articulation(v) != want) apFail++;
            if (ap.components_after_removing(v) != pieceCount) apPiecesFail++;
        }
    }

    cout << "Connectivity: " << checked << " random graphs (" << multiCases << " forced multi-edges)\n";
    cout << "  SCC membership vs reachability : " << sccFail << " failures\n";
    cout << "  condensation is a reverse-topo DAG : " << condFail << " failures\n";
    cout << "  is_ancestor implies reachability   : " << ancFail << " failures\n";
    cout << "  bridges vs edge-removal test   : " << brFail << " failures\n";
    cout << "  2-edge-connected components    : " << br2eccFail << " failures\n";
    cout << "  bridge_tree is a forest        : " << brTreeFail << " failures\n";
    cout << "  articulation vs vertex removal : " << apFail << " failures\n";
    cout << "  components_after_removing      : " << apPiecesFail << " failures\n";
    cout << "  online bridge count matches    : " << obFail << " failures\n";
    cout << "  online two_edge_connected      : " << ob2eccFail << " failures\n";
}
