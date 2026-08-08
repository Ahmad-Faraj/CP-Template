# Floyd-Warshall

## Floyd-Warshall

Computes all-pairs shortest paths in $\mathcal{O}(V^3)$.
$$dist[i][j] = \min(dist[i][j], dist[i][k] + dist[k][j])$$
The $k$-loop **must** be the outermost loop.

### Transitive Closure & Bitset Optimization
If we only care about reachability (boolean weights), FW becomes:
$$reach[i][j] = reach[i][j] \lor (reach[i][k] \land reach[k][j])$$
Using `std::bitset`, this can be optimized to $\mathcal{O}(V^3 / 64)$:
```cpp
for (int k = 0; k < V; k++) {
    for (int i = 0; i < V; i++) {
        if (reach[i][k]) reach[i] |= reach[k];
    }
}
```

### Min-Plus Matrix Multiplication
Floyd-Warshall is functionally computing the $V$-th power of the adjacency matrix using the $(\min, +)$ semi-ring.
*   If you need paths of exactly length $L$, you can compute $A^L$ under $(\min, +)$ multiplication in $\mathcal{O}(V^3 \log L)$.
*   Useful for "shortest path with exactly $K$ edges".

### Dynamic Floyd-Warshall
If we add a vertex (and its incident edges) to the graph, we can update the APSP matrix in $\mathcal{O}(V^2)$:
```cpp
// k is the new vertex
for(int i=0; i<V; ++i)
    for(int j=0; j<V; ++j)
        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
```
(Offline trick: To handle deletions, process the queries in reverse time, treating deletions as insertions).

