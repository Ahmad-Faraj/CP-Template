# Guide to Minimum Spanning Trees (MST)

Minimum Spanning Tree (MST) problems appear frequently in competitive programming. This guide covers classical approaches, structures, and specific patterns needed to master MST problems.

---

## Classical Algorithms: Kruskal and Prim

While basic, knowing *when* to use which algorithm is critical.

### Kruskal's Algorithm
- **Complexity:** $\mathcal{O}(E \log V)$
- **Core Mechanism:** Sort all edges by weight, iterate and add edges connecting different components using a Disjoint Set Union (DSU).
- **When to use:** Sparse graphs, or when you are given a list of edges and want to process them offline. Often combined with DSU with Rollback for dynamic edge problems.

### Prim's Algorithm
- **Complexity:** $\mathcal{O}((V+E) \log V)$ using PQ, or $\mathcal{O}(V^2)$ for dense graphs without PQ.
- **Core Mechanism:** Grow the tree from a single vertex by picking the smallest edge connecting the tree to a non-tree vertex.
- **When to use:** Dense graphs where $E \approx V^2$. The $\mathcal{O}(V^2)$ implementation is faster than Kruskal for dense graphs. It's also useful when edge weights are computed on the fly.

---

## Borůvka's Algorithm

Borůvka's is often the *only* viable algorithm for certain classes of problems, specifically when $E$ is $\mathcal{O}(V^2)$ but edges are generated implicitly.

### Core Idea
1. Start with $V$ isolated components.
2. In each step, for every component, find the minimum weight edge leaving it.
3. Add these edges to the MST and merge the components.
4. Repeat. Since the number of components halves each step, there are at most $\mathcal{O}(\log V)$ steps.

### Pattern: Borůvka on Dense Graphs
If edge weights are defined by a mathematical formula (e.g., $w(u,v) = f(u,v)$) on a complete graph, Borůvka reduces the problem to: *For a given set of vertices, find the nearest neighbor outside the set*. This is much easier to optimize using data structures than finding all $V^2$ edges.

### Pattern: Borůvka + Trie (XOR MST)
**Problem:** Given an array $A$, the edge weight between $i$ and $j$ is $A[i] \oplus A[j]$. Find the MST.
- **Solution:** Insert all $A[i]$ into a Binary Trie. Borůvka's steps align perfectly with the Trie's structure. For a node in the Trie, its left subtree (bit 0) and right subtree (bit 1) form two components that must be connected by the minimum XOR edge between them. This can be found by taking elements from the smaller subtree and querying the larger subtree, yielding an $\mathcal{O}(N \log^2(\max A))$ solution.

---

## Manhattan MST

**Problem:** Given $N$ points in 2D, the edge weight is $|x_1 - x_2| + |y_1 - y_2|$. Find the MST in $\mathcal{O}(N \log N)$.
- **Solution:** A complete graph has $\mathcal{O}(N^2)$ edges, which is too slow. 
- **Trick:** Divide the plane around each point into 8 octants. In each octant, a point only needs to connect to its nearest neighbor. This reduces the number of edges from $N^2$ to $8N$.
- **Implementation:** Sweep-line algorithm. Transform the coordinates to align octants with the axes, sort points, and use a Fenwick tree (Binary Indexed Tree) to query the point with the maximum $x+y$ that satisfies the octant constraints. Generate the $\mathcal{O}(N)$ candidate edges and run Kruskal's.

---

## Minimum Bottleneck Spanning Tree (MBST)

**Definition:** A spanning tree that minimizes the *maximum* edge weight in the tree.
- **Key Property:** *Every MST is an MBST.* (The converse is not true).
- **Application:** If a problem asks to minimize the maximum edge on a path between all pairs of nodes, finding the MST solves it. If you only need to connect $k$ specific nodes, Kruskal's will connect them at the exact moment the edge weight reaches the bottleneck.

---

## Strict Second Minimum Spanning Tree

**Problem:** Find a spanning tree with the smallest weight strictly greater than the MST.
- **Solution:** 
  1. Find the standard MST using Kruskal.
  2. For every non-tree edge $e = (u, v)$ with weight $w$, adding it to the MST creates a cycle.
  3. We must remove an edge on the tree path between $u$ and $v$. To minimize the new tree's weight, we should remove the heaviest edge on this path.
  4. To ensure the new tree is *strictly* greater, the removed edge must have weight strictly less than $w$.
  5. Use **Binary Lifting (LCA)** or **Heavy-Light Decomposition (HLD)** to find the maximum and *strict second maximum* edge weights on the path between $u$ and $v$ in $\mathcal{O}(\log V)$ time.
  6. Swap $e$ with the best valid edge found, and track the minimum overall increase.

---

## Dynamic Connectivity and MST on Trees

When edges are added and removed over time, maintaining the MST requires techniques.

### Offline Dynamic Connectivity (Divide & Conquer on Time)
- If all queries are known offline, use a Segment Tree over the time domain. 
- Edges are inserted into $\mathcal{O}(\log T)$ nodes in the segment tree. 
- Traverse the segment tree with a **DSU with Rollback** (no path compression, only union by rank/size). When reaching a leaf, answer the query.

### Link-Cut Trees (LCT)
- For online dynamic MST (or tracking the path max), LCT is required.
- Maintain the MST as a forest. When an edge $(u, v)$ with weight $w$ is added:
  - If $u$ and $v$ are disconnected, `link(u, v)`.
  - If they are connected, query the maximum edge $e_{max}$ on the path between $u$ and $v$. If $w < weight(e_{max})$, `cut(e_{max})` and `link(u, v)`.
- Edges are typically represented as nodes in the LCT to handle edge weights efficiently.

---

## CP Problem Patterns & Tricks

1. **"Add edges with weight = GCD/LCM" / Divisor Multiples:**
   Instead of iterating pairs, iterate over multiples of $d$ (for $d=1 \dots \max A$). Connect the smallest element divisible by $d$ to all other elements divisible by $d$. The number of generated edges is $O(N \log N)$ (Harmonic Series), allowing standard Kruskal.

2. **Kruskal Reconstruction Tree (Reachability Tree):**
   When running Kruskal, instead of merging sets, create a new parent node for the two sets with the weight of the edge. This creates a rooted binary tree where the leaves are the original vertices. The maximum edge on the path between $u$ and $v$ is simply the weight of their Lowest Common Ancestor (LCA) in this tree. Incredibly useful for reachability queries constrained by edge weights.

3. **Multi-source / Super-node MST:**
   If a problem gives you a cost to "build a power plant" at any node, and costs to lay cables between nodes, add a dummy super-node $0$. Connect it to each node $i$ with weight equal to the power plant cost. The MST of this new graph gives the optimal answer.

4. **1D Sweep-line MST:**
   If nodes are on a 1D line and you can connect segments, use a DSU where each set knows its maximum (rightmost) element. When connecting an interval, jump through the DSU components to avoid processing already connected components, resulting in an amortized $\mathcal{O}(N \alpha(N))$ time complexity.

