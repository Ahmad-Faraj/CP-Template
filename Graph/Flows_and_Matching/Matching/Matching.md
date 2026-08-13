# Bipartite Matching and Graph Theory Theorems

### Hopcroft-Karp Algorithm
Computes the maximum bipartite matching in $O(E \sqrt{V})$. It uses BFS to find multiple augmenting paths of the shortest length simultaneously, followed by DFS to push flow. 

### Kőnig's Theorem & Equivalences
In any bipartite graph:
**Maximum Matching = Minimum Vertex Cover (MVC)**
- **Maximum Independent Set (MIS)** = $V$ - MVC.
- **Minimum Edge Cover** = $V$ - Maximum Matching (if no isolated vertices).

*Constructing the MVC from Matching*:
1. Find Max Matching.
2. Run DFS/BFS from all unmatched vertices on the LEFT side, moving via alternating paths (unmatched edges L $\to$ R, matched edges R $\to$ L).
3. MVC consists of: **Unvisited LEFT vertices** + **Visited RIGHT vertices**.

```cpp
// Blueprint: Minimum Vertex Cover from Bipartite Matching
// After running Bipartite Matching (Kuhn's / Hopcroft-Karp):
vector<bool> vis_L(N + 1, false), vis_R(M + 1, false);
auto dfs_mvc = [&](auto& self, int u) -> void {
    vis_L[u] = true;
    for (int v : adj[u]) {
        // Traverse unmatched edges L -> R
        if (!vis_R[v]) { 
            vis_R[v] = true;
            // R -> L must be matched edges
            if (mt[v] != -1 && !vis_L[mt[v]]) {
                self(self, mt[v]); 
            }
        }
    }
};
// Start DFS from all unmatched LEFT nodes
for (int i = 1; i <= N; ++i) {
    if (!is_matched_left[i]) dfs_mvc(dfs_mvc, i);
}
// MVC = {Unvisited L} U {Visited R}
```

### Hall's Marriage Theorem
A bipartite graph with partitions $L$ and $R$ has a matching covering all vertices of $L$ iff for every subset $S \subseteq L$, the neighborhood $N(S)$ satisfies $|N(S)| \ge |S|$.
*Application*: In a $k$-regular bipartite graph ($k \ge 1$), a perfect matching always exists.

### Dilworth's Theorem (DAGs)
In a Directed Acyclic Graph:
**Minimum number of paths to cover all vertices = Maximum size of an anti-chain**.
- *Path Cover Model*: Split each vertex $v$ into $v_{in}$ and $v_{out}$. For edge $u \to v$, add edge $u_{out} \to v_{in}$. Minimum paths = $V - \text{MaxMatching}$.
- *Anti-chain*: A set of vertices where no two are reachable from one another.

