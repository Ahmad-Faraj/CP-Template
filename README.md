# CP-Template

Contest template library. One folder per area; the numbers fix the printed order.

Every file will get a short header (what it does, use when, time, indexing) in phase 2, and this index will then be
generated from the `Use when` lines, so a template can be found by what the problem says, not by its topic name.

✓ = verified by a brute-force test in `tests/` that calls every public method (run `tools/run_tests.sh`).

Other material: `notes/` (cheat sheets, formulas) and `other/` (teammates' and coaches' templates, kept as they are).

## 01 · Data Structures
[Binary_Trie](01_Data_Structures/Binary_Trie.cpp) ✓ · [Cartesian_Tree](01_Data_Structures/Cartesian_Tree.cpp) ✓ · [DSU](01_Data_Structures/DSU.cpp) ✓ · [DSU_Rollback](01_Data_Structures/DSU_Rollback.cpp) · [MEX](01_Data_Structures/MEX.cpp) ✓ · [MEX_All_Subarrays](01_Data_Structures/MEX_All_Subarrays.cpp) ✓ · [MEX_Range_Query](01_Data_Structures/MEX_Range_Query.cpp) ✓ · [MEX_With_Updates](01_Data_Structures/MEX_With_Updates.cpp) · [Monotonic_Queue](01_Data_Structures/Monotonic_Queue.cpp) · [Monotonic_Stack](01_Data_Structures/Monotonic_Stack.cpp) ✓ · [Ordered_Set](01_Data_Structures/Ordered_Set.cpp) ✓ · [Treap](01_Data_Structures/Treap.cpp) ✓

## 02 · Range Query
[Fenwick_Tree](02_Range_Query/Fenwick_Tree.cpp) ✓ · [Merge_Sort_Tree](02_Range_Query/Merge_Sort_Tree.cpp) · [MO](02_Range_Query/MO.cpp) · [MO_On_Tree](02_Range_Query/MO_On_Tree.cpp) ✓ · [Segment_Tree](02_Range_Query/Segment_Tree.cpp) · [Segment_Tree_2D](02_Range_Query/Segment_Tree_2D.cpp) · [Segment_Tree_Beats](02_Range_Query/Segment_Tree_Beats.cpp) ✓ · [Segment_Tree_Dynamic](02_Range_Query/Segment_Tree_Dynamic.cpp) · [Segment_Tree_Dynamic_2D](02_Range_Query/Segment_Tree_Dynamic_2D.cpp) ✓ · [Segment_Tree_Iterative](02_Range_Query/Segment_Tree_Iterative.cpp) ✓ · [Segment_Tree_Lazy](02_Range_Query/Segment_Tree_Lazy.cpp) · [Segment_Tree_Lazy_Iterative](02_Range_Query/Segment_Tree_Lazy_Iterative.cpp) · [Segment_Tree_Lazy_Persistent](02_Range_Query/Segment_Tree_Lazy_Persistent.cpp) · [Segment_Tree_Persistent](02_Range_Query/Segment_Tree_Persistent.cpp) · [Sparse_Table](02_Range_Query/Sparse_Table.cpp) ✓ · [Sparse_Table_2D](02_Range_Query/Sparse_Table_2D.cpp) ✓ · [SQRT_Decomposition](02_Range_Query/SQRT_Decomposition.cpp) ✓ · [Wavelet_Tree](02_Range_Query/Wavelet_Tree.cpp) ✓

## 03 · Trees
[Centroid_Decomposition](03_Trees/Centroid_Decomposition.cpp) · [Diameter](03_Trees/Diameter.cpp) ✓ · [DSU_On_Tree](03_Trees/DSU_On_Tree.cpp) · [DSU_On_Tree_2](03_Trees/DSU_On_Tree_2.cpp) ✓ · [Euler_Tour](03_Trees/Euler_Tour.cpp) ✓ · [HLD](03_Trees/HLD.cpp) · [LCA](03_Trees/LCA.cpp) ✓ · [Link_Cut_Tree](03_Trees/Link_Cut_Tree.cpp) ✓ · [Tree_Isomorphism](03_Trees/Tree_Isomorphism.cpp)

