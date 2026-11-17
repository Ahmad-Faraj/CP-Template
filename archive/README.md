# archive/

Files taken out of the library on 2026-11-16. They are not printed and not used; the full history stays in git.
They will be deleted after ACPC 2026 unless someone wants something from them.

| File | Was in | Why |
|---|---|---|
| Slope_Trick.cpp | 05_DP | Empty (`//TODO`) |
| FWHT.cpp | 07_Math | Empty (0 lines) |
| Discrete_Log.cpp | 07_Math | Includes only, no code |
| Treap_Implicit.cpp | 01_Data_Structures | Exact copy of Treap.cpp; there was no implicit treap |
| Dijkstra_2.cpp | 04_Graphs | Duplicate of Dijkstra (whole solution file) |
| Geometry_3.cpp | 09_Geometry | Duplicate geometry library (whole solution file) |
| Geometry_4.cpp | 09_Geometry | Duplicate; treats a segment as a ray |
| Points_Lines_Segments.cpp | 09_Geometry | Duplicate; divides by zero on vertical lines |
| Geometry_Complex_Extra.cpp | 09_Geometry | 29-line fragment of Geometry_Complex |
| Geometry_Complex.cpp | 09_Geometry | Does not compile; wrong min enclosing circle, Pick's theorem, centroid; Voronoi returns empty |
| Convex_Hull_3.cpp | 09_Geometry | `int` overflow; three other hulls remain |
| Hashing_Segment_Tree.cpp | 06_Strings | Leaf weights use tree-node index, so every range hash is wrong |
| Segment_Tree_Hashing.cpp | 02_Range_Query | `int` × `int` overflow, so every hash is wrong |
| FFT_Mod.cpp | 07_Math | Does not compile (two full copies of everything) |
| Combinatorics_Basic.cpp | 07_Math | Does not compile; Combinatorics.cpp covers it |
| Catalan.cpp | 07_Math | Does not compile |
| Aho_Corasick_With_Delete.cpp | 06_Strings | "Delete" only frees memory; it cannot remove a pattern |
| XOR_Basis_Range_Offline.cpp | 07_Math | Does not compile (`swap(int, vector)`); two queries ignore `l` |

Also renamed: `04_Graphs/Bipartite_Matching.cpp` → `04_Graphs/Bipartite_Check.cpp` (it checks bipartiteness; it is not a matching).

## replaced/ (2026-11-17)

Main's old versions of templates that were replaced by a verified version. Each replacement passed an
independent brute-force test calling every public method, in a `-O2` build and an ASan/UBSan build
(`tests/`, run with `tools/run_tests.sh`).

| Template | Replaced by | Why main's version was replaced |
|---|---|---|
| Fenwick_Tree | normalize branch version | Everything private; constructor never built the tree (every sum wrong) |
| Segment_Tree_Iterative | normalize version + KACTL query loop | Private constructor; query combined the right side in the wrong order (wrong for non-commutative merges; the normalize copy had the same bug) |
| Sparse_Table | normalize version | Was a script, not a reusable struct |
| Coordinate_Compression | normalize version | Did not compile. Ranks are now 0-based |
| Dijkstra | normalize version | Adjacency sized by edge count (out of bounds); with T=int nothing relaxed |
| Bellman_Ford | normalize version | INF became 0 with T=int; early-exit flag never reset |
| Floyd_Warshall | normalize version + KACTL -INF clamp | Did not compile (INF undefined); the normalize copy overflowed on negative cycles |
| MST_Kruskal | normalize version | Did not compile (sorted a const parameter) |
| DSU | main's own file + includes | Kept main's logic; only the missing includes were added |
| SCC_Tarjan | normalize version | No includes; is_ancestor used low-links (wrong). Gains the condensation DAG |
| Bridges | normalize version | Treated a parallel edge as a bridge. Gains 2-edge-connected components and the bridge tree |
| Articulation_Points | main's own file + includes | Kept main's logic (the normalize copy was wrong for isolated vertices) |
| LCA | normalize version | No includes; read input itself |
| Sieve | normalize version | `int i*i` overflow past 46341 (out of bounds) |
| CRT | normalize version | Used the wrong modulus; wrong whenever moduli differ |
| XOR_Basis, XOR_Basis_2 | normalize version + features of both | k-th smallest was wrong. The new file keeps every feature of both old ones |
