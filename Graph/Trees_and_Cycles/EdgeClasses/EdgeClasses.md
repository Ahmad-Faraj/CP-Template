# Edge Classification and Cycle Properties

## DFS Trees
When performing a DFS, edges are classified:
- **Undirected:** Tree edges and Back edges. (No cross or forward edges!). Every back edge $(u, v)$ creates a **fundamental cycle** consisting of the back edge and the tree path between $u$ and $v$.
- **Directed:** Tree, Back, Forward, and Cross edges.

## Biconnected Components (BCC) & Bridges
- **Bridge:** An edge whose removal increases the number of connected components.
- **Articulation Point:** A vertex whose removal increases components.
- **Block-Cut Tree:** Condenses a graph into a tree where nodes represent either a BCC or an articulation point. Extremely powerful for solving paths-in-graph problems.