## 04 · Graphs
[2_SAT](04_Graphs/2_SAT.cpp) ✓ · [Articulation_Points](04_Graphs/Articulation_Points.cpp) ✓ · [Bellman_Ford](04_Graphs/Bellman_Ford.cpp) ✓ · [Bipartite_Check](04_Graphs/Bipartite_Check.cpp) · [Bridges](04_Graphs/Bridges.cpp) ✓ · [DFS](04_Graphs/DFS.cpp) · [Dijkstra](04_Graphs/Dijkstra.cpp) ✓ · [Dynamic_Connectivity](04_Graphs/Dynamic_Connectivity.cpp) ✓ · [Flow_Cheatsheet](04_Graphs/Flow_Cheatsheet.md) · [Floyd_Warshall](04_Graphs/Floyd_Warshall.cpp) ✓ · [Hopcroft_Karp](04_Graphs/Hopcroft_Karp.cpp) ✓ · [Hungarian](04_Graphs/Hungarian.cpp) · [Max_Flow_Dinic](04_Graphs/Max_Flow_Dinic.cpp) · [Min_Cost_Max_Flow](04_Graphs/Min_Cost_Max_Flow.cpp) · [MST_Kruskal](04_Graphs/MST_Kruskal.cpp) ✓ · [Online_Bridges](04_Graphs/Online_Bridges.cpp) · [SCC_Tarjan](04_Graphs/SCC_Tarjan.cpp) ✓ · [SPFA](04_Graphs/SPFA.cpp) ✓ · [Strong_Orientation](04_Graphs/Strong_Orientation.cpp) ✓

## 05 · DP
[Bitmask_DP_Elevator_Rides](05_DP/Bitmask_DP_Elevator_Rides.cpp) ✓ · [CHT](05_DP/CHT.cpp) ✓ · [Deque_Trick](05_DP/Deque_Trick.cpp) ✓ · [Digit_DP](05_DP/Digit_DP.cpp) ✓ · [DnC_Optimization](05_DP/DnC_Optimization.cpp) ✓ · [Knuth_Optimization](05_DP/Knuth_Optimization.cpp) ✓ · [Knuth_Optimization_Stick_Cutting](05_DP/Knuth_Optimization_Stick_Cutting.cpp) ✓ · [Li_Chao_Tree](05_DP/Li_Chao_Tree.cpp) ✓ · [SOS](05_DP/SOS.cpp) ✓ · [Weighted_Job_Scheduling](05_DP/Weighted_Job_Scheduling.cpp) ✓

## 06 · Strings
[Aho_Corasick](06_Strings/Aho_Corasick.cpp) ✓ · [Aho_Corasick_Dynamic](06_Strings/Aho_Corasick_Dynamic.cpp) ✓ · [Aho_Corasick_Intervals](06_Strings/Aho_Corasick_Intervals.cpp) · [Booth](06_Strings/Booth.cpp) · [Hashing](06_Strings/Hashing.cpp) · [Hashing_2D](06_Strings/Hashing_2D.cpp) ✓ · [Hashing_Dynamic](06_Strings/Hashing_Dynamic.cpp) ✓ · [Hashing_Lazy_Segment_Tree](06_Strings/Hashing_Lazy_Segment_Tree.cpp) ✓ · [KMP](06_Strings/KMP.cpp) · [KMP_2](06_Strings/KMP_2.cpp) · [Manacher](06_Strings/Manacher.cpp) · [Palindromic_Tree](06_Strings/Palindromic_Tree.cpp) ✓ · [Suffix_Array](06_Strings/Suffix_Array.cpp) ✓ · [Suffix_Array_Integers](06_Strings/Suffix_Array_Integers.cpp) ✓ · [Trie](06_Strings/Trie.cpp) ✓ · [XOR_Hashing](06_Strings/XOR_Hashing.cpp) ✓ · [Z_Function](06_Strings/Z_Function.cpp) ✓

