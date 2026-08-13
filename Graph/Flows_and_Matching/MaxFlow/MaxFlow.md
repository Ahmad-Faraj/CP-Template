# Max Flow

## Max-Flow Min-Cut Theorem & Core Algorithms

The Max-Flow Min-Cut theorem is the cornerstone of flow theory: the maximum amount of flow passing from the source $S$ to the sink $T$ is equal to the total weight of the edges in the minimum cut that separates $S$ and $T$.

### Dinic's Algorithm
Dinic's algorithm operates on the residual graph by constructing level graphs using BFS and finding blocking flows using DFS.
- **Complexity**: $O(V^2 E)$ in general graphs.
- **Unit Networks**: $O(E \min(V^{2/3}, \sqrt{E}))$.
- **Bipartite Matching**: $O(E \sqrt{V})$.
*Implementation Detail*: Always maintain a `head` or `work` array in the DFS to avoid re-exploring dead ends. This is crucial for achieving the theoretical time complexity; without it, Dinic degrades to Edmonds-Karp $O(V E^2)$ or worse.

### Max Flow Algorithms Comparison
When choosing a Max Flow algorithm, understanding the exact bottlenecks and practical performance on modern architectures is critical.
- **Edmonds-Karp ($O(V E^2)$)**:
  - **Mechanics**: Uses standard BFS to find the shortest augmenting path.
  - **Pros/Cons**: Very simple to implement, extremely lightweight constant factor.
  - **Use Case**: Excellent for very sparse networks or where $V, E$ are extremely small (e.g., $V \le 200, E \le 1000$). Practically overshadowed by Dinic in all CP settings unless you need a quick typed solution for a trivial bounds problem.
- **Dinic's Algorithm ($O(V^2 E)$)**:
  - **Mechanics**: Constructs a level graph using BFS and finds blocking flows via DFS with a dead-end pointer (`head`/`work` array).
  - **Pros/Cons**: Supreme in bipartite matching ($O(E \sqrt{V})$) and unit network flow ($O(E \min(V^{2/3}, \sqrt{E}))$). Very fast on sparse-to-medium dense graphs. The constant factor is incredibly low in competitive programming.
  - **Use Case**: Your default choice for 99% of max flow problems.
- **Push-Relabel (FIFO/HLPP) ($O(V^3)$ or $O(V^2 \sqrt{E})$)**:
  - **Mechanics**: Abandons the augmenting path approach. Maintains a pre-flow and pushes excess to lower-height neighbors. The gap heuristic is absolutely mandatory to achieve good practical time.
  - **Pros/Cons**: Theoretical $O(V^3)$ (FIFO) or $O(V^2 \sqrt{E})$ (HLPP). Unbeatable on extremely dense graphs with large capacities (e.g., $E \approx V^2$ and $V \ge 500$), where Dinic's level graph reconstruction becomes an $O(V^2)$ bottleneck per phase.
  - **Use Case**: Use on worst-case dense flow networks designed to break Dinic.

## Max-Flow Min-Cut Applications

Often, problems ask to minimize a cost or maximize a profit subject to binary decisions and penalties. These are classic Min-Cut models.

### Project Selection (Maximum Weight Closure Problem)
You have a set of projects, each yielding a profit $p_i$ (which can be negative, i.e., a cost), and dependencies: project $u$ requires project $v$.
**Goal**: Select a subset of projects to maximize profit while satisfying dependencies.

**Model**:
1. Add a source $S$ and sink $T$.
2. For each project $i$ with $p_i > 0$, add an edge $S \to i$ with capacity $p_i$. Add $p_i$ to `Total_Positive_Profit`.
3. For each project $i$ with $p_i < 0$, add an edge $i \to T$ with capacity $|p_i|$.
4. For each dependency $u \to v$ (meaning $u$ needs $v$), add an edge $u \to v$ with capacity $\infty$.

