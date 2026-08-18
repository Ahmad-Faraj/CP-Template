# CP-Template

Contest reference library. 143 templates. Grab, paste, delete `solve()`, go.

Every file opens with a fixed header — what it does, **when to reach for it**, what it
handles, complexity, indexing, gotchas. Read the header, not the code. Nothing in this
repo explains how an algorithm works; that is deliberate.

Rules for adding or changing a template: [.claude/CLAUDE.md](.claude/CLAUDE.md).

## Checking

```powershell
pwsh tools/check.ps1              # whole repo: header contract + compiles clean
pwsh tools/check.ps1 -Path DP     # one directory
pwsh tools/check.ps1 -NoCompile   # contract only, instant
```

Needs PowerShell 7+ and `g++` on PATH. Exits non-zero on any failure.

## Index

`■` normalized and verified · `◧` partly · `□` not yet

A directory subdivides only once it passes ~15 files. Everything is `Title_Snake_Case`.

| Directory | | Count | Subfolders |
| --- | --- | --- | --- |
| [Range_Query](Range_Query/) | ■ | 24 | `Segment_Tree` |
| [Strings](Strings/) | ■ | 18 | `Hashing` `Aho_Corasick` |
| [Data_Structure](Data_Structure/) | ■ | 16 | |
| [DP](DP/) | ■ | 12 | |
| [Tree](Tree/) | ■ | 11 | |
| [Graph](Graph/) | ■ | 20 | `Shortest_Path` `Connectivity` `Flow_And_Matching` |
| [Geometry](Geometry/) | ■ | 6 | |
| [Math](Math/) | ■ | 31 | `Number_Theory` `Combinatorics` `Polynomial` `Linear_Algebra` `Diophantine` `Calculus` |
| [Game_Theory](Game_Theory/) | ■ | 5 | |
| [Misc](Misc/) | ■ | 6 | |

Per-file listings appear here as each directory is normalized.

### Range_Query

| File | | Use when |
| --- | --- | --- |
| [Segment_Tree.cpp](Range_Query/Segment_Tree/Segment_Tree.cpp) | **Main** | range sum/min/max/gcd with point updates |
| [Segment_Tree_Lazy.cpp](Range_Query/Segment_Tree/Segment_Tree_Lazy.cpp) | **Main** | updates hit whole ranges, not points |
| [Segment_Tree_Iterative.cpp](Range_Query/Segment_Tree/Segment_Tree_Iterative.cpp) | variant | shortest and fastest; point updates only |
| [Segment_Tree_2D.cpp](Range_Query/Segment_Tree/Segment_Tree_2D.cpp) | variant | rectangle query on a grid up to ~500×500 |
| [Segment_Tree_Dynamic.cpp](Range_Query/Segment_Tree/Segment_Tree_Dynamic.cpp) | variant | indices reach 1e9, cannot compress |
| [Segment_Tree_Dynamic_2D.cpp](Range_Query/Segment_Tree/Segment_Tree_Dynamic_2D.cpp) | variant | sparse points on a 1e9 × 1e9 space |
| [Segment_Tree_Persistent.cpp](Range_Query/Segment_Tree/Segment_Tree_Persistent.cpp) | variant | read the array as it was k updates ago |
| [Segment_Tree_Lazy_Persistent.cpp](Range_Query/Segment_Tree/Segment_Tree_Lazy_Persistent.cpp) | variant | the same, with range updates and rollback |
| [Segment_Tree_Beats.cpp](Range_Query/Segment_Tree/Segment_Tree_Beats.cpp) | variant | range `chmin`/`chmax` alongside sum queries |
| [Segment_Tree_Merging.cpp](Range_Query/Segment_Tree/Segment_Tree_Merging.cpp) | variant | one tree per vertex, merged upward — subtree multisets |
| [Fenwick_Tree.cpp](Range_Query/Fenwick_Tree.cpp) | | prefix/range sums, inversions, prefix-sum search |
| [Fenwick_Tree_Range.cpp](Range_Query/Fenwick_Tree_Range.cpp) | variant | range add **and** range sum together |
| [Prefix_Sum_2D.cpp](Range_Query/Prefix_Sum_2D.cpp) | | rectangle sums on a **fixed** grid, O(1) each |
| [Count_Inversions.cpp](Range_Query/Count_Inversions.cpp) | | pairs out of order, or minimum adjacent swaps to sort |
| [Sparse_Table.cpp](Range_Query/Sparse_Table.cpp) | | RMQ in O(1) on a static array |
| [Sparse_Table_2D.cpp](Range_Query/Sparse_Table_2D.cpp) | | max over a submatrix of a static grid |
| [MO.cpp](Range_Query/MO.cpp) | | many offline `[l, r]` queries that cannot merge |
| [MO_on_Tree.cpp](Range_Query/MO_on_Tree.cpp) | | the same, but over tree paths `u..v` |
| [Merge_Sort_Tree.cpp](Range_Query/Merge_Sort_Tree.cpp) | | count elements in `[l, r]` above a threshold |
| [Wavelet_Tree.cpp](Range_Query/Wavelet_Tree.cpp) | | k-th smallest / rank / conditional sum on a range |
| [SQRT_Decomposition.cpp](Range_Query/SQRT_Decomposition.cpp) | | the operation a segment tree cannot merge |
| [Dynamic_Connectivity.cpp](Range_Query/Dynamic_Connectivity.cpp) | | edges appear and vanish; connectivity over time |

