# Bridges, Articulation Points, and Block-Cut Trees

Finding bridges/cut-vertices is standard using the low-link algorithm ($low[v] \ge tin[u]$). The advanced applications in CP revolve around the **Block-Cut Tree (BCT)** or **Bridge Tree**.

## 1. Bridge Tree (2-Edge-Connected Components)
Compress 2-edge-connected components (2-ECC) into single nodes. The resulting graph is a forest (or a tree if the original graph was connected).
- **Trick:** Two nodes are in the same 2-ECC if there is no bridge on any path between them.
- **Application:** Finding if a specific edge is strictly required to reach $V$ from $U$. If the edge is a bridge and lies on the path in the Bridge Tree between the 2-ECC of $U$ and the 2-ECC of $V$, it is strictly required. You can solve this using LCA on the Bridge Tree.

```cpp
// Blueprint: Building the Bridge Tree
vector<vector<int>> bridge_tree(ecc_count + 1);
for (auto& edge : edges) {
    int u = edge.u, v = edge.v;
    int cu = comp[u], cv = comp[v]; // 2-ECC components
    if (cu != cv) {
        bridge_tree[cu].push_back(cv);
        bridge_tree[cv].push_back(cu);
    }
}
// bridge_tree is now a valid Tree/Forest!
```

## 2. Block-Cut Tree (Biconnected Components)
Nodes in the BCT represent either **Blocks (Biconnected Components)** or **Cut Vertices (Articulation Points)**.
- **Construction:** Maintain a stack of edges during the DFS. When an articulation point is found ($low[v] \ge tin[u]$), pop edges from the stack to form a Block.
- **Properties:**
  - **Bipartite tree:** Block nodes only connect to Cut Vertex nodes, and vice versa.
  - **Path queries:** The number of cut vertices on all simple paths between $U$ and $V$ is exactly the number of Cut Vertex nodes on the path between $U$ and $V$ in the Block-Cut Tree. This can be queried using standard tree DP, LCA, or Heavy-Light Decomposition (HLD).

```cpp
// Blueprint: BCT Path Counting (Vertices in any simple path)
// In the BCT, assign weight 1 to Cut-Vertex nodes, and weight 0 to Block nodes.
// The distance between node U and V in the BCT gives the answer!
int cut_vertices_on_path = lca.query_sum(comp[u], comp[v]);
```
