# Euler Tours and Tree Flattening

An Euler tour on a tree flattens the tree structure into a 1D array, allowing us to leverage powerful array-based data structures (Segment Trees, Fenwick Trees, Mo's Algorithm).

## 1.1 The Standard Euler Tour (Subtree Queries)
The most basic form records the `in` and `out` times of each node during a DFS.
- **Property:** The subtree of node $u$ corresponds to the contiguous subarray `[in[u], out[u]]`.
- **Applications:** Subtree sum/update, distinct values in a subtree (using Mo's on trees or persistent segment trees).

## 1.2 Path Queries (The 2N/3N Tour)
To query paths, we record nodes when we enter them and when we return to them.
Alternatively, to handle vertex weights on paths using a Fenwick tree:
- Add weight $W(u)$ at `in[u]`.
- Subtract weight $W(u)$ at `out[u]`.
Prefix sum up to `in[u]` gives the path sum from root to $u$. Path from $u$ to $v$ is `prefix(u) + prefix(v) - 2 * prefix(LCA) + W(LCA)`.