**Answer**: `Total_Positive_Profit` - `Max_Flow(S, T)`.
*Why?* A cut partitions the graph into $S$-side (selected) and $T$-side (rejected). Cutting $S \to i$ means we abandon profit $p_i$. Cutting $i \to T$ means we pay cost $|p_i|$. The $\infty$ edges prevent selecting $u$ without $v$.

```cpp
// Blueprint: Recovering Min-Cut Edges (after running Dinic's Max Flow)
vector<bool> reachable(N + 1, false);
queue<int> q;
q.push(source);
reachable[source] = true;

while (!q.empty()) {
    int u = q.front(); q.pop();
    for (auto& edge : dinic.g[u]) {
        if (edge.w - edge.flow > 0 && !reachable[edge.to]) {
            reachable[edge.to] = true;
            q.push(edge.to);
        }
    }
}
// Any edge connecting a reachable node to an unreachable node is in the Min Cut!
```

### Minimum Cut for Binary Assignments (Penalty Model)
Given variables $x_i \in \{0, 1\}$. 
- Assigning $x_i = 0$ costs $A_i$.
- Assigning $x_i = 1$ costs $B_i$.
- If $x_i = 1$ and $x_j = 0$, you pay a penalty $C_{i,j}$.

**Model**:
- $x_i = 0 \iff i \in S$-cut. $x_i = 1 \iff i \in T$-cut.
- Edge $i \to T$ with capacity $A_i$.
- Edge $S \to i$ with capacity $B_i$.
- Edge $i \to j$ with capacity $C_{i,j}$.
Minimum cost = `Max_Flow(S, T)`.

## Flows with Demands and Bounds (LR Flows)

Sometimes edges have a lower bound $L_e$ and upper bound $R_e$ on the flow.

### Feasible Circulation with Demands
Given a graph with demands $d_v$ for each vertex (where $\sum d_v = 0$, $d_v > 0$ means it needs flow, $d_v < 0$ means it supplies flow), and edges with capacities $C_e$. 
To find if a circulation exists:
1. Add a super-source $SS$ and super-sink $TT$.
2. For each $v$ with $d_v > 0$, add $SS \to v$ with cap $d_v$.
3. For each $v$ with $d_v < 0$, add $v \to TT$ with cap $-d_v$.
4. Check if Max Flow from $SS$ to $TT$ equals $\sum_{d_v > 0} d_v$.

### Feasible Flow with Lower/Upper Bounds
For each edge $u \xrightarrow{[L, R]} v$:
1. Send $L$ flow directly. The residual capacity becomes $R - L$.
2. This creates a deficit of $L$ at $v$ (it gained $L$) and a surplus at $u$ (it lost $L$).
3. Update demands: $d_v += L$, $d_u -= L$.
4. Solve the feasible circulation problem on the updated demands using the residual capacities $R-L$.

### Max/Min Flow with Lower Bounds from $S$ to $T$
To find the Max Flow from $S$ to $T$ with bounds:
1. Add a $\infty$-capacity edge from $T$ to $S$. This turns it into a circulation problem.
2. Solve the feasible circulation with bounds as above using $SS$ and $TT$.
3. If feasible, the current flow is the flow on the $T \to S$ edge.
4. Remove the $T \to S$ edge and its flow.
5. Run Max Flow from $S$ to $T$ in the residual graph and add it to the previous flow.
(For Min Flow, run Max Flow from $T$ to $S$ in the residual graph and subtract).

## Modeling Tricks & Paradigms

1. **Vertex Capacities**: Split a vertex $v$ into $v_{in} \to v_{out}$. Set the capacity of this edge to the vertex's capacity. All incoming edges go to $v_{in}$, outgoing from $v_{out}$.
2. **Grid / Checkerboard**: In a 2D grid where edges only connect adjacent cells, the graph is bipartite. Color cells like a chessboard. $S \to \text{White}$, $\text{Black} \to T$.
3. **Time-Expanded Networks**: When travel time or scheduling is involved, duplicate vertices for each time step $t$. E.g., $v_t \to v_{t+1}$ with capacity $\infty$ (waiting), and $u_t \to v_{t+1}$ with capacity 1 (moving).