`Segment_Tree_Beats.cpp` is the one file using **half-open** `[l, r)` ranges. Everything else is inclusive.

### Strings

| File | | Use when |
| --- | --- | --- |
| [Hashing/Hash.cpp](Strings/Hashing/Hash.cpp) | **Main** | compare any two substrings; random base defeats anti-hash tests |
| [Hashing/Hash_Segment_Tree.cpp](Strings/Hashing/Hash_Segment_Tree.cpp) | variant | the string changes by single characters |
| [Hashing/Hash_Lazy_Segment_Tree.cpp](Strings/Hashing/Hash_Lazy_Segment_Tree.cpp) | variant | whole ranges get set to one letter |
| [Hashing/Hash_Deque.cpp](Strings/Hashing/Hash_Deque.cpp) | variant | a window that grows and shrinks at both ends |
| [Hashing/Hash_2D.cpp](Strings/Hashing/Hash_2D.cpp) | variant | rectangles of a grid instead of substrings |
| [Hashing/XOR_Hashing.cpp](Strings/Hashing/XOR_Hashing.cpp) | | compare *sets* of values, ignoring order and multiplicity |
| [Aho_Corasick/Aho_Corasick.cpp](Strings/Aho_Corasick/Aho_Corasick.cpp) | **Main** | match many patterns against one text in a single pass |
| [Aho_Corasick/Aho_Corasick_Dynamic.cpp](Strings/Aho_Corasick/Aho_Corasick_Dynamic.cpp) | variant | the pattern set changes between queries |
| [KMP.cpp](Strings/KMP.cpp) | | one pattern in a text, borders, periodicity |
| [Z_Function.cpp](Strings/Z_Function.cpp) | | same, via longest common prefix with each suffix |
| [Suffix_Array.cpp](Strings/Suffix_Array.cpp) | | count a pattern, distinct substrings, longest repeated/common |
| [Suffix_Array_Integers.cpp](Strings/Suffix_Array_Integers.cpp) | variant | the alphabet is integers, not characters |
| [Manacher.cpp](Strings/Manacher.cpp) | | longest palindrome, or "is `s[l..r]` a palindrome" in O(1) |
| [Palindromic_Tree.cpp](Strings/Palindromic_Tree.cpp) | | count distinct palindromic substrings and their occurrences |
| [Trie.cpp](Strings/Trie.cpp) | | prefix counting, dictionary lookups, autocomplete |
| [Expression_Parsing.cpp](Strings/Expression_Parsing.cpp) | ⚠ | the input **is** a formula: evaluate it, or convert to postfix |
| [Booth.cpp](Strings/Booth.cpp) | | lexicographically smallest rotation |

### Data_Structure

