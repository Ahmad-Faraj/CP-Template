# Strongly Connected Components (SCC)

## 1. Graph Condensation (DAG Construction)
While Kosaraju's and Tarjan's algorithms are well-known, the real CP challenge lies in what you do *after* finding Strongly Connected Components (SCCs): **Graph Condensation**.
Compressing a directed graph into a Directed Acyclic Graph (DAG) of its SCCs allows for Dynamic Programming (DP) and efficient reachability queries.

**Heuristics & Pitfalls:**
- **Duplicate Edges:** When building the condensed graph, you will often generate multiple edges between the same pair of SCCs. Use a `std::set`, or better yet, a sorted `std::vector` with `std::unique` to remove duplicates, as graph traversal with multi-edges can cause TLE in dense condensed graphs.
- **Self-Loops:** Ignore edges where `comp[u] == comp[v]`.
- **Topological Order:** Tarjan’s algorithm inherently discovers SCCs in **reverse topological order**. You do *not* need an extra topological sort step on the condensed graph! Iterating from $C-1$ down to $0$ (where $C$ is the number of SCCs) gives a valid topological processing order.

```cpp
// Blueprint: Building the Condensed DAG from SCCs safely
vector<vector<int>> dag(scc_count + 1);
set<pair<int, int>> used_edges;

for (int u = 1; u <= N; u++) {
    for (int v : adj[u]) {
        int cu = comp[u], cv = comp[v];
        if (cu != cv && !used_edges.count({cu, cv})) {
            dag[cu].push_back(cv);
            used_edges.insert({cu, cv});
        }
    }
}
```

## 2. Reachability (Bitsets)
For dense DAGs, querying reachability between pairs of components can be optimized using `std::bitset`.
- **Memory Limit Issues:** A matrix of size $N \times N$ might MLE for $N = 10^5$. 
- **Chunking:** Process reachability in chunks of 32,000 to 64,000 vertices. Keep a `bitset<B>` for the active chunk and sweep the DAG in reverse topological order: `reach[u] |= reach[v]`. This solves $\mathcal{O}(N^2/64)$ reachability under tight memory constraints.

## 3. Kosaraju vs Tarjan
- **Kosaraju:** Requires two DFS passes. More cache misses, slightly slower constant factor. Easier to implement if you need topological order explicitly without thinking.
- **Tarjan:** One DFS pass. Less overhead, inherently provides reverse topological order. Better for tight time limits.
- **Recursion Limit:** In Python or Java, deep recursion can crash. In C++, deep recursion is usually fine but can stack overflow on some platforms. Iterative Tarjan using an explicit `std::vector` as a stack (simulating the call stack) is a rare but necessary trick for extreme constraints.
