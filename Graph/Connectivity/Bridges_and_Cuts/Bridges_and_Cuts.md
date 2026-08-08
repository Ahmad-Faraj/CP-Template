## Bridges, Articulation Points, and Block-Cut Trees

Finding bridges/cut-vertices is standard ($low[v] \ge tin[u]$). The applications revolve around the **Block-Cut Tree (BCT)** or **Bridge Tree**.

### Bridge Tree (2-Edge-Connected Components)
Compress 2-edge-connected components (2-ECC) into single nodes. The resulting graph is a forest (or tree if the original was connected).
- **Trick:** Two nodes are in the same 2-ECC if there is no bridge on any path between them.
- **Application:** Finding if a specific edge is strictly required to reach $V$ from $U$. If the edge is a bridge and lies on the path in the Bridge Tree between the 2-ECC of $U$ and the 2-ECC of $V$, it is required. Use LCA on the Bridge Tree.

### Block-Cut Tree (Biconnected Components)
Nodes in the BCT represent either **Blocks (Biconnected Components)** or **Cut Vertices (Articulation Points)**.
- **Construction:** Maintain a stack of edges during the DFS. When an articulation point is found ($low[v] \ge tin[u]$), pop edges from the stack to form a Block.
- **Properties:**
  - Bipartite tree: Block nodes only connect to Cut Vertex nodes, and vice versa.
  - Path queries: The number of cut vertices on all simple paths between $U$ and $V$ is exactly the number of Cut Vertex nodes on the path between $U$ and $V$ in the BCT. Use standard tree DP or HLD to query this.

