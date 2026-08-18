// Brute-force test for:
//   Tree/Rerooting.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace RR {
#include "../Tree/Rerooting.cpp"
}
using RR::Rerooting;

// The reference: root the tree at r and run the SAME down-DP explicitly. This shares the
// down pass but nothing of the up pass, which is the only part that can be wrong.
template <typename T>
T brute_at(int r, int n, vector<vector<pair<int, ll>>> &adj, function<T()> id, function<T(T, T)> mg,
           function<T(T, int, int, ll)> ae, function<T(T, int)> an) {
    function<T(int, int)> go = [&](int u, int p) {
        T acc = id();
        for (auto [v, w] : adj[u])
            if (v != p) acc = mg(acc, ae(go(v, u), v, u, w));
        return an(acc, u);
    };
    return go(r, 0);
}

int main() {
    mt19937_64 gen(20260818);
    int sumFail = 0, maxFail = 0, cntFail = 0, wFail = 0, downFail = 0, cases = 0;

    for (int it = 0; it < 4000; it++) {
        int n = 1 + (int)(gen() % 9);
        vector<vector<pair<int, ll>>> adj(n + 1);
        vector<array<ll, 3>> edges;
        for (int v = 2; v <= n; v++) { // random tree, every parent uniform among earlier nodes
            int p = 1 + (int)(gen() % (v - 1));
            ll w = 1 + (ll)(gen() % 9);
            edges.push_back({(ll)p, (ll)v, w});
            adj[p].push_back({v, w});
            adj[v].push_back({p, w});
        }
        cases++;

        // ---- 1. unweighted distance sum: (count, total distance)
        {
            using T = pair<ll, ll>;
            function<T()> id = [] { return make_pair(0LL, 0LL); };
            function<T(T, T)> mg = [](T a, T b) { return make_pair(a.first + b.first, a.second + b.second); };
            function<T(T, int, int, ll)> ae = [](T v, int, int, ll) { return make_pair(v.first, v.second + v.first); };
            function<T(T, int)> an = [](T v, int) { return make_pair(v.first + 1, v.second); };
            Rerooting<T> r(n);
            r.identity = id; r.merge = mg; r.add_edge = ae; r.add_node = an;
            for (auto &e : edges) r.add((int)e[0], (int)e[1], e[2]);
            r.run(1);
            for (int v = 1; v <= n; v++) {
                vector<vector<pair<int, ll>>> a2(n + 1);
                for (auto &e : edges) a2[e[0]].push_back({(int)e[1], 1}), a2[e[1]].push_back({(int)e[0], 1});
                if (r.answer[v] != brute_at<T>(v, n, a2, id, mg, ae, an)) sumFail++;
            }
            if (r.answer[1] != r.down[1]) downFail++; // rooted at run()'s root, the two must agree
        }

        // ---- 2. weighted distance sum
        {
            using T = pair<ll, ll>;
            function<T()> id = [] { return make_pair(0LL, 0LL); };
            function<T(T, T)> mg = [](T a, T b) { return make_pair(a.first + b.first, a.second + b.second); };
            function<T(T, int, int, ll)> ae = [](T v, int, int, ll w) { return make_pair(v.first, v.second + v.first * w); };
            function<T(T, int)> an = [](T v, int) { return make_pair(v.first + 1, v.second); };
            Rerooting<T> r(n);
            r.identity = id; r.merge = mg; r.add_edge = ae; r.add_node = an;
            for (auto &e : edges) r.add((int)e[0], (int)e[1], e[2]);
            r.run(1);
            for (int v = 1; v <= n; v++)
                if (r.answer[v] != brute_at<T>(v, n, adj, id, mg, ae, an)) wFail++;
        }

        // ---- 3. farthest node (CSES 1132): a MAX merge, not a sum
        {
            using T = ll;
            function<T()> id = [] { return 0LL; };
            function<T(T, T)> mg = [](T a, T b) { return max(a, b); };
            function<T(T, int, int, ll)> ae = [](T v, int, int, ll w) { return v + w; };
            function<T(T, int)> an = [](T v, int) { return v; };
            Rerooting<T> r(n);
            r.identity = id; r.merge = mg; r.add_edge = ae; r.add_node = an;
            for (auto &e : edges) r.add((int)e[0], (int)e[1], e[2]);
            r.run(1 + (int)(gen() % n)); // run from an arbitrary root: the answers must not depend on it
            for (int v = 1; v <= n; v++)
                if (r.answer[v] != brute_at<T>(v, n, adj, id, mg, ae, an)) maxFail++;
        }

        // ---- 4. node count, where add_node depends on the node id (odd nodes count double)
        {
            using T = ll;
            function<T()> id = [] { return 0LL; };
            function<T(T, T)> mg = [](T a, T b) { return a + b; };
            function<T(T, int, int, ll)> ae = [](T v, int, int, ll) { return v; };
            function<T(T, int)> an = [](T v, int u) { return v + (u % 2 ? 2 : 1); };
            Rerooting<T> r(n);
            r.identity = id; r.merge = mg; r.add_edge = ae; r.add_node = an;
            for (auto &e : edges) r.add((int)e[0], (int)e[1], e[2]);
            r.run(1);
            for (int v = 1; v <= n; v++)
                if (r.answer[v] != brute_at<T>(v, n, adj, id, mg, ae, an)) cntFail++;
        }
    }

    // ---- shapes that break naive implementations: star (one huge degree) and path
    int shapeFail = 0;
    for (int shape = 0; shape < 2; shape++)
        for (int n = 1; n <= 60; n++) {
            vector<array<ll, 3>> edges;
            vector<vector<pair<int, ll>>> adj(n + 1);
            for (int v = 2; v <= n; v++) {
                int p = shape == 0 ? 1 : v - 1;
                edges.push_back({(ll)p, (ll)v, 1});
                adj[p].push_back({v, 1}), adj[v].push_back({p, 1});
            }
            using T = pair<ll, ll>;
            function<T()> id = [] { return make_pair(0LL, 0LL); };
            function<T(T, T)> mg = [](T a, T b) { return make_pair(a.first + b.first, a.second + b.second); };
            function<T(T, int, int, ll)> ae = [](T v, int, int, ll w) { return make_pair(v.first, v.second + v.first * w); };
            function<T(T, int)> an = [](T v, int) { return make_pair(v.first + 1, v.second); };
            Rerooting<T> r(n);
            r.identity = id; r.merge = mg; r.add_edge = ae; r.add_node = an;
            for (auto &e : edges) r.add((int)e[0], (int)e[1], e[2]);
            r.run(1);
            for (int v = 1; v <= n; v++)
                if (r.answer[v] != brute_at<T>(v, n, adj, id, mg, ae, an)) shapeFail++;
        }

    printf("Rerooting  (%d random trees, every node as root)\n", cases);
    printf("  distance sum, unweighted   : %d failures\n", sumFail);
    printf("  distance sum, weighted     : %d failures\n", wFail);
    printf("  farthest node (max merge)  : %d failures\n", maxFail);
    printf("  node-dependent add_node    : %d failures\n", cntFail);
    printf("  answer[root] == down[root] : %d failures\n", downFail);
    printf("  star and path, n = 1..60   : %d failures\n", shapeFail);
}
