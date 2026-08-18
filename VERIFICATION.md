# Verification status

What actually backs each template. Regenerate with `py tools/checklist.py`;
the data comes from `verify/results.json`, which `tools/verify.py` writes.

| | meaning |
| --- | --- |
| **JUDGE** | passes a real judge's full system tests, run locally against the judge's own checker |
| brute force | agrees with an independent reference over randomised inputs; no judge data exists for it |
| FAIL / TLE | the judge data rejects it - see the note |
| OVER LIMIT | every case is CORRECT, but the slowest run exceeded the limit here |

Times below are the slowest single case on the machine that ran this, against the judge's stated limit. That machine is not the judge - read a thin margin as a warning.

**24 of 149 templates carry a judge verdict.** The rest stand on brute-force agreement,
which catches wrong answers but cannot establish a time bound.

## DP

| Template | Status | Evidence |
| --- | --- | --- |
| [CHT.cpp](DP/CHT.cpp) | brute force | no matching judge problem found |
| [CHT_Monotonic.cpp](DP/CHT_Monotonic.cpp) | brute force | no matching judge problem found |
| [Deque_Trick.cpp](DP/Deque_Trick.cpp) | brute force | no matching judge problem found |
| [Digit_DP.cpp](DP/Digit_DP.cpp) | brute force | no matching judge problem found |
| [Divide_and_Conquer.cpp](DP/Divide_and_Conquer.cpp) | brute force | no matching judge problem found |
| [Knuth_Optimization.cpp](DP/Knuth_Optimization.cpp) | brute force | no matching judge problem found |
| [Li_Chao_Tree.cpp](DP/Li_Chao_Tree.cpp) | brute force | no matching judge problem found |
| [Longest_Increasing_Subsequence.cpp](DP/Longest_Increasing_Subsequence.cpp) | **JUDGE** | [longest_increasing_subsequence](https://judge.yosupo.jp/problem/longest_increasing_subsequence) 19/19 cases, 0.10s of 5s |
| [Meet_in_the_Middle.cpp](DP/Meet_in_the_Middle.cpp) | brute force | no matching judge problem found |
| [Slope_Trick.cpp](DP/Slope_Trick.cpp) | brute force | judge-proven separately: CSES 2132 "Increasing Array II", ACCEPTED 2026-08-17 |
| [SOS.cpp](DP/SOS.cpp) | brute force | no matching judge problem found |
| [Weighted_Job_Scheduling.cpp](DP/Weighted_Job_Scheduling.cpp) | brute force | no matching judge problem found |

## Data_Structure

| Template | Status | Evidence |
| --- | --- | --- |
| [Binary_Trie.cpp](Data_Structure/Binary_Trie.cpp) | brute force | no matching judge problem found |
| [Cartesian_Tree.cpp](Data_Structure/Cartesian_Tree.cpp) | **JUDGE** | [cartesian_tree](https://judge.yosupo.jp/problem/cartesian_tree) 25/25 cases, 0.23s of 5s |
| [DSU.cpp](Data_Structure/DSU.cpp) | **JUDGE** | [unionfind](https://judge.yosupo.jp/problem/unionfind) 18/18 cases, 0.15s of 5s |
| [DSU_Rollback.cpp](Data_Structure/DSU_Rollback.cpp) | brute force | no matching judge problem found |
| [DSU_Weighted.cpp](Data_Structure/DSU_Weighted.cpp) | **JUDGE** | [unionfind_with_potential](https://judge.yosupo.jp/problem/unionfind_with_potential) 18/18 cases, 0.12s of 5s |
| [Hash_Table.cpp](Data_Structure/Hash_Table.cpp) | brute force | no matching judge problem found |
| [Interval_Set.cpp](Data_Structure/Interval_Set.cpp) | brute force | submission ready in verify/Interval_Set (CSES 1735), unsubmitted |
| [MEX.cpp](Data_Structure/MEX.cpp) | brute force | no matching judge problem found |
| [MEX_All_Subarrays.cpp](Data_Structure/MEX_All_Subarrays.cpp) | brute force | no matching judge problem found |
| [MEX_Range_Query.cpp](Data_Structure/MEX_Range_Query.cpp) | brute force | no matching judge problem found |
| [MEX_With_Updates.cpp](Data_Structure/MEX_With_Updates.cpp) | brute force | no matching judge problem found |
| [Monotonic_Queue.cpp](Data_Structure/Monotonic_Queue.cpp) | brute force | no matching judge problem found |
| [Monotonic_Stack.cpp](Data_Structure/Monotonic_Stack.cpp) | brute force | no matching judge problem found |
| [Ordered_Set.cpp](Data_Structure/Ordered_Set.cpp) | brute force | no matching judge problem found |
| [Treap.cpp](Data_Structure/Treap.cpp) | brute force | no matching judge problem found |
| [Treap_Implicit.cpp](Data_Structure/Treap_Implicit.cpp) | brute force | no matching judge problem found |

## Game_Theory

| Template | Status | Evidence |
| --- | --- | --- |
| [Games_On_Graph.cpp](Game_Theory/Games_On_Graph.cpp) | brute force | no matching judge problem found |
| [Grundy.cpp](Game_Theory/Grundy.cpp) | brute force | no matching judge problem found |
| [Hackenbush.cpp](Game_Theory/Hackenbush.cpp) | brute force | no matching judge problem found |
| [Remoteness.cpp](Game_Theory/Remoteness.cpp) | brute force | no matching judge problem found |
| [Wythoff_Game.cpp](Game_Theory/Wythoff_Game.cpp) | brute force | no matching judge problem found |

## Geometry

| Template | Status | Evidence |
| --- | --- | --- |
| [Circle.cpp](Geometry/Circle.cpp) | brute force | no matching judge problem found |
| [Closest_Pair.cpp](Geometry/Closest_Pair.cpp) | brute force | no matching judge problem found |
| [Convex_Hull.cpp](Geometry/Convex_Hull.cpp) | brute force | no matching judge problem found |
| [Geometry.cpp](Geometry/Geometry.cpp) | brute force | no matching judge problem found |
| [Polygon.cpp](Geometry/Polygon.cpp) | brute force | no matching judge problem found |
| [Rectangle_Union.cpp](Geometry/Rectangle_Union.cpp) | brute force | no matching judge problem found |

## Graph

| Template | Status | Evidence |
| --- | --- | --- |
| [2_SAT.cpp](Graph/2_SAT.cpp) | **JUDGE** | [two_sat](https://judge.yosupo.jp/problem/two_sat) 18/18 cases, 0.96s of 5s |
| [Breadth_First_Search.cpp](Graph/Breadth_First_Search.cpp) | brute force | no matching judge problem found |
| [Depth_First_Search.cpp](Graph/Depth_First_Search.cpp) | brute force | no matching judge problem found |
| [Eulerian_Path.cpp](Graph/Eulerian_Path.cpp) | brute force | no matching judge problem found |
| [Kruskal_Reconstruction_Tree.cpp](Graph/Kruskal_Reconstruction_Tree.cpp) | brute force | no matching judge problem found |
| [MST_Kruskal.cpp](Graph/MST_Kruskal.cpp) | **JUDGE** | [minimum_spanning_tree](https://judge.yosupo.jp/problem/minimum_spanning_tree) 31/31 cases, 0.31s of 5s |
| [Strong_Orientation.cpp](Graph/Strong_Orientation.cpp) | brute force | no matching judge problem found |

## Graph/Connectivity

| Template | Status | Evidence |
| --- | --- | --- |
| [Articulation_Points.cpp](Graph/Connectivity/Articulation_Points.cpp) | brute force | no matching judge problem found |
| [Bridges.cpp](Graph/Connectivity/Bridges.cpp) | brute force | no matching judge problem found |
| [Bridges_Online.cpp](Graph/Connectivity/Bridges_Online.cpp) | brute force | no matching judge problem found |
| [Strongly_Connected_Components.cpp](Graph/Connectivity/Strongly_Connected_Components.cpp) | **JUDGE** | [scc](https://judge.yosupo.jp/problem/scc) 12/12 cases, 0.48s of 5s |

## Graph/Flow_And_Matching

| Template | Status | Evidence |
| --- | --- | --- |
| [Bipartite_Matching.cpp](Graph/Flow_And_Matching/Bipartite_Matching.cpp) | TLE | [bipartitematching](https://judge.yosupo.jp/problem/bipartitematching) 41/44 cases |
| [Bipartite_Matching_Hopcroft_Karp.cpp](Graph/Flow_And_Matching/Bipartite_Matching_Hopcroft_Karp.cpp) | **JUDGE** | [bipartitematching](https://judge.yosupo.jp/problem/bipartitematching) 44/44 cases, 1.77s of 5s |
| [Dinic.cpp](Graph/Flow_And_Matching/Dinic.cpp) | brute force | judge-proven separately: CSES 1694 "Download Speed", ACCEPTED 2026-08-18 |
| [Hungarian.cpp](Graph/Flow_And_Matching/Hungarian.cpp) | brute force | no matching judge problem found |
| [Minimum_Cost_Maximum_Flow.cpp](Graph/Flow_And_Matching/Minimum_Cost_Maximum_Flow.cpp) | brute force | no matching judge problem found |

## Graph/Shortest_Path

| Template | Status | Evidence |
| --- | --- | --- |
| [Bellman_Ford.cpp](Graph/Shortest_Path/Bellman_Ford.cpp) | brute force | no matching judge problem found |
| [Bellman_Ford_SPFA.cpp](Graph/Shortest_Path/Bellman_Ford_SPFA.cpp) | brute force | no matching judge problem found |
| [Dijkstra.cpp](Graph/Shortest_Path/Dijkstra.cpp) | **JUDGE** | [shortest_path](https://judge.yosupo.jp/problem/shortest_path) 29/29 cases, 0.74s of 5s |
| [Floyd.cpp](Graph/Shortest_Path/Floyd.cpp) | brute force | no matching judge problem found |

## Math

| Template | Status | Evidence |
| --- | --- | --- |
| [Big_Integer.cpp](Math/Big_Integer.cpp) | brute force | no matching judge problem found |
| [Gray_Code.cpp](Math/Gray_Code.cpp) | brute force | no matching judge problem found |

## Math/Calculus

| Template | Status | Evidence |
| --- | --- | --- |
| [Differentiation.cpp](Math/Calculus/Differentiation.cpp) | brute force | numeric, no exact-answer judge problem exists |
| [Integration.cpp](Math/Calculus/Integration.cpp) | brute force | numeric, no exact-answer judge problem exists |

## Math/Combinatorics

| Template | Status | Evidence |
| --- | --- | --- |
| [Binomial_Coefficients.cpp](Math/Combinatorics/Binomial_Coefficients.cpp) | brute force | no matching judge problem found |
| [Binomial_Coefficients_Any_Mod.cpp](Math/Combinatorics/Binomial_Coefficients_Any_Mod.cpp) | brute force | no matching judge problem found |
| [Catalan.cpp](Math/Combinatorics/Catalan.cpp) | brute force | no matching judge problem found |
| [Permutations.cpp](Math/Combinatorics/Permutations.cpp) | brute force | no matching judge problem found |

## Math/Diophantine

| Template | Status | Evidence |
| --- | --- | --- |
| [LDE.cpp](Math/Diophantine/LDE.cpp) | brute force | no matching judge problem found |
| [LDE_N_Variables.cpp](Math/Diophantine/LDE_N_Variables.cpp) | brute force | no matching judge problem found |
| [LDE_N_Variables_Non_Negative.cpp](Math/Diophantine/LDE_N_Variables_Non_Negative.cpp) | brute force | no matching judge problem found |

## Math/Linear_Algebra

| Template | Status | Evidence |
| --- | --- | --- |
| [Gaussian_Elimination.cpp](Math/Linear_Algebra/Gaussian_Elimination.cpp) | brute force | no matching judge problem found |
| [XOR_Basis.cpp](Math/Linear_Algebra/XOR_Basis.cpp) | brute force | no matching judge problem found |
| [XOR_Basis_Range.cpp](Math/Linear_Algebra/XOR_Basis_Range.cpp) | brute force | no matching judge problem found |

## Math/Number_Theory

| Template | Status | Evidence |
| --- | --- | --- |
| [Base_Conversion.cpp](Math/Number_Theory/Base_Conversion.cpp) | brute force | no matching judge problem found |
| [CRT.cpp](Math/Number_Theory/CRT.cpp) | brute force | no matching judge problem found |
| [Discrete_Log.cpp](Math/Number_Theory/Discrete_Log.cpp) | **JUDGE** | [discrete_logarithm_mod](https://judge.yosupo.jp/problem/discrete_logarithm_mod) 20/20 cases, 0.61s of 10s |
| [Divisors_And_Factors.cpp](Math/Number_Theory/Divisors_And_Factors.cpp) | brute force | no matching judge problem found |
| [Euler_Phi.cpp](Math/Number_Theory/Euler_Phi.cpp) | brute force | no matching judge problem found |
| [Exponentiation.cpp](Math/Number_Theory/Exponentiation.cpp) | brute force | no matching judge problem found |
| [Extended_GCD.cpp](Math/Number_Theory/Extended_GCD.cpp) | brute force | no matching judge problem found |
| [Fibonacci.cpp](Math/Number_Theory/Fibonacci.cpp) | brute force | no matching judge problem found |
| [Miller_Rabin_Pollard_Rho.cpp](Math/Number_Theory/Miller_Rabin_Pollard_Rho.cpp) | **JUDGE** | [primality_test](https://judge.yosupo.jp/problem/primality_test) 12/12 cases, 1.04s of 5s; [factorize](https://judge.yosupo.jp/problem/factorize) 31/31 cases, 0.36s of 10s |
| [Mod_Arithmetic.cpp](Math/Number_Theory/Mod_Arithmetic.cpp) | brute force | no matching judge problem found |
| [Modular_Int.cpp](Math/Number_Theory/Modular_Int.cpp) | brute force | no matching judge problem found |
| [Sieve.cpp](Math/Number_Theory/Sieve.cpp) | OVER LIMIT | [enumerate_primes](https://judge.yosupo.jp/problem/enumerate_primes) 10/10 cases, 12.03s of 10s |

## Math/Polynomial

| Template | Status | Evidence |
| --- | --- | --- |
| [FFT.cpp](Math/Polynomial/FFT.cpp) | brute force | no matching judge problem found |
| [FWHT.cpp](Math/Polynomial/FWHT.cpp) | **JUDGE** | [bitwise_xor_convolution](https://judge.yosupo.jp/problem/bitwise_xor_convolution) 13/13 cases, 0.84s of 5s |
| [Lagrange_Interpolation.cpp](Math/Polynomial/Lagrange_Interpolation.cpp) | brute force | no matching judge problem found |
| [NTT.cpp](Math/Polynomial/NTT.cpp) | **JUDGE** | [convolution_mod](https://judge.yosupo.jp/problem/convolution_mod) 53/53 cases, 0.53s of 5s |
| [NTT_Any_Mod.cpp](Math/Polynomial/NTT_Any_Mod.cpp) | **JUDGE** | [convolution_mod_1000000007](https://judge.yosupo.jp/problem/convolution_mod_1000000007) 48/48 cases, 1.25s of 10s |

## Misc

| Template | Status | Evidence |
| --- | --- | --- |
| [Coordinate_Compression.cpp](Misc/Coordinate_Compression.cpp) | brute force | no matching judge problem found |
| [Huffman_Coding.cpp](Misc/Huffman_Coding.cpp) | brute force | no matching judge problem found |
| [Josephus.cpp](Misc/Josephus.cpp) | brute force | no matching judge problem found |
| [Logarithmic_Subarray_Aggregator.cpp](Misc/Logarithmic_Subarray_Aggregator.cpp) | brute force | no matching judge problem found |
| [Majority_Element.cpp](Misc/Majority_Element.cpp) | brute force | Library Checker's majority_voting is a range query with updates; this is whole-array Boyer-Moore |
| [Ternary_Search.cpp](Misc/Ternary_Search.cpp) | brute force | no matching judge problem found |

## Range_Query

| Template | Status | Evidence |
| --- | --- | --- |
| [Count_Inversions.cpp](Range_Query/Count_Inversions.cpp) | brute force | no matching judge problem found |
| [Dynamic_Connectivity.cpp](Range_Query/Dynamic_Connectivity.cpp) | brute force | no matching judge problem found |
| [Fenwick_Tree.cpp](Range_Query/Fenwick_Tree.cpp) | **JUDGE** | [point_add_range_sum](https://judge.yosupo.jp/problem/point_add_range_sum) 21/21 cases, 0.33s of 5s |
| [Fenwick_Tree_2D.cpp](Range_Query/Fenwick_Tree_2D.cpp) | brute force | no matching judge problem found |
| [Fenwick_Tree_Range.cpp](Range_Query/Fenwick_Tree_Range.cpp) | brute force | no matching judge problem found |
| [Merge_Sort_Tree.cpp](Range_Query/Merge_Sort_Tree.cpp) | brute force | no matching judge problem found |
| [MO.cpp](Range_Query/MO.cpp) | brute force | no matching judge problem found |
| [MO_on_Tree.cpp](Range_Query/MO_on_Tree.cpp) | brute force | no matching judge problem found |
| [Parallel_Binary_Search.cpp](Range_Query/Parallel_Binary_Search.cpp) | brute force | no matching judge problem found |
| [Prefix_Sum_2D.cpp](Range_Query/Prefix_Sum_2D.cpp) | brute force | no matching judge problem found |
| [Sparse_Table.cpp](Range_Query/Sparse_Table.cpp) | **JUDGE** | [staticrmq](https://judge.yosupo.jp/problem/staticrmq) 27/27 cases, 0.38s of 5s |
| [Sparse_Table_2D.cpp](Range_Query/Sparse_Table_2D.cpp) | brute force | no matching judge problem found |
| [SQRT_Decomposition.cpp](Range_Query/SQRT_Decomposition.cpp) | brute force | no matching judge problem found |
| [Wavelet_Tree.cpp](Range_Query/Wavelet_Tree.cpp) | **JUDGE** | [range_kth_smallest](https://judge.yosupo.jp/problem/range_kth_smallest) 24/24 cases, 2.40s of 5s |

## Range_Query/Segment_Tree

| Template | Status | Evidence |
| --- | --- | --- |
| [Segment_Tree.cpp](Range_Query/Segment_Tree/Segment_Tree.cpp) | **JUDGE** | [point_add_range_sum](https://judge.yosupo.jp/problem/point_add_range_sum) 21/21 cases, 0.50s of 5s |
| [Segment_Tree_2D.cpp](Range_Query/Segment_Tree/Segment_Tree_2D.cpp) | brute force | no matching judge problem found |
| [Segment_Tree_Beats.cpp](Range_Query/Segment_Tree/Segment_Tree_Beats.cpp) | brute force | no matching judge problem found |
| [Segment_Tree_Dynamic.cpp](Range_Query/Segment_Tree/Segment_Tree_Dynamic.cpp) | brute force | no matching judge problem found |
| [Segment_Tree_Dynamic_2D.cpp](Range_Query/Segment_Tree/Segment_Tree_Dynamic_2D.cpp) | brute force | no matching judge problem found |
| [Segment_Tree_Iterative.cpp](Range_Query/Segment_Tree/Segment_Tree_Iterative.cpp) | **JUDGE** | [point_add_range_sum](https://judge.yosupo.jp/problem/point_add_range_sum) 21/21 cases, 0.38s of 5s |
| [Segment_Tree_Lazy.cpp](Range_Query/Segment_Tree/Segment_Tree_Lazy.cpp) | brute force | Library Checker has range AFFINE range sum; this template does range ADD range sum |
| [Segment_Tree_Lazy_Persistent.cpp](Range_Query/Segment_Tree/Segment_Tree_Lazy_Persistent.cpp) | brute force | no matching judge problem found |
| [Segment_Tree_Merging.cpp](Range_Query/Segment_Tree/Segment_Tree_Merging.cpp) | brute force | no matching judge problem found |
| [Segment_Tree_Persistent.cpp](Range_Query/Segment_Tree/Segment_Tree_Persistent.cpp) | brute force | no matching judge problem found |

## Strings

| Template | Status | Evidence |
| --- | --- | --- |
| [Booth.cpp](Strings/Booth.cpp) | brute force | no matching judge problem found |
| [Expression_Parsing.cpp](Strings/Expression_Parsing.cpp) | brute force | submission ready in verify/Expression_Parsing (SPOJ ONP), unsubmitted |
| [KMP.cpp](Strings/KMP.cpp) | brute force | no matching judge problem found |
| [Manacher.cpp](Strings/Manacher.cpp) | **JUDGE** | [enumerate_palindromes](https://judge.yosupo.jp/problem/enumerate_palindromes) 24/24 cases, 0.09s of 5s |
| [Palindromic_Tree.cpp](Strings/Palindromic_Tree.cpp) | brute force | no matching judge problem found |
| [Suffix_Array.cpp](Strings/Suffix_Array.cpp) | **JUDGE** | [suffixarray](https://judge.yosupo.jp/problem/suffixarray) 50/50 cases, 0.77s of 5s |
| [Suffix_Array_Integers.cpp](Strings/Suffix_Array_Integers.cpp) | brute force | no matching judge problem found |
| [Suffix_Automaton.cpp](Strings/Suffix_Automaton.cpp) | **JUDGE** | [number_of_substrings](https://judge.yosupo.jp/problem/number_of_substrings) 24/24 cases, 0.69s of 5s |
| [Trie.cpp](Strings/Trie.cpp) | brute force | no matching judge problem found |
| [Z_Function.cpp](Strings/Z_Function.cpp) | **JUDGE** | [zalgorithm](https://judge.yosupo.jp/problem/zalgorithm) 29/29 cases, 0.07s of 5s |

## Strings/Aho_Corasick

| Template | Status | Evidence |
| --- | --- | --- |
| [Aho_Corasick.cpp](Strings/Aho_Corasick/Aho_Corasick.cpp) | brute force | no matching judge problem found |
| [Aho_Corasick_Dynamic.cpp](Strings/Aho_Corasick/Aho_Corasick_Dynamic.cpp) | brute force | no matching judge problem found |

## Strings/Hashing

| Template | Status | Evidence |
| --- | --- | --- |
| [Hash.cpp](Strings/Hashing/Hash.cpp) | brute force | no matching judge problem found |
| [Hash_2D.cpp](Strings/Hashing/Hash_2D.cpp) | brute force | no matching judge problem found |
| [Hash_Deque.cpp](Strings/Hashing/Hash_Deque.cpp) | brute force | no matching judge problem found |
| [Hash_Lazy_Segment_Tree.cpp](Strings/Hashing/Hash_Lazy_Segment_Tree.cpp) | brute force | no matching judge problem found |
| [Hash_Segment_Tree.cpp](Strings/Hashing/Hash_Segment_Tree.cpp) | brute force | no matching judge problem found |
| [XOR_Hashing.cpp](Strings/Hashing/XOR_Hashing.cpp) | brute force | no matching judge problem found |

## Tree

| Template | Status | Evidence |
| --- | --- | --- |
| [Centroid_Decomposition.cpp](Tree/Centroid_Decomposition.cpp) | brute force | no matching judge problem found |
| [Diameter.cpp](Tree/Diameter.cpp) | brute force | Library Checker's tree_diameter is WEIGHTED; this template is unweighted |
| [DSU_on_Tree.cpp](Tree/DSU_on_Tree.cpp) | brute force | no matching judge problem found |
| [Euler_Tour.cpp](Tree/Euler_Tour.cpp) | brute force | no matching judge problem found |
| [HLD.cpp](Tree/HLD.cpp) | brute force | no matching judge problem found |
| [LCA.cpp](Tree/LCA.cpp) | **JUDGE** | [lca](https://judge.yosupo.jp/problem/lca) 25/25 cases, 1.85s of 5s |
| [LCT.cpp](Tree/LCT.cpp) | brute force | no matching judge problem found |
| [Rerooting.cpp](Tree/Rerooting.cpp) | brute force | submission ready in verify/Rerooting (CSES 1133); tree_path_composite_sum is the Library Checker equivalent, not yet driven |
| [Small_To_Large.cpp](Tree/Small_To_Large.cpp) | brute force | no matching judge problem found |
| [Tree_Isomorphism.cpp](Tree/Tree_Isomorphism.cpp) | brute force | no matching judge problem found |
| [Virtual_Tree.cpp](Tree/Virtual_Tree.cpp) | brute force | no matching judge problem found |