| File | | Use when |
| --- | --- | --- |
| [DSU.cpp](Data_Structure/DSU.cpp) | **Main** | connectivity, Kruskal, "how many components", "how big is mine" |
| [DSU_Weighted.cpp](Data_Structure/DSU_Weighted.cpp) | variant | online "a[v] - a[u] = w" constraints; parity / bipartite |
| [DSU_Rollback.cpp](Data_Structure/DSU_Rollback.cpp) | variant | "try this edge, then take it back" — no path compression |
| [MEX.cpp](Data_Structure/MEX.cpp) | **Main** | smallest absent number; Sprague-Grundy values |
| [MEX_With_Updates.cpp](Data_Structure/MEX_With_Updates.cpp) | variant | `a[i] = v` then report the mex, repeatedly |
| [MEX_Range_Query.cpp](Data_Structure/MEX_Range_Query.cpp) | variant | mex of `[l, r]`, static array, queries known up front |
| [MEX_All_Subarrays.cpp](Data_Structure/MEX_All_Subarrays.cpp) | variant | smallest positive integer that is no subarray's mex |
| [Treap.cpp](Data_Structure/Treap.cpp) | **Main** | sorted multiset that also answers k-th and rank |
| [Treap_Implicit.cpp](Data_Structure/Treap_Implicit.cpp) | variant | insert/erase mid-array, range reverse, cyclic shift |
| [Hash_Table.cpp](Data_Structure/Hash_Table.cpp) | | a hash map anti-hash tests cannot blow up; pb_ds fast map |
| [Ordered_Set.cpp](Data_Structure/Ordered_Set.cpp) | | rank queries with insert/erase — reach for this before a treap |
| [Binary_Trie.cpp](Data_Structure/Binary_Trie.cpp) | | max XOR of a pair; XOR queries over a changing set |
| [Monotonic_Queue.cpp](Data_Structure/Monotonic_Queue.cpp) | | sliding-window maximum |
| [Monotonic_Stack.cpp](Data_Structure/Monotonic_Stack.cpp) | | next greater element, largest rectangle in a histogram |
| [Interval_Set.cpp](Data_Structure/Interval_Set.cpp) | ⚠ | "set a[l..r] all to v" repeatedly; range add too; runs collapse |
| [Cartesian_Tree.cpp](Data_Structure/Cartesian_Tree.cpp) | | range minimum as an LCA; divide and conquer over minima |

### Tree

| File | | Use when |
| --- | --- | --- |
| [LCA.cpp](Tree/LCA.cpp) | **Main** | LCA, distance `u..v`, k-th ancestor on a rooted tree |
| [Euler_Tour.cpp](Tree/Euler_Tour.cpp) | | subtree becomes one contiguous range — subtree queries only |
| [HLD.cpp](Tree/HLD.cpp) | | path queries `u..v` with point updates; values on nodes *or* edges |
| [LCT.cpp](Tree/LCT.cpp) | | edges appear and vanish **online** and you still need path sums |
| [Centroid_Decomposition.cpp](Tree/Centroid_Decomposition.cpp) | | counting over all paths — "paths of length k", nearest marked node |
| [DSU_on_Tree.cpp](Tree/DSU_on_Tree.cpp) | **Main** | per-subtree "distinct colours" / "most frequent value" |
| [Small_To_Large.cpp](Tree/Small_To_Large.cpp) | variant | the same, when you want the shortest code that works |
| [Diameter.cpp](Tree/Diameter.cpp) | | longest path, tree centre or radius |
| [Tree_Isomorphism.cpp](Tree/Tree_Isomorphism.cpp) | | "same shape?", counting distinct subtree shapes |
| [Virtual_Tree.cpp](Tree/Virtual_Tree.cpp) | | many queries each marking k nodes, with sum of k bounded |
| [Rerooting.cpp](Tree/Rerooting.cpp) | ⚠ | "for each node, the answer if the tree were rooted there" — one pass, not n |

`HLD.cpp` covers paths, `Euler_Tour.cpp` covers subtrees. Reach for `LCT.cpp` only when the
tree's shape actually changes — it is the heaviest file here.

### DP

