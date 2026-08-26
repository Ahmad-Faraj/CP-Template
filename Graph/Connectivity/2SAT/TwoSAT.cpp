#include "../../../core.h"

/*
 * Topic: 2-Satisfiability (2-SAT)
 * Description: Solves boolean satisfiability for clauses of size 2 using SCCs (Kosaraju's).
 *              Finds if there is any assignment of variables such that formula F is true.
 *              Formula F = (x_0 OP y_0) AND (x_1 OP y_1) AND ... (x_{n-1} OP y_{n-1})
 *              where OP belongs to {OR, XOR}.
 *
 * Important Facts:
 * - 0-based indexing for variables.
 * - Time Complexity: O(V + E)
 * - Space Complexity: O(V + E)
 */
struct twosat {
    int n; // total size combining +, -. must be even.
    vector<vector<int>> g, gt;
    vector<bool> vis, res;
    vector<int> comp;
    stack<int> ts;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Allocates space for a 2-SAT problem with the given number of variables.
    twosat(int vars = 0) {
        n = vars << 1;
        g.resize(n);
        gt.resize(n);
    }

    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // Helper function to add directed implication edges.
    void _add(int a, bool af, int b, bool bf) {
        a += a + (af ^ 1);
        b += b + (bf ^ 1);
        g[a].push_back(b);
        gt[b].push_back(a);
    }

    // add this type of condition->
    // add(a,af,b,bf) means if a is af then b must need to be bf
    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // Enforces that if variable a evaluates to af, then variable b must evaluate to bf.
    void add(int a, bool af, int b, bool bf) {
        _add(a, af, b, bf);
        _add(b, !bf, a, !af);
    }

    // Time Complexity: O(V + E) overall
    // Space Complexity: O(V)
    // First DFS pass of Kosaraju's algorithm to determine the finishing order.
    void dfs1(int u) {
        vis[u] = true;
        for (int v : g[u])
            if (!vis[v]) dfs1(v);
        ts.push(u);
    }

    // Time Complexity: O(V + E) overall
    // Space Complexity: O(V)
    // Second DFS pass of Kosaraju's algorithm to identify strongly connected components.
    void dfs2(int u, int c) {
        comp[u] = c;
        for (int v : gt[u])
            if (comp[v] == -1) dfs2(v, c);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // Evaluates satisfiability and constructs a valid variable assignment if possible.
    bool ok() {
        vis.resize(n, false);
        for (int i = 0; i < n; ++i)
            if (!vis[i]) dfs1(i);
        int scc = 0;
        comp.resize(n, -1);
        while (!ts.empty()) {
            int u = ts.top();
            ts.pop();
            if (comp[u] == -1) dfs2(u, scc++);
        }
        res.resize(n / 2);
        for (int i = 0; i < n; i += 2) {
            if (comp[i] == comp[i + 1]) return false;
            res[i / 2] = (comp[i] > comp[i + 1]);
        }
        return true;
    }

    //(x_a or (not x_b))-> af=1,bf=0
    void addOR(int a, bool af, int b, bool bf) {
        a += a + (af ^ 1);
        b += b + (bf ^ 1);
        g[a ^ 1].push_back(b); // !a => b
        g[b ^ 1].push_back(a); // !b => a
        gt[b].push_back(a ^ 1);
        gt[a].push_back(b ^ 1);
    }

    //(!x_a xor !x_b)-> af=0, bf=0
    void addXOR(int a, bool af, int b, bool bf) {
        addOR(a, af, b, bf);
        addOR(a, !af, b, !bf);
    }
};

// Reads input, builds the 2-SAT graph, checks satisfiability, and prints the result.
int main() {
    int n, m;
    cin >> n >> m;
    twosat ts(n);
    for (int i = 0; i < m; i++) {
        int u, v, k;
        cin >> u >> v >> k;
        --u;
        --v;
        if (k)
            ts.add(u, 0, v, 0), ts.add(u, 1, v, 1), ts.add(v, 0, u, 0), ts.add(v, 1, u, 1);
        else
            ts.add(u, 0, v, 1), ts.add(u, 1, v, 0), ts.add(v, 0, u, 1), ts.add(v, 1, u, 0);
    }
    int k = ts.ok();
    if (!k)
        cout << "Impossible\n";
    else {
        vector<int> v;
        for (int i = 0; i < n; i++)
            if (ts.res[i]) v.push_back(i);
        cout << (int)v.size() << '\n';
        for (auto x : v) cout << x + 1 << ' ';
        cout << '\n';
    }
    return 0;
}