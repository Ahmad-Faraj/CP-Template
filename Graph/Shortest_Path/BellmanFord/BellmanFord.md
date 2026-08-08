# Bellman-Ford & Applications

## Difference Constraints

Difference Constraints elegantly reduce system of linear inequalities into shortest path problems.
Given inequalities of the form $X_i - X_j \le W$:
1.  Draw a directed edge from $j$ to $i$ with weight $W$.
2.  Add a dummy source $S$ with $0$-weight edges to all nodes (ensuring connectivity).
3.  The shortest distances $dist[i]$ from $S$ satisfy all constraints! $dist[i] - dist[j] \le W$.
4.  If a negative cycle exists, the system has **no solution**.

### Max/Min Variable Constraints
*   **To maximize $X_i - X_j$:** This is exactly the shortest path from $j$ to $i$.
*   $X_i = X_j$ becomes $X_i - X_j \le 0$ and $X_j - X_i \le 0$.
*   Strict inequalities $X_i - X_j < W$: For integers, convert to $X_i - X_j \le W - 1$.
*   If we require $X_i \ge 0$, add an edge $S \to i$ with weight $0$, or constrain against a fixed origin node $X_0 = 0$.

---

## Minimum Mean-Weight Cycle

Given a directed graph, find a cycle $C$ that minimizes:
$$\mu(C) = \frac{\sum_{e \in C} w(e)}{|C|}$$

### Binary Search + Bellman-Ford/SPFA ($\mathcal{O}(VE \log(\text{range})))$
1.  Binary search the answer $\mu$.
2.  Reweight all edges: $w'(u, v) = w(u, v) - \mu$.
3.  If the reweighted graph has a **negative cycle**, then there exists a cycle where $\sum (w - \mu) < 0 \implies \frac{\sum w}{|C|} < \mu$. We decrease the upper bound of our search.
4.  Otherwise, increase the lower bound.
*Detection via DFS-SPFA is extremely fast here.*

### Karp's Algorithm ($\mathcal{O}(VE)$)
1.  Pick an arbitrary source $s$.
2.  Let $d[k][v]$ be the shortest path from $s$ to $v$ of exactly length $k$.
3.  Compute $d[k][v]$ for all $k \in [0, V]$ using dynamic programming.
4.  The minimum mean cycle is given by:
    $$\min_{v \in V} \max_{0 \le k < V} \frac{d[V][v] - d[k][v]}{V - k}$$
(Note: Only applies if the graph is strongly connected, or you add a dummy node $S$ connected to all nodes).