| File | | Use when |
| --- | --- | --- |
| [CHT.cpp](DP/CHT.cpp) | **Main** | `dp[i] = max(dp[j] + a[j]*x[i])`, best over many lines |
| [CHT_Monotonic.cpp](DP/CHT_Monotonic.cpp) | variant | same, but slopes sorted and queries move one way — O(1) |
| [Li_Chao_Tree.cpp](DP/Li_Chao_Tree.cpp) | variant | a line is valid only on a sub-range of x |
| [Divide_and_Conquer.cpp](DP/Divide_and_Conquer.cpp) | | split into exactly k consecutive groups, min cost |
| [Knuth_Optimization.cpp](DP/Knuth_Optimization.cpp) | | interval DP merging adjacent ranges, O(n³) too slow |
| [Deque_Trick.cpp](DP/Deque_Trick.cpp) | | window min/max inside a DP transition |
| [SOS.cpp](DP/SOS.cpp) | | for each mask, aggregate over its subsets or supersets |
| [Digit_DP.cpp](DP/Digit_DP.cpp) | | count numbers in `[l, r]` by a property of their digits |
| [Slope_Trick.cpp](DP/Slope_Trick.cpp) | | minimum total `\|change\|` to make an array non-decreasing |
| [Weighted_Job_Scheduling.cpp](DP/Weighted_Job_Scheduling.cpp) | | pick non-overlapping intervals, maximise value |
| [Meet_in_the_Middle.cpp](DP/Meet_in_the_Middle.cpp) | | n ≈ 40 and the values are too large to knapsack |

### Misc

| File | | Use when |
| --- | --- | --- |
| [Ternary_Search.cpp](Misc/Ternary_Search.cpp) | | minimise/maximise a unimodal function |
| [Huffman_Coding.cpp](Misc/Huffman_Coding.cpp) | | shortest prefix-free code; optimal merge order |
| [Josephus.cpp](Misc/Josephus.cpp) | | every k-th person leaves the circle |
| [Majority_Element.cpp](Misc/Majority_Element.cpp) | | a value occupying more than n/2, or n/k |
| [Coordinate_Compression.cpp](Misc/Coordinate_Compression.cpp) | | values reach 1e9 but only 1e5 are distinct |
| [Logarithmic_Subarray_Aggregator.cpp](Misc/Logarithmic_Subarray_Aggregator.cpp) | | "how many subarrays have gcd / AND / OR exactly x" |

### Game_Theory

| File | | Use when |
| --- | --- | --- |
| [Grundy.cpp](Game_Theory/Grundy.cpp) | **Main** | "who wins with perfect play"; Nim, misere Nim, sums of games |
| [Games_On_Graph.cpp](Game_Theory/Games_On_Graph.cpp) | | positions can repeat, so a DRAW is possible |
| [Wythoff_Game.cpp](Game_Theory/Wythoff_Game.cpp) | | two piles, take from one or equally from both |
| [Remoteness.cpp](Game_Theory/Remoteness.cpp) | | win in the fewest moves, or survive the longest |
| [Hackenbush.cpp](Game_Theory/Hackenbush.cpp) | | cut edges off a grounded graph; anything detached falls off |

### Math

All of `Math/` is done.