## 07 · Math
[Combinatorics](07_Math/Combinatorics.cpp) · [CRT](07_Math/CRT.cpp) ✓ · [Differentiation](07_Math/Differentiation.cpp) ✓ · [Divisors_And_Factors](07_Math/Divisors_And_Factors.cpp) ✓ · [Euler_Phi](07_Math/Euler_Phi.cpp) · [Exponentiation](07_Math/Exponentiation.cpp) · [Extended_GCD](07_Math/Extended_GCD.cpp) · [FFT](07_Math/FFT.cpp) ✓ · [Fibonacci](07_Math/Fibonacci.cpp) · [Gaussian_Elimination](07_Math/Gaussian_Elimination.cpp) · [Integration](07_Math/Integration.cpp) ✓ · [LDE](07_Math/LDE.cpp) · [LDE_All_Solutions](07_Math/LDE_All_Solutions.cpp) · [LDE_N_Vars](07_Math/LDE_N_Vars.cpp) ✓ · [LDE_N_Vars_Non_Negative](07_Math/LDE_N_Vars_Non_Negative.cpp) ✓ · [Mod_Arithmetic](07_Math/Mod_Arithmetic.cpp) · [NTT](07_Math/NTT.cpp) ✓ · [NTT_Any_Mod](07_Math/NTT_Any_Mod.cpp) ✓ · [NTT_Large](07_Math/NTT_Large.cpp) ✓ · [Sieve](07_Math/Sieve.cpp) ✓ · [XOR_Basis](07_Math/XOR_Basis.cpp) ✓ · [XOR_Basis_Range](07_Math/XOR_Basis_Range.cpp) ✓

## 08 · Game Theory
[Games_On_Graph](08_Game_Theory/Games_On_Graph.cpp) · [Hackenbush](08_Game_Theory/Hackenbush.cpp) ✓ · [Impartial_Games](08_Game_Theory/Impartial_Games.cpp) · [Remoteness_Suspense](08_Game_Theory/Remoteness_Suspense.cpp) ✓ · [Super_Win_Lose](08_Game_Theory/Super_Win_Lose.cpp) ✓ · [Wythoff](08_Game_Theory/Wythoff.cpp) ✓

## 09 · Geometry
[Convex_Hull](09_Geometry/Convex_Hull.cpp) · [Convex_Hull_2](09_Geometry/Convex_Hull_2.cpp) ✓ · [Convex_Hull_Complex](09_Geometry/Convex_Hull_Complex.cpp) · [Geometry_Lib](09_Geometry/Geometry_Lib.cpp) · [Line_Sweep_Rectangles](09_Geometry/Line_Sweep_Rectangles.cpp) ✓ · [Points_And_Polygons](09_Geometry/Points_And_Polygons.cpp)

## 10 · Misc
[All_Permutations](10_Misc/All_Permutations.cpp) · [Bases](10_Misc/Bases.cpp) · [Big_Int](10_Misc/Big_Int.cpp) ✓ · [Bits](10_Misc/Bits.cpp) · [Coordinate_Compression](10_Misc/Coordinate_Compression.cpp) ✓ · [Gray_Code](10_Misc/Gray_Code.cpp) · [Inversions](10_Misc/Inversions.cpp) ✓ · [Logarithmic_Subarray_Aggregator](10_Misc/Logarithmic_Subarray_Aggregator.cpp) ✓ · [Meet_In_The_Middle](10_Misc/Meet_In_The_Middle.cpp) ✓ · [Misc_Helpers](10_Misc/Misc_Helpers.cpp) · [Permutation_Cycles](10_Misc/Permutation_Cycles.cpp) ✓

## Phase 2: duplicates to merge

Each group below becomes one file (best version kept, missing features carried over, tested).

| Area | Merge into one |
|---|---|
| Geometry | Geometry_Lib, Points_And_Polygons |
| Convex hull | Convex_Hull, Convex_Hull_2, Convex_Hull_Complex |
| KMP | KMP, KMP_2 |
| Knuth | Knuth_Optimization, Knuth_Optimization_Stick_Cutting (example) |
| DSU on tree | DSU_On_Tree, DSU_On_Tree_2 (really small-to-large; decide whether both stay) |

Every template now compiles as a library (alone and under `#define int long long`). Templates without ✓ were
not broken but have no brute-force test yet.

Removed or replaced files are in `archive/` (see its README); nothing was deleted.
