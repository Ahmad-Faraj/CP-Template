# Graph Shortest Paths

This document distills shortest path algorithms. It avoids basic textbook proofs and instead focuses on state-space modeling, algorithmic limits, counter-tests, and reductions.

## Dijkstra's Algorithm

At its core, Dijkstra computes shortest paths on graphs with **non-negative edge weights**. Its standard implementation using a binary heap runs in $\mathcal{O}((V+E) \log V)$.

### Optimizations and Variations

*   **Dial's Algorithm:** If edge weights are small integers bounded by $W$, use an array of buckets (or a circular queue of size $W+1$). Complexity: $\mathcal{O}(V + E + d_{max}) = \mathcal{O}(E + VW)$. Useful when $W \le 10^6$.
*   **Radix Heap:** For integer weights, a Radix Heap can achieve $\mathcal{O}(E + V \log W)$. It is practically extremely fast and avoids the $\log V$ overhead of `std::priority_queue`.
*   **Dense Graphs:** If $E = \Theta(V^2)$, an $\mathcal{O}(V^2)$ Dijkstra without a priority queue (just scanning for the minimum distance in an array) is strictly faster due to constant factors and cache locality.

### Multi-State & Layered Graphs
The true power of Dijkstra in CP lies in state-space search. Nodes are no longer just vertices, but tuples $(u, \text{state})$.
*   **Fractional Path Problems:** "Shortest path but you can skip $K$ edges." Duplicate the graph into $K+1$ layers. An edge $u \to v$ with weight $w$ becomes transitions $(u, k) \xrightarrow{w} (v, k)$ and $(u, k) \xrightarrow{0} (v, k+1)$.
*   **Implicit Graphs:** Do not build the graph explicitly if $E$ is huge. Generate edges on the fly inside the `while (!pq.empty())` loop. 
*   **Segment Tree / Data Structure Optimization:** If you have transitions like "add directed edges from vertex $u$ to all vertices in range $[L, R]$ with cost $W$", use a Segment Tree to build a layered graph where nodes represent intervals. Complexity reduces from $\mathcal{O}(V^2)$ to $\mathcal{O}(V \log V)$.

### $K$-Shortest Paths
*   **State Extension:** If $K$ is small (e.g., $K \le 10$), store a vector of $K$ distances for each vertex. Instead of relaxing $dist[v]$, merge the $K$ best paths of $dist[v]$ and $dist[u] + w$.
*   **Eppstein's Algorithm:** Finds the $K$-shortest paths in $\mathcal{O}(E + V \log V + K \log K)$ by building a shortest-path tree to the destination, and maintaining a persistent heap of "detours" (sidetracks).
*   **Yen's Algorithm:** $\mathcal{O}(K V(E + V \log V))$ using Lawler's modification. Often too slow for competitive programming unless $K$ is tiny.

### Dijkstra with Potentials (Johnson's Idea / A*)
If edges can be negative (but no negative cycles exist), and we know a potential function $h(v)$ satisfying the triangle inequality $h(u) - h(v) \le w(u, v)$, we can reweight edges:
$$w'(u, v) = w(u, v) + h(u) - h(v) \ge 0$$
Now Dijkstra can be run using $w'$. 
*   **A* Search:** In implicit graphs (like 15-puzzle), if $h(v)$ is a consistent heuristic estimating the distance from $v$ to $T$, A* is exactly Dijkstra on the reweighted graph $w'(u,v) = w(u,v) + h(u) - h(v)$.
*   **Min-Cost Max-Flow (MCMF):** Successive shortest path algorithms use Bellman-Ford/SPFA once to find initial potentials, and then run Dijkstra for all subsequent flow augmentations.

---

## SPFA (Shortest Path Faster Algorithm) & Bellman-Ford

Bellman-Ford solves the single-source shortest path problem with negative weights in $\mathcal{O}(VE)$. SPFA is Bellman-Ford implemented with a queue, keeping track of which vertices have been relaxed and only expanding them.

### The "Anti-SPFA" Meta
In the worst case, SPFA is $\mathcal{O}(VE)$. Problem setters *will* generate grid graphs or layered graphs designed to trigger exponential-like relaxation cascades. **Never use SPFA if negative edges are not explicitly required, unless the graph is guaranteed to be random.**

### Heuristics (SLF and LLL)
If you *must* use SPFA (e.g., Min-Cost Max-Flow where potentials aren't cleanly maintained):
*   **SLF (Small Label First):** When pushing $v$ to the queue, if $dist[v] < dist[\text{queue.front()}]$, push to the front (using a `std::deque`), else push to the back.
*   **LLL (Large Label Last):** Keep track of the average distance in the queue. Before popping, if $dist[\text{front}] > \text{avg}$, push it to the back.
*   **Warning:** These heuristics can *still* be defeated by maliciously constructed graphs (e.g., by creating chains of small negative weights that trick SLF).

### Negative Cycle Detection
*   **Standard BF:** If distances update on the $V$-th iteration, a negative cycle exists.
*   **SPFA (Queue Count):** If a vertex enters the queue $V$ times, a negative cycle exists.
*   **SPFA (Subtree Count):** Maintain the shortest-path tree structure. If the depth of a node in the tree exceeds $V$, a negative cycle exists. This is practically the fastest method.
*   **DFS-SPFA:** Maintain a boolean array of nodes currently in the recursion stack. If we relax an edge to a node already in the stack, we found a negative cycle. Extremely fast for graphs with many small negative cycles.

```cpp
// Blueprint: Negative Cycle Recovery (Bellman-Ford / SPFA)
// `parent` array tracks the relaxed edges. `last_updated_node` is the node updated on the V-th iteration.
int y = last_updated_node;
for (int i = 0; i < V; ++i) {
    y = parent[y]; // walk back V times to guarantee we are inside the cycle
}
vector<int> path;
for (int cur = y; ; cur = parent[cur]) {
    path.push_back(cur);
    if (cur == y && path.size() > 1) break;
}
reverse(path.begin(), path.end());
// `path` now contains the negative cycle!
```