| File | | Use when |
| --- | --- | --- |
| [Number_Theory/Mod_Arithmetic.cpp](Math/Number_Theory/Mod_Arithmetic.cpp) | **Main** | anything "modulo 1e9+7"; inverses, division, a number too big to read |
| [Number_Theory/Modular_Int.cpp](Math/Number_Theory/Modular_Int.cpp) | **Main** | a type that carries the modulus, so `+ - * /` are already reduced |
| [Number_Theory/Exponentiation.cpp](Math/Number_Theory/Exponentiation.cpp) | | `a^n mod m`, matrix power for a recurrence, `k` shuffles of a permutation |
| [Number_Theory/Extended_GCD.cpp](Math/Number_Theory/Extended_GCD.cpp) | | an inverse under a **composite** modulus; is `ax + by = c` solvable |
| [Number_Theory/CRT.cpp](Math/Number_Theory/CRT.cpp) | | combine `x = r_i (mod m_i)`; handles non-coprime moduli |
| [Number_Theory/Sieve.cpp](Math/Number_Theory/Sieve.cpp) | **Main** | primes up to n, then factorize many numbers in O(log n) each |
| [Number_Theory/Divisors_And_Factors.cpp](Math/Number_Theory/Divisors_And_Factors.cpp) | | factorize **one** number near 1e18; divisor counts and sums over a range |
| [Number_Theory/Miller_Rabin_Pollard_Rho.cpp](Math/Number_Theory/Miller_Rabin_Pollard_Rho.cpp) | | primality and factorization up to **9e18**, where trial division dies |
| [Number_Theory/Euler_Phi.cpp](Math/Number_Theory/Euler_Phi.cpp) | | count coprimes, shrink an exponent by Euler's theorem |
| [Number_Theory/Fibonacci.cpp](Math/Number_Theory/Fibonacci.cpp) | | `F(n) mod m` at huge n, or any two-term linear recurrence |
| [Number_Theory/Discrete_Log.cpp](Math/Number_Theory/Discrete_Log.cpp) | | smallest `x` with `a^x = b (mod m)`; works when `a`, `m` share a factor |
| [Number_Theory/Base_Conversion.cpp](Math/Number_Theory/Base_Conversion.cpp) | | write n in base b, or read a base-b token |
| [Polynomial/FFT.cpp](Math/Polynomial/FFT.cpp) | **Main** | convolution with **no** modulus; "how many pairs sum to k" |
| [Polynomial/NTT.cpp](Math/Polynomial/NTT.cpp) | **Main** | the same, exact, mod 998244353; also polynomial powers |
| [Polynomial/NTT_Any_Mod.cpp](Math/Polynomial/NTT_Any_Mod.cpp) | variant | any modulus (1e9+7), via three NTTs and CRT |
| [Polynomial/FWHT.cpp](Math/Polynomial/FWHT.cpp) | | indices combine by **XOR / AND / OR**, not addition |
| [Combinatorics/Binomial_Coefficients.cpp](Math/Combinatorics/Binomial_Coefficients.cpp) | **Main** | nCr / nPr mod a prime, stars and bars, derangements, Lucas |
| [Combinatorics/Binomial_Coefficients_Any_Mod.cpp](Math/Combinatorics/Binomial_Coefficients_Any_Mod.cpp) | variant | the modulus is **not** prime, so factorials will not invert |
| [Combinatorics/Catalan.cpp](Math/Combinatorics/Catalan.cpp) | | bracket sequences, binary trees, triangulations |
| [Polynomial/Lagrange_Interpolation.cpp](Math/Polynomial/Lagrange_Interpolation.cpp) | | "sum of i^k up to a huge n"; any DP known to be polynomial |
| [Combinatorics/Permutations.cpp](Math/Combinatorics/Permutations.cpp) | | k shuffles, cycle decomposition, order, sign |
| [Diophantine/LDE.cpp](Math/Diophantine/LDE.cpp) | **Main** | `ax + by = c`: one solution, all of them, or how many in a box |
| [Diophantine/LDE_N_Variables.cpp](Math/Diophantine/LDE_N_Variables.cpp) | variant | more than two coefficients, any integer solution |
| [Diophantine/LDE_N_Variables_Non_Negative.cpp](Math/Diophantine/LDE_N_Variables_Non_Negative.cpp) | variant | coin values: which totals are reachable, and how many |
| [Linear_Algebra/Gaussian_Elimination.cpp](Math/Linear_Algebra/Gaussian_Elimination.cpp) | | solve a linear system; rank, determinant, and GF(2) systems |
| [Linear_Algebra/XOR_Basis.cpp](Math/Linear_Algebra/XOR_Basis.cpp) | **Main** | largest XOR of a subset, reachability, k-th smallest XOR |
| [Linear_Algebra/XOR_Basis_Range.cpp](Math/Linear_Algebra/XOR_Basis_Range.cpp) | variant | the same, restricted to a suffix `a[l..]` |
| [Calculus/Integration.cpp](Math/Calculus/Integration.cpp) | | area under a curve; adaptive where it bends sharply |
| [Calculus/Differentiation.cpp](Math/Calculus/Differentiation.cpp) | | numerical slope, gradient, Newton's method |
| [Big_Integer.cpp](Math/Big_Integer.cpp) | | values past 64 bits: exact arithmetic, FFT multiplication |
| [Gray_Code.cpp](Math/Gray_Code.cpp) | | enumerate subsets changing one element at a time |

