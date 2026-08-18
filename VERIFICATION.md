# Verification status

What actually backs each template. Regenerate with `py tools/checklist.py`;
the data comes from `verify/results.json`, which `tools/verify.py` writes.

| | meaning |
| --- | --- |
| **JUDGE** | passes a real judge's full system tests, run locally against the judge's own checker |
| brute force | a test in `tests/` compares it to an independent reference over randomised inputs |
| UNGUARDED | neither - nothing here re-checks it |
| FAIL / TLE | the judge data rejects it - see the note |
| OVER LIMIT | every case is CORRECT, but the slowest run exceeded the limit here |

Times below are the slowest single case on the machine that ran this, against the judge's stated limit. That machine is not the judge - read a thin margin as a warning.

**29 of 150 templates carry a judge verdict.** The rest stand on brute-force agreement,
which catches wrong answers but cannot establish a time bound.

## DP

| Template | Status | Evidence |
| --- | --- | --- |
| [Aliens_Trick.cpp](DP/Aliens_Trick.cpp) | brute force | tests/Aliens_Trick.cpp - submission ready in verify/Aliens_Trick (AtCoder ABC 218 H), unsubmitted |
| [CHT.cpp](DP/CHT.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [CHT_Monotonic.cpp](DP/CHT_Monotonic.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Deque_Trick.cpp](DP/Deque_Trick.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Digit_DP.cpp](DP/Digit_DP.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Divide_and_Conquer.cpp](DP/Divide_and_Conquer.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Knuth_Optimization.cpp](DP/Knuth_Optimization.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Li_Chao_Tree.cpp](DP/Li_Chao_Tree.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Longest_Increasing_Subsequence.cpp](DP/Longest_Increasing_Subsequence.cpp) | **JUDGE** | [longest_increasing_subsequence](https://judge.yosupo.jp/problem/longest_increasing_subsequence) 19/19 cases, 0.09s of 5s |
| [Meet_in_the_Middle.cpp](DP/Meet_in_the_Middle.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Slope_Trick.cpp](DP/Slope_Trick.cpp) | UNGUARDED | judge-proven separately: CSES 2132 "Increasing Array II", ACCEPTED 2026-08-17 |
| [SOS.cpp](DP/SOS.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Weighted_Job_Scheduling.cpp](DP/Weighted_Job_Scheduling.cpp) | UNGUARDED | no judge problem and no brute-force test |

## Data_Structure

| Template | Status | Evidence |
| --- | --- | --- |
| [Binary_Trie.cpp](Data_Structure/Binary_Trie.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Cartesian_Tree.cpp](Data_Structure/Cartesian_Tree.cpp) | **JUDGE** | [cartesian_tree](https://judge.yosupo.jp/problem/cartesian_tree) 25/25 cases, 0.20s of 5s |
| [DSU.cpp](Data_Structure/DSU.cpp) | **JUDGE** | [unionfind](https://judge.yosupo.jp/problem/unionfind) 18/18 cases, 0.14s of 5s |
| [DSU_Rollback.cpp](Data_Structure/DSU_Rollback.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [DSU_Weighted.cpp](Data_Structure/DSU_Weighted.cpp) | **JUDGE** | [unionfind_with_potential](https://judge.yosupo.jp/problem/unionfind_with_potential) 18/18 cases, 0.11s of 5s |
| [Hash_Table.cpp](Data_Structure/Hash_Table.cpp) | **JUDGE** | [associative_array](https://judge.yosupo.jp/problem/associative_array) 20/20 cases, 0.87s of 5s |
| [Interval_Set.cpp](Data_Structure/Interval_Set.cpp) | brute force | tests/Interval_Set.cpp - submission ready in verify/Interval_Set (CSES 1735), unsubmitted |
| [MEX.cpp](Data_Structure/MEX.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [MEX_All_Subarrays.cpp](Data_Structure/MEX_All_Subarrays.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [MEX_Range_Query.cpp](Data_Structure/MEX_Range_Query.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [MEX_With_Updates.cpp](Data_Structure/MEX_With_Updates.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Monotonic_Queue.cpp](Data_Structure/Monotonic_Queue.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Monotonic_Stack.cpp](Data_Structure/Monotonic_Stack.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Ordered_Set.cpp](Data_Structure/Ordered_Set.cpp) | **JUDGE** | [predecessor_problem](https://judge.yosupo.jp/problem/predecessor_problem) 22/22 cases, 5.79s of 10s |
| [Treap.cpp](Data_Structure/Treap.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Treap_Implicit.cpp](Data_Structure/Treap_Implicit.cpp) | UNGUARDED | no judge problem and no brute-force test |

## Game_Theory

| Template | Status | Evidence |
| --- | --- | --- |
| [Games_On_Graph.cpp](Game_Theory/Games_On_Graph.cpp) | brute force | tests/Game_Theory.cpp |
| [Grundy.cpp](Game_Theory/Grundy.cpp) | brute force | tests/Game_Theory.cpp |
| [Hackenbush.cpp](Game_Theory/Hackenbush.cpp) | brute force | tests/Hackenbush.cpp |
| [Remoteness.cpp](Game_Theory/Remoteness.cpp) | brute force | tests/Remoteness.cpp |
| [Wythoff_Game.cpp](Game_Theory/Wythoff_Game.cpp) | brute force | tests/Game_Theory.cpp |

## Geometry

| Template | Status | Evidence |
| --- | --- | --- |
| [Circle.cpp](Geometry/Circle.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Closest_Pair.cpp](Geometry/Closest_Pair.cpp) | brute force | tests/Closest_Pair.cpp |
| [Convex_Hull.cpp](Geometry/Convex_Hull.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Geometry.cpp](Geometry/Geometry.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Polygon.cpp](Geometry/Polygon.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Rectangle_Union.cpp](Geometry/Rectangle_Union.cpp) | **JUDGE** | [area_of_union_of_rectangles](https://judge.yosupo.jp/problem/area_of_union_of_rectangles) 21/21 cases, 3.26s of 5s |

## Graph

| Template | Status | Evidence |
| --- | --- | --- |
| [2_SAT.cpp](Graph/2_SAT.cpp) | **JUDGE** | [two_sat](https://judge.yosupo.jp/problem/two_sat) 18/18 cases, 0.95s of 5s |
| [Breadth_First_Search.cpp](Graph/Breadth_First_Search.cpp) | brute force | tests/Breadth_First_Search.cpp |
| [Depth_First_Search.cpp](Graph/Depth_First_Search.cpp) | brute force | tests/MST_And_Depth_First_Search.cpp |
| [Eulerian_Path.cpp](Graph/Eulerian_Path.cpp) | brute force | tests/Assorted_Additions.cpp |
| [Kruskal_Reconstruction_Tree.cpp](Graph/Kruskal_Reconstruction_Tree.cpp) | brute force | tests/Kruskal_Reconstruction_Tree.cpp |
| [MST_Kruskal.cpp](Graph/MST_Kruskal.cpp) | **JUDGE** | [minimum_spanning_tree](https://judge.yosupo.jp/problem/minimum_spanning_tree) 31/31 cases, 0.26s of 5s |
| [Strong_Orientation.cpp](Graph/Strong_Orientation.cpp) | brute force | tests/Two_SAT_And_Strong_Orientation.cpp |

## Graph/Connectivity

| Template | Status | Evidence |
| --- | --- | --- |
| [Articulation_Points.cpp](Graph/Connectivity/Articulation_Points.cpp) | brute force | tests/Connectivity.cpp |
| [Bridges.cpp](Graph/Connectivity/Bridges.cpp) | brute force | tests/Connectivity.cpp |
| [Bridges_Online.cpp](Graph/Connectivity/Bridges_Online.cpp) | brute force | tests/Connectivity.cpp |
| [Strongly_Connected_Components.cpp](Graph/Connectivity/Strongly_Connected_Components.cpp) | **JUDGE** | [scc](https://judge.yosupo.jp/problem/scc) 12/12 cases, 0.52s of 5s |

## Graph/Flow_And_Matching

| Template | Status | Evidence |
| --- | --- | --- |
| [Bipartite_Matching.cpp](Graph/Flow_And_Matching/Bipartite_Matching.cpp) | TLE | [bipartitematching](https://judge.yosupo.jp/problem/bipartitematching) 40/44 cases |
| [Bipartite_Matching_Hopcroft_Karp.cpp](Graph/Flow_And_Matching/Bipartite_Matching_Hopcroft_Karp.cpp) | **JUDGE** | [bipartitematching](https://judge.yosupo.jp/problem/bipartitematching) 44/44 cases, 0.62s of 5s |
| [Dinic.cpp](Graph/Flow_And_Matching/Dinic.cpp) | brute force | tests/Flow_And_Matching.cpp - judge-proven separately: CSES 1694 "Download Speed", ACCEPTED 2026-08-18 |
| [Hungarian.cpp](Graph/Flow_And_Matching/Hungarian.cpp) | **JUDGE** | [assignment](https://judge.yosupo.jp/problem/assignment) 14/14 cases, 0.12s of 5s |
| [Minimum_Cost_Maximum_Flow.cpp](Graph/Flow_And_Matching/Minimum_Cost_Maximum_Flow.cpp) | brute force | tests/Flow_And_Matching.cpp |

## Graph/Shortest_Path

| Template | Status | Evidence |
| --- | --- | --- |
| [Bellman_Ford.cpp](Graph/Shortest_Path/Bellman_Ford.cpp) | brute force | tests/Shortest_Path.cpp |
| [Bellman_Ford_SPFA.cpp](Graph/Shortest_Path/Bellman_Ford_SPFA.cpp) | brute force | tests/Shortest_Path.cpp |
| [Dijkstra.cpp](Graph/Shortest_Path/Dijkstra.cpp) | **JUDGE** | [shortest_path](https://judge.yosupo.jp/problem/shortest_path) 29/29 cases, 0.66s of 5s |
| [Floyd.cpp](Graph/Shortest_Path/Floyd.cpp) | brute force | tests/Shortest_Path.cpp |

## Math

| Template | Status | Evidence |
| --- | --- | --- |
| [Big_Integer.cpp](Math/Big_Integer.cpp) | brute force | tests/Big_Integer.cpp |
| [Gray_Code.cpp](Math/Gray_Code.cpp) | UNGUARDED | no judge problem and no brute-force test |

## Math/Calculus

| Template | Status | Evidence |
| --- | --- | --- |
| [Differentiation.cpp](Math/Calculus/Differentiation.cpp) | brute force | tests/Calculus.cpp - numeric, no exact-answer judge problem exists |
| [Integration.cpp](Math/Calculus/Integration.cpp) | brute force | tests/Calculus.cpp - numeric, no exact-answer judge problem exists |

## Math/Combinatorics

| Template | Status | Evidence |
| --- | --- | --- |
| [Binomial_Coefficients.cpp](Math/Combinatorics/Binomial_Coefficients.cpp) | brute force | tests/Combinatorics.cpp |
| [Binomial_Coefficients_Any_Mod.cpp](Math/Combinatorics/Binomial_Coefficients_Any_Mod.cpp) | brute force | tests/Binomial_Coefficients_Any_Mod.cpp |
| [Catalan.cpp](Math/Combinatorics/Catalan.cpp) | brute force | tests/Combinatorics.cpp |
| [Permutations.cpp](Math/Combinatorics/Permutations.cpp) | brute force | tests/Combinatorics.cpp |

## Math/Diophantine

| Template | Status | Evidence |
| --- | --- | --- |
| [LDE.cpp](Math/Diophantine/LDE.cpp) | brute force | tests/Diophantine.cpp |
| [LDE_N_Variables.cpp](Math/Diophantine/LDE_N_Variables.cpp) | brute force | tests/Diophantine.cpp |
| [LDE_N_Variables_Non_Negative.cpp](Math/Diophantine/LDE_N_Variables_Non_Negative.cpp) | brute force | tests/Diophantine.cpp |

## Math/Linear_Algebra

| Template | Status | Evidence |
| --- | --- | --- |
| [Gaussian_Elimination.cpp](Math/Linear_Algebra/Gaussian_Elimination.cpp) | brute force | tests/Linear_Algebra.cpp |
| [XOR_Basis.cpp](Math/Linear_Algebra/XOR_Basis.cpp) | brute force | tests/Linear_Algebra.cpp |
| [XOR_Basis_Range.cpp](Math/Linear_Algebra/XOR_Basis_Range.cpp) | brute force | tests/Linear_Algebra.cpp |

## Math/Number_Theory

| Template | Status | Evidence |
| --- | --- | --- |
| [Base_Conversion.cpp](Math/Number_Theory/Base_Conversion.cpp) | brute force | tests/Number_Theory_B.cpp |
| [CRT.cpp](Math/Number_Theory/CRT.cpp) | brute force | tests/Number_Theory_A.cpp |
| [Discrete_Log.cpp](Math/Number_Theory/Discrete_Log.cpp) | **JUDGE** | [discrete_logarithm_mod](https://judge.yosupo.jp/problem/discrete_logarithm_mod) 20/20 cases, 0.48s of 10s |
| [Divisors_And_Factors.cpp](Math/Number_Theory/Divisors_And_Factors.cpp) | brute force | tests/Number_Theory_B.cpp |
| [Euler_Phi.cpp](Math/Number_Theory/Euler_Phi.cpp) | brute force | tests/Number_Theory_B.cpp |
| [Exponentiation.cpp](Math/Number_Theory/Exponentiation.cpp) | brute force | tests/Number_Theory_A.cpp |
| [Extended_GCD.cpp](Math/Number_Theory/Extended_GCD.cpp) | brute force | tests/Number_Theory_A.cpp |
| [Fibonacci.cpp](Math/Number_Theory/Fibonacci.cpp) | brute force | tests/Number_Theory_B.cpp |
| [Miller_Rabin_Pollard_Rho.cpp](Math/Number_Theory/Miller_Rabin_Pollard_Rho.cpp) | **JUDGE** | [primality_test](https://judge.yosupo.jp/problem/primality_test) 12/12 cases, 1.01s of 5s; [factorize](https://judge.yosupo.jp/problem/factorize) 31/31 cases, 0.34s of 10s |
| [Mod_Arithmetic.cpp](Math/Number_Theory/Mod_Arithmetic.cpp) | brute force | tests/Number_Theory_A.cpp |
| [Modular_Int.cpp](Math/Number_Theory/Modular_Int.cpp) | brute force | tests/Modular_Int_And_Hash_Table.cpp |
| [Sieve.cpp](Math/Number_Theory/Sieve.cpp) | **JUDGE** | [enumerate_primes](https://judge.yosupo.jp/problem/enumerate_primes) 10/10 cases, 9.55s of 10s |

## Math/Polynomial

| Template | Status | Evidence |
| --- | --- | --- |
| [FFT.cpp](Math/Polynomial/FFT.cpp) | brute force | tests/Polynomial.cpp |
| [FWHT.cpp](Math/Polynomial/FWHT.cpp) | **JUDGE** | [bitwise_xor_convolution](https://judge.yosupo.jp/problem/bitwise_xor_convolution) 13/13 cases, 0.78s of 5s |
| [Lagrange_Interpolation.cpp](Math/Polynomial/Lagrange_Interpolation.cpp) | brute force | tests/Lagrange_Interpolation.cpp |
| [NTT.cpp](Math/Polynomial/NTT.cpp) | **JUDGE** | [convolution_mod](https://judge.yosupo.jp/problem/convolution_mod) 53/53 cases, 0.56s of 5s |
| [NTT_Any_Mod.cpp](Math/Polynomial/NTT_Any_Mod.cpp) | **JUDGE** | [convolution_mod_1000000007](https://judge.yosupo.jp/problem/convolution_mod_1000000007) 48/48 cases, 1.39s of 10s |

## Misc

| Template | Status | Evidence |
| --- | --- | --- |
| [Coordinate_Compression.cpp](Misc/Coordinate_Compression.cpp) | brute force | tests/Coordinate_Compression_And_Aggregator.cpp |
| [Huffman_Coding.cpp](Misc/Huffman_Coding.cpp) | brute force | tests/Modular_Int_And_Hash_Table.cpp |
| [Josephus.cpp](Misc/Josephus.cpp) | brute force | tests/Josephus_Ternary_Prefix2D_Majority.cpp |
| [Logarithmic_Subarray_Aggregator.cpp](Misc/Logarithmic_Subarray_Aggregator.cpp) | brute force | tests/Coordinate_Compression_And_Aggregator.cpp |
| [Majority_Element.cpp](Misc/Majority_Element.cpp) | brute force | tests/Josephus_Ternary_Prefix2D_Majority.cpp - Library Checker's majority_voting is a range query with updates; this is whole-array Boyer-Moore |
| [Ternary_Search.cpp](Misc/Ternary_Search.cpp) | brute force | tests/Josephus_Ternary_Prefix2D_Majority.cpp |

## Range_Query

| Template | Status | Evidence |
| --- | --- | --- |
| [Count_Inversions.cpp](Range_Query/Count_Inversions.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Dynamic_Connectivity.cpp](Range_Query/Dynamic_Connectivity.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Fenwick_Tree.cpp](Range_Query/Fenwick_Tree.cpp) | **JUDGE** | [point_add_range_sum](https://judge.yosupo.jp/problem/point_add_range_sum) 21/21 cases, 0.29s of 5s |
| [Fenwick_Tree_2D.cpp](Range_Query/Fenwick_Tree_2D.cpp) | brute force | tests/Assorted_Additions.cpp |
| [Fenwick_Tree_Range.cpp](Range_Query/Fenwick_Tree_Range.cpp) | brute force | tests/Fenwick_Tree_Range.cpp |
| [Merge_Sort_Tree.cpp](Range_Query/Merge_Sort_Tree.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [MO.cpp](Range_Query/MO.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [MO_on_Tree.cpp](Range_Query/MO_on_Tree.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Parallel_Binary_Search.cpp](Range_Query/Parallel_Binary_Search.cpp) | brute force | tests/Parallel_Binary_Search.cpp |
| [Prefix_Sum_2D.cpp](Range_Query/Prefix_Sum_2D.cpp) | brute force | tests/Josephus_Ternary_Prefix2D_Majority.cpp |
| [Sparse_Table.cpp](Range_Query/Sparse_Table.cpp) | **JUDGE** | [staticrmq](https://judge.yosupo.jp/problem/staticrmq) 27/27 cases, 0.41s of 5s |
| [Sparse_Table_2D.cpp](Range_Query/Sparse_Table_2D.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [SQRT_Decomposition.cpp](Range_Query/SQRT_Decomposition.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Wavelet_Tree.cpp](Range_Query/Wavelet_Tree.cpp) | **JUDGE** | [range_kth_smallest](https://judge.yosupo.jp/problem/range_kth_smallest) 24/24 cases, 1.96s of 5s |

## Range_Query/Segment_Tree

| Template | Status | Evidence |
| --- | --- | --- |
| [Segment_Tree.cpp](Range_Query/Segment_Tree/Segment_Tree.cpp) | **JUDGE** | [point_add_range_sum](https://judge.yosupo.jp/problem/point_add_range_sum) 21/21 cases, 0.44s of 5s |
| [Segment_Tree_2D.cpp](Range_Query/Segment_Tree/Segment_Tree_2D.cpp) | brute force | tests/Segment_Tree_Family.cpp |
| [Segment_Tree_Beats.cpp](Range_Query/Segment_Tree/Segment_Tree_Beats.cpp) | brute force | tests/Segment_Tree_Family.cpp |
| [Segment_Tree_Dynamic.cpp](Range_Query/Segment_Tree/Segment_Tree_Dynamic.cpp) | brute force | tests/Segment_Tree_Family.cpp |
| [Segment_Tree_Dynamic_2D.cpp](Range_Query/Segment_Tree/Segment_Tree_Dynamic_2D.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Segment_Tree_Iterative.cpp](Range_Query/Segment_Tree/Segment_Tree_Iterative.cpp) | **JUDGE** | [point_add_range_sum](https://judge.yosupo.jp/problem/point_add_range_sum) 21/21 cases, 0.41s of 5s |
| [Segment_Tree_Lazy.cpp](Range_Query/Segment_Tree/Segment_Tree_Lazy.cpp) | brute force | tests/Segment_Tree_Family.cpp - Library Checker has range AFFINE range sum; this template does range ADD range sum |
| [Segment_Tree_Lazy_Persistent.cpp](Range_Query/Segment_Tree/Segment_Tree_Lazy_Persistent.cpp) | brute force | tests/Segment_Tree_Family.cpp |
| [Segment_Tree_Merging.cpp](Range_Query/Segment_Tree/Segment_Tree_Merging.cpp) | brute force | tests/Segment_Tree_Merging.cpp |
| [Segment_Tree_Persistent.cpp](Range_Query/Segment_Tree/Segment_Tree_Persistent.cpp) | brute force | tests/Segment_Tree_Family.cpp |

## Strings

| Template | Status | Evidence |
| --- | --- | --- |
| [Booth.cpp](Strings/Booth.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Expression_Parsing.cpp](Strings/Expression_Parsing.cpp) | brute force | tests/Expression_Parsing.cpp - submission ready in verify/Expression_Parsing (SPOJ ONP), unsubmitted |
| [KMP.cpp](Strings/KMP.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Manacher.cpp](Strings/Manacher.cpp) | **JUDGE** | [enumerate_palindromes](https://judge.yosupo.jp/problem/enumerate_palindromes) 24/24 cases, 0.10s of 5s |
| [Palindromic_Tree.cpp](Strings/Palindromic_Tree.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Suffix_Array.cpp](Strings/Suffix_Array.cpp) | **JUDGE** | [suffixarray](https://judge.yosupo.jp/problem/suffixarray) 50/50 cases, 0.76s of 5s |
| [Suffix_Array_Integers.cpp](Strings/Suffix_Array_Integers.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Suffix_Automaton.cpp](Strings/Suffix_Automaton.cpp) | **JUDGE** | [number_of_substrings](https://judge.yosupo.jp/problem/number_of_substrings) 24/24 cases, 0.66s of 5s |
| [Trie.cpp](Strings/Trie.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Z_Function.cpp](Strings/Z_Function.cpp) | **JUDGE** | [zalgorithm](https://judge.yosupo.jp/problem/zalgorithm) 29/29 cases, 0.06s of 5s |

## Strings/Aho_Corasick

| Template | Status | Evidence |
| --- | --- | --- |
| [Aho_Corasick.cpp](Strings/Aho_Corasick/Aho_Corasick.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Aho_Corasick_Dynamic.cpp](Strings/Aho_Corasick/Aho_Corasick_Dynamic.cpp) | UNGUARDED | no judge problem and no brute-force test |

## Strings/Hashing

| Template | Status | Evidence |
| --- | --- | --- |
| [Hash.cpp](Strings/Hashing/Hash.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Hash_2D.cpp](Strings/Hashing/Hash_2D.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Hash_Deque.cpp](Strings/Hashing/Hash_Deque.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Hash_Lazy_Segment_Tree.cpp](Strings/Hashing/Hash_Lazy_Segment_Tree.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Hash_Segment_Tree.cpp](Strings/Hashing/Hash_Segment_Tree.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [XOR_Hashing.cpp](Strings/Hashing/XOR_Hashing.cpp) | UNGUARDED | no judge problem and no brute-force test |

## Tree

| Template | Status | Evidence |
| --- | --- | --- |
| [Centroid_Decomposition.cpp](Tree/Centroid_Decomposition.cpp) | brute force | tests/Tree_Family.cpp |
| [Diameter.cpp](Tree/Diameter.cpp) | brute force | tests/Tree_Family.cpp - Library Checker's tree_diameter is WEIGHTED; this template is unweighted |
| [DSU_on_Tree.cpp](Tree/DSU_on_Tree.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Euler_Tour.cpp](Tree/Euler_Tour.cpp) | brute force | tests/Tree_Family.cpp |
| [HLD.cpp](Tree/HLD.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [LCA.cpp](Tree/LCA.cpp) | **JUDGE** | [lca](https://judge.yosupo.jp/problem/lca) 25/25 cases, 1.60s of 5s |
| [LCT.cpp](Tree/LCT.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Rerooting.cpp](Tree/Rerooting.cpp) | brute force | tests/Rerooting.cpp - submission ready in verify/Rerooting (CSES 1133); tree_path_composite_sum is the Library Checker equivalent, not yet driven |
| [Small_To_Large.cpp](Tree/Small_To_Large.cpp) | UNGUARDED | no judge problem and no brute-force test |
| [Tree_Isomorphism.cpp](Tree/Tree_Isomorphism.cpp) | brute force | tests/Tree_Family.cpp |
| [Virtual_Tree.cpp](Tree/Virtual_Tree.cpp) | brute force | tests/Virtual_Tree.cpp |


**51 templates are UNGUARDED** - no judge problem and no brute-force test.

