// 2 SAT: decides whether boolean variables can satisfy clauses of two literals, and produces one assignment.
// Use when: every constraint pairs two things with two states each - "at most one of", "these differ", "if A then B".
// Handles: OR, XOR, implication, equality and forcing a variable. Reports unsatisfiable rather than guessing.
// Time: O(n + m)
// Indexing: 1-based variables 1..n; value[v] is filled by satisfiable()
// Note: satisfiable() is recursive over 2n implication nodes. Call it once - it does not reset between calls.

#include <bits/stdc++.h>
using namespace std;

struct Two_SAT {
    int n, lits;
    vector<vector<int>> g, gt;
    vector<char> vis;
    vector<int> comp, order;
    vector<char> value; // value[v] is variable v's assignment, valid after satisfiable() returns true

    Two_SAT(int n) : n(n), lits(2 * n), g(2 * n), gt(2 * n), vis(2 * n, 0), comp(2 * n, -1), value(n + 1, 0) {}

    int lit(int v, bool want) { return 2 * (v - 1) + (want ? 0 : 1); } // node for "variable v equals want"

    void add_implication(int a, bool af, int b, bool bf) { // a == af forces b == bf
        int x = lit(a, af), y = lit(b, bf);
        g[x].push_back(y);
        gt[y].push_back(x);
        g[y ^ 1].push_back(x ^ 1); // the contrapositive
        gt[x ^ 1].push_back(y ^ 1);
    }

    void add_or(int a, bool af, int b, bool bf) { // (a == af) OR (b == bf)
        add_implication(a, !af, b, bf);
    }

    void add_xor(int a, bool af, int b, bool bf) { // exactly one of the two holds
        add_or(a, af, b, bf);
        add_or(a, !af, b, !bf);
    }

    void force(int a, bool af) { add_or(a, af, a, af); }        // a must equal af
    void add_equal(int a, int b) { add_xor(a, true, b, false); } // a == b
    void add_not_equal(int a, int b) { add_xor(a, true, b, true); }
    void at_most_one_true(int a, int b) { add_or(a, false, b, false); }

    void dfs1(int u) {
        vis[u] = 1;
        for (int v : g[u])
            if (!vis[v]) dfs1(v);
        order.push_back(u);
    }

    void dfs2(int u, int c) {
        comp[u] = c;
        for (int v : gt[u])
            if (comp[v] == -1) dfs2(v, c);
    }

    bool satisfiable() { // false when some variable and its negation land in one component
        for (int i = 0; i < lits; i++)
            if (!vis[i]) dfs1(i);
        int c = 0;
        for (int i = lits - 1; i >= 0; i--)
            if (comp[order[i]] == -1) dfs2(order[i], c++);
        for (int v = 1; v <= n; v++) {
            int t = lit(v, true), f = lit(v, false);
            if (comp[t] == comp[f]) return false;
            value[v] = comp[t] > comp[f]; // later in topological order wins
        }
        return true;
    }
};

// Standard problem: m clauses of the form (literal OR literal); print an assignment or report impossibility
void solve() {
    int n, m;
    cin >> n >> m;
    Two_SAT sat(n);
    for (int i = 0; i < m; i++) { // each clause: sign var sign var, with + meaning true and - meaning false
        char s1, s2;
        int a, b;
        cin >> s1 >> a >> s2 >> b;
        sat.add_or(a, s1 == '+', b, s2 == '+');
    }
    if (!sat.satisfiable()) {
        cout << "IMPOSSIBLE\n";
        return;
    }
    for (int v = 1; v <= n; v++) cout << (sat.value[v] ? '+' : '-') << " \n"[v == n];
}