Reach for `Sieve.cpp` when you factorize **many** numbers and `Divisors_And_Factors.cpp` for **one**
big one — that is the only thing separating them. `Mod_Arithmetic.cpp` is safe for a modulus up to
9e18; most files that hand-roll `(a * b) % m` are not.

For convolution, pick by modulus: none → `FFT.cpp`, 998244353 → `NTT.cpp`, anything else →
`NTT_Any_Mod.cpp` (three times the work). `FFT.cpp` carries `rounding_margin()` — call it once on
real input to confirm the double precision is holding before you trust the answer.

### Geometry

Two point representations, on purpose. `complex<double>` for the analytic work, and a plain
`struct Point<ll>` for the exact work — because the standard has no `complex<long long>`, so
integer-exact geometry cannot use `complex` at all.

| File | Point type | Use when |
| --- | --- | --- |
| [Geometry.cpp](Geometry/Geometry.cpp) | `complex<double>` | **Main** — rotate or reflect, where two lines meet, distance to a segment, an angle |
| [Circle.cpp](Geometry/Circle.cpp) | `complex<double>` | circles: intersections, overlap area, circumcircle, smallest enclosing circle |
| [Convex_Hull.cpp](Geometry/Convex_Hull.cpp) | `struct Point<ll>` | smallest fence around all points; also exact `polar_sort` |
| [Polygon.cpp](Geometry/Polygon.cpp) | `struct Point<ll>` | area, point-in-polygon, is it convex, lattice-point counts (Pick's) |
| [Closest_Pair.cpp](Geometry/Closest_Pair.cpp) | `struct Point<ll>` | the two nearest points; exact, squared distance |
| [Rectangle_Union.cpp](Geometry/Rectangle_Union.cpp) | `ll` corners | total area covered by overlapping axis-aligned rectangles |

Reach for the exact pair whenever the input is integers: they use no EPS at all, so collinearity
is `cross == 0` rather than a tuned tolerance. `cut_polygon` lives in `Geometry.cpp`, not
`Polygon.cpp`, because cutting integer coordinates produces fractional vertices.
See [notes/Geometry.md](notes/Geometry.md) for what each `complex<double>` function does — in
particular `norm` is the length *squared*, which is the usual misread.

### Graph

| File | | Use when |
| --- | --- | --- |
| [Shortest_Path/Dijkstra.cpp](Graph/Shortest_Path/Dijkstra.cpp) | **Main** | cheapest route, non-negative weights; multi-source, path recovery |
| [Shortest_Path/Bellman_Ford.cpp](Graph/Shortest_Path/Bellman_Ford.cpp) | **Main** | weights can be negative; names every node a negative cycle affects |
| [Shortest_Path/Bellman_Ford_SPFA.cpp](Graph/Shortest_Path/Bellman_Ford_SPFA.cpp) | variant | the same, queue-driven — far faster on typical graphs |
| [Shortest_Path/Floyd.cpp](Graph/Shortest_Path/Floyd.cpp) | | all pairs, n up to a few hundred; paths, later edge additions |
| [Depth_First_Search.cpp](Graph/Depth_First_Search.cpp) | **Main** | components, cycle check, bipartite check, topological order |
| [Breadth_First_Search.cpp](Graph/Breadth_First_Search.cpp) | **Main** | fewest moves; **0/1 weights** via deque, multi-source, grids |
| [Connectivity/Strongly_Connected_Components.cpp](Graph/Connectivity/Strongly_Connected_Components.cpp) | | collapse a directed graph's cycles; gives the condensation DAG |
| [Connectivity/Bridges.cpp](Graph/Connectivity/Bridges.cpp) | **Main** | critical **edges**, two-edge-connected components, the bridge tree |
| [Connectivity/Bridges_Online.cpp](Graph/Connectivity/Bridges_Online.cpp) | variant | edges arrive as queries and each asks for the bridge count |
| [Connectivity/Articulation_Points.cpp](Graph/Connectivity/Articulation_Points.cpp) | | critical **vertices**, and how many pieces removing one leaves |
| [MST_Kruskal.cpp](Graph/MST_Kruskal.cpp) | | connect everything as cheaply as possible; also maximum spanning |
| [Kruskal_Reconstruction_Tree.cpp](Graph/Kruskal_Reconstruction_Tree.cpp) | | "reachable using edges <= w"; minimax edge on a path |
| [2_SAT.cpp](Graph/2_SAT.cpp) | | every constraint pairs two things with two states each |
| [Strong_Orientation.cpp](Graph/Strong_Orientation.cpp) | | make every edge one-way with the fewest components forced |
| [Flow_And_Matching/Dinic.cpp](Graph/Flow_And_Matching/Dinic.cpp) | **Main** | maximum flow, minimum cut, disjoint paths |
| [Flow_And_Matching/Minimum_Cost_Maximum_Flow.cpp](Graph/Flow_And_Matching/Minimum_Cost_Maximum_Flow.cpp) | variant | each unit shipped has a price; stop at k units |
| [Flow_And_Matching/Bipartite_Matching.cpp](Graph/Flow_And_Matching/Bipartite_Matching.cpp) | **Main** | most pairs with no shared endpoint; min cover, max independent set |
| [Flow_And_Matching/Bipartite_Matching_Hopcroft_Karp.cpp](Graph/Flow_And_Matching/Bipartite_Matching_Hopcroft_Karp.cpp) | variant | the same answer, `O(E√V)` — only when the size demands it |
| [Flow_And_Matching/Hungarian.cpp](Graph/Flow_And_Matching/Hungarian.cpp) | | a full cost matrix: cheapest assignment of workers to jobs |

`Depth_First_Search.cpp` sits at the top of `Graph/`, not in `Connectivity/` — it answers more than
connectivity. Reach for `Bridges.cpp` for critical edges and `Articulation_Points.cpp` for critical
vertices; the two are easy to confuse and solve different questions.

## Verification

[VERIFICATION.md](VERIFICATION.md) lists every template and what actually backs it. 24 of 149
carry a judge verdict: `tools/verify.py` embeds the template byte for byte, downloads a judge's
full system tests, and runs them against that judge's own checker. Everything else stands on
brute-force agreement, which finds wrong answers but cannot establish a time bound.

Two results worth knowing before you grab a file:

- **`Bipartite_Matching.cpp` (Kuhn) times out on adversarial input.** 41 of 44 Library Checker
  cases pass; `line_00`, `line_01` and `issue1068_large_02` exceed 15 s. Those cases exist
  specifically to kill Kuhn. `Bipartite_Matching_Hopcroft_Karp.cpp` clears all 44 in 0.97 s.
  Use Kuhn when the graph is small or friendly; reach for Hopcroft-Karp when it is neither.
- **`Sieve.cpp`'s `primes_up_to` is correct but slow at the top of its range.** All 10 cases of
  `enumerate_primes` are right, the slowest taking 12 s against a 10 s limit at N = 5e8.

```
py tools/verify.py            # run every judge entry
py tools/verify.py --only DSU # just one
py tools/checklist.py         # regenerate VERIFICATION.md
```

⚠ `Interval_Set.cpp`, `Expression_Parsing.cpp` and `Rerooting.cpp` are unproven: tier-5 sources,
verified locally against brute-force references but not yet by a judge. Ready-to-submit files sit in
`verify/`. `Rerooting.cpp` is the strongest case of the three - no rerooting implementation exists in
`other/` or in any of the four reference libraries, so it was written from the standard formulation.

`Slope_Trick.cpp` was in exactly that position — no source in this codebase or the reference
libraries has it either — and is the worked example of how a flag comes off. It was submitted to a
judge to prove it: **CSES 2132 "Increasing Array II", ACCEPTED on 2026-08-17**, all 8 tests, slowest 0.07 s.
The submitted file embeds the template byte for byte; it is kept in `verify/Slope_Trick/`.

## Layout

- `other/` — frozen archive of the team's older templates. Read it, never edit it.
- `notes/` — prose, formula sheets, PDFs.
- `tools/` — the checker.
