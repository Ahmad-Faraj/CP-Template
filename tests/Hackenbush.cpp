// Brute-force test for:
//   Game_Theory/Hackenbush.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;

namespace HB {
#include "../Game_Theory/Hackenbush.cpp"
}

int m_edges, N_NODES;
vector<pair<int, int>> E;
map<int, int> memo;

int alive(int mask) {
    vector<vector<pair<int, int>>> adj(N_NODES + 1);
    for (int i = 0; i < m_edges; i++)
        if (mask >> i & 1) {
            adj[E[i].first].push_back({E[i].second, i});
            adj[E[i].second].push_back({E[i].first, i});
        }
    vector<char> seen(N_NODES + 1, 0);
    vector<int> st{1};
    seen[1] = 1;
    int keep = 0;
    while (!st.empty()) {
        int u = st.back();
        st.pop_back();
        for (auto [v, id] : adj[u]) {
            keep |= 1 << id;
            if (!seen[v]) seen[v] = 1, st.push_back(v);
        }
    }
    return keep;
}

int grundy(int mask) {
    mask = alive(mask);
    auto it = memo.find(mask);
    if (it != memo.end()) return it->second;
    set<int> r;
    for (int i = 0; i < m_edges; i++)
        if (mask >> i & 1) r.insert(grundy(mask ^ (1 << i)));
    int g = 0;
    while (r.count(g)) g++;
    return memo[mask] = g;
}

int run(int n, const vector<pair<int, int>> &edges) {
    HB::Hackenbush h(n);
    h.set_ground(1);
    for (auto [u, v] : edges) h.add_edge(u, v);
    return h.grundy();
}

int main() {
    mt19937 gen(2024);
    int bad = 0, checked = 0, deepLoopCases = 0, deepBad = 0, nonzero = 0;
    vector<string> firstBad;

    for (int it = 0; it < 12000; it++) {
        int n = 1 + (int)(gen() % 6);
        int m = (int)(gen() % 9);
        vector<pair<int, int>> edges;
        for (int i = 0; i < m; i++) edges.push_back({1 + (int)(gen() % n), 1 + (int)(gen() % n)});
        N_NODES = n;
        E = edges;
        m_edges = (int)edges.size();
        int keep = alive(m_edges == 0 ? 0 : (1 << m_edges) - 1);
        vector<pair<int, int>> kept;
        for (int i = 0; i < m_edges; i++)
            if (keep >> i & 1) kept.push_back(edges[i]);
        E = kept;
        m_edges = (int)kept.size();
        memo.clear();
        int want = grundy(m_edges == 0 ? 0 : (1 << m_edges) - 1);
        int got = run(n, kept);
        checked++;
        if (want) nonzero++;
        bool deep = false;
        for (auto [u, v] : kept)
            if (u == v && u != 1) deep = true;
        if (deep) deepLoopCases++;
        if (want != got) {
            bad++;
            if (deep) deepBad++;
            if (firstBad.size() < 3) {
                string s = "n=" + to_string(n) + " edges:";
                for (auto [u, v] : kept) s += " " + to_string(u) + "-" + to_string(v);
                s += "  brute=" + to_string(want) + " got=" + to_string(got);
                firstBad.push_back(s);
            }
        }
    }

    // multiple ground nodes: grounding 1 and 2 fuses them
    int groundFail = 0;
    {
        HB::Hackenbush h(3);
        h.set_ground(1);
        h.set_ground(2);
        h.add_edge(1, 3);
        h.add_edge(2, 3); // with 1 and 2 fused this is a 2-cycle, value 0
        if (h.grundy() != 0) groundFail++;
    }
    {
        HB::Hackenbush h(4);
        h.set_ground(1);
        h.add_edge(1, 2);
        h.add_edge(2, 2); // a loop on a deep node: 1 (the loop) then +1 for the bridge = 2
        if (h.grundy() != 2) groundFail++;
    }

    printf("Hackenbush (fixed): %d random grounded graphs, %d with non-zero grundy\n", checked, nonzero);
    printf("  grundy vs the game definition : %d mismatches\n", bad);
    printf("  ... of which deep self-loops  : %d mismatches / %d such cases\n", deepBad, deepLoopCases);
    printf("  ground fusion + deep loop     : %d failures\n", groundFail);
    for (auto &s : firstBad) printf("    %s\n", s.c_str());
}
