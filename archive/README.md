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
| Hashing_2.cpp | 06_Strings | Single-mod duplicate of Hashing_Dynamic; pop needs the caller to pass the right char |
| Hashing_3.cpp | 06_Strings | Duplicate of Hashing_Dynamic; fixed 1e5 power table read out of bounds past 1e5 pushes |
| Hashing_Prefix.cpp | 06_Strings | Single-mod duplicate of Hashing.cpp; 'A'-based values collide on lowercase ("Ba" == "CB"); crashed on an empty string |
| Binary_Trie_2.cpp | 01_Data_Structures | Same algorithm and API as Binary_Trie; its features (search, count, count_greater, template T/LOG) merged into Binary_Trie.cpp |
| Convex_Hull_Complex.cpp | 09_Geometry | Duplicate float hull (global pivot); crashed on empty input; Convex_Hull + Convex_Hull_2 cover it |
| KMP_2.cpp | 06_Strings | Same KMP struct; its extra features (build_non_overlapping, countInAnother) merged into KMP.cpp |
| LDE.cpp | 07_Math | Identical to Extended_GCD.cpp (same extended_gcd + find_any_solution) |
| KMP_2.cpp | 06_Strings | Same KMP class as KMP.cpp; every feature (match, borders, prefix counts, non-overlapping borders) merged into KMP.cpp |
| LDE.cpp | 07_Math | find_any_solution / extended_gcd are also in LDE_All_Solutions.cpp |
| Convex_Hull_Complex.cpp | 09_Geometry | Second floating-point hull; Convex_Hull.cpp (complex, monotone chain) and Convex_Hull_2.cpp (exact integers) cover it |

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
| MEX | normalize version | Did not compile (two `mex` definitions); the array version silently ignored values > 1e5 |
| Treap | normalize version | Did not compile (`sp`, `nl`); destructor freed the shared EMPTY sentinel, so a second treap of the same type crashed |
| MO_On_Tree_Edges, MO_On_Tree_Nodes | one MO_On_Tree.cpp (normalize version) | Edges did not compile (undefined `MEX`); add/remove were empty; the two were the same class. New file does both (`MoTree<int, true>` for edges) |
| Digit_DP | normalize version as `digit_dp(l, r, k)` | Included a missing `../../core.h`; `solve()` never called `rec()` |
| Trie | normalize version | `sz` macro; erasing "ab" after inserting "ab" and "abc" left "ab" searchable |
| SPFA | normalize version | `#define ll long long int` broke under `#define int long long`; `neg_cycle` never reset. Now 1-based, and the 4th `add_edge` argument means bidirectional |
| Hashing_Dynamic | own file + trivial fixes | Fixed 1e6 power table (out of bounds past 1e6); constexpr precompute writing globals. Now `Dynamic_Hashing h;` (no <>), merge static |
| Hashing_2D | own file + trivial fixes | No logic bug; macros/I-O removed; cell values must span < 2999 |
| Hashing_Lazy_Segment_Tree | own file + trivial fixes (+ equal() from normalize) | n = 0 recursed forever; update() took a raw value while build() mapped chars; 32 MB global tables. update() now takes a char |
| Binary_Trie | merge of Binary_Trie_2 code + Binary_Trie | _2: cnt uninitialised; min_xor missed bit LOG; count wrong for k<0. Binary_Trie: erase of absent value null deref; constructor silently inserted 0 (now insert(0) yourself) |
| Cartesian_Tree | own algorithm as a struct | No bug; globals and problem dfs removed |
| MEX_All_Subarrays | own algorithm (CF 1436E) | Fixed N=1e5 arrays overflowed for larger values; dead `set seg` removed |
| MEX_Range_Query | own algorithm | Values >= n+1 or negative wrote out of bounds (a={5,0,1}); now 0-based queries; SegmentTree renamed MexSegTree |
| Monotonic_Stack | own prev_smaller + 3 directions from normalize | No bug; now returns indices |
| Z_Function | own + fixes | Did not compile (max(int, long long)); minString tested == instead of >= (minString("ab","aba") threw out_of_range) |
| Palindromic_Tree | own, main removed, build() added | No bug |
| XOR_Hashing | normalize struct form | Was a whole program |
| Suffix_Array_Integers | own + trivial fixes | No bug; values must be >= 1; same struct name as Suffix_Array |
| Suffix_Array | own + fixes | compare(i,j,l) compared the wrong ranks ("abab" compare(0,2,2) gave 1); longest_common_substring skipped the last pair (lcs("z","z") == "") |
| Aho_Corasick | own + trivial fixes | No bug; unused global removed |
| Hackenbush | normalize version | XORed every self loop into the global answer: edges 1-2 + loop at 2 gave Bob (true value 2, Alice) |
| Remoteness_Suspense | own + fixes, as a struct | remoteness() returned 0 for losing non-terminal positions; suspense() took max instead of min on losing side |
| Super_Win_Lose | own, as a struct | No bug |
| Wavelet_Tree | own + fixes | int prefix sums overflowed (a={1e9,1e9,1e9}, sum(1,3,1e9)); mid=(lo+hi)>>1 overflowed on wide ranges; malloc never freed. sum() now ll |
| Segment_Tree_Beats | own | No logic bug; ~64 MB of fixed 4*N arrays inside the struct (stack overflow if local, n capped at 2e5). Now SGTBeats(vector<ll>), [l, r) |
| Segment_Tree_Dynamic_2D | own | Default constructor left root null (crash on update) |
| Sparse_Table_2D | own, restructured | No bug; fixed 505x505 global (~102 MB) and problem code. Now SparseTable2D<T>(grid), 0-based, max only tested |
| SQRT_Decomposition | own as struct | No bug. Now SqrtDecomp(vector<ll>), 0-based inclusive |
| Link_Cut_Tree | own + 2 fixes | Path-add lazy also added to virtual subtrees (link 1-2, 2-3, add +1 on path 1..2: component_sum(1) = 3, want 2); access() moved an un-pushed child's subsum. Same code as Shohag's and normalize's copies |
| Dynamic_Connectivity | own as a struct | Union by rank bumped the child and saved undo after the change: a 1e5-edge star took 7.3 s (depth 99999); now 0.05 s at 3e5 |
| 2_SAT | own | ok() used resize, so a second call after new clauses returned the old answer |
| Hopcroft_Karp | own | Second maximum_matching() call returned only the extra pairs |
| Diameter | own | No bug; diameter() now returns {len, a, b} |
| DSU_On_Tree_2 | own as Small_To_Large struct | No bug |
| Euler_Tour | own (tour only) | No bug; the bundled segment tree was dropped (02_Range_Query covers it); endd -> finish |
| Strong_Orientation | own as a struct | No bug; static timer and globals removed; 0-based |
| Big_Int | own + fixes | (-7)/2 gave q=-3 r=1 (q*b+r != a); gcd(-4,-6) hung; 5 + (-5) gave -0; * and / took int (BigInt(1)*1e10 wrong); % (ll) wrong past INT_MAX; FFT multiply dropped the top carry (~9000+ digits); shift UB; BigInt(LLONG_MIN) overflow |
| Differentiation | own | Second derivative used h=1e-6: f=x^3 at x=1000 gave 0 (want 6000) |
| Integration | own | No bug |
| Divisors_And_Factors | own + one-line fixes | prime_factorization returned vector<int> (2*4294967311 -> {2, 15}); int i*i overflow near INT_MAX; get_sum_div printed debug lines |
| FFT | own + precomputed roots (from main's FFT_Mod) | w *= wlen roots lost precision: 2^17 x 2^17 with values <= 1e5 already wrong; result now trimmed. ~1.6x slower (long double) |
| NTT | own | int products overflowed without the file's #define int long long; poly_pow_mod returned trailing zeros; generator(2) returned 2 |
| NTT_Any_Mod | own as struct, modulus a parameter | Negative inputs gave negative results; result not trimmed |
| NTT_Large | own wrapped in a struct | No bug |
| LDE_N_Vars | own + one-line guard | A zero suffix gcd crashed (find_any_solution({2,0}, 4) -> SIGFPE) |
| LDE_N_Vars_Non_Negative | own | inf = 1e18 collided with real k (a={2}: can(1e18+1) true); can(k<0) read out of bounds |
| CHT | own + 2 fixes | Three equal slopes deleted the lowest line (query_bs(-5) -11, want -17); ptr went stale if the hull shrank then grew between queries. Added query_bs |
| Li_Chao_Tree | own + fix | query_segment swapped node and query ranges (wrong on ~10% of checks) - removed; default constructor left root null |
| Weighted_Job_Scheduling | own + fixes, as max_profit(jobs) | n=0 read garbage; job 0 was forced (jobs (5,9,-7),(1,2,-1) gave -1, want 0); touching intervals clashed depending on sign |
| Knuth_Optimization_Stick_Cutting | own + fix | The 2-line Knuth block indexed opt[i][-1]; fixed loop now active. stick_cutting(len, cuts) |
| Bitmask_DP_Elevator_Rides | own | No bug (8 MB stack array -> vector). elevator_rides(w, x) |
| Deque_Trick | own | Negative index when k > n; 200 MB stack array. max_pick_every_k(a, k, x) |
| DnC_Optimization | own, generic | No bug. dnc_dp(n, k, cost) |
| Knuth_Optimization | own, generic | No bug. knuth(n, cost) |
| SOS | own | No bug. sum_over_subsets / sum_over_supersets |
| Logarithmic_Subarray_Aggregator | own, generic op | No bug. subarray_counts(a, op) for gcd/AND/OR, ll counts |
| Meet_In_The_Middle | own | No bug. subset_sums / count_subsets_with_sum, ll elements |
| Permutation_Cycles | own + fixes | Order kept in int (cycles 23..47 gave -1031784251, want 63392725189); power(p, k) overflowed j + k and returned nothing for k < 0 |
| Wythoff | own + exact floor_k_phi from normalize | double k*phi wrong from k = F40 (is_wythoff_losing(165580140, 267914295) false); int overflow past 1.3e9; is_valid_wythoff_move accepted negative targets |
| Convex_Hull_2 | own + small fixes | All-identical points returned twice; n <= 2 returned the raw input |
| Line_Sweep_Rectangles | own algorithm as Rect_Union::area | Empty input crashed; all zero-height recursed forever; int overflow past 2^31; MX = 1<<17 overflowed at 70000 rectangles |
| Ordered_Set | own + trivial fixes | The macro named ordered_set was really a less_equal multiset (find/erase silently fail, lower/upper_bound swapped). Now ordered_set / ordered_multiset aliases + multiset_erase; pb_ds included safely under #define int long long |
| Inversions | own + one fix | o_set<int> dropped duplicates: [2,1,1] gave 1 (want 2). Now count_inversions(a) with (value, index) keys |
| Aho_Corasick_Dynamic | own + trivial fixes | No bug; delete_word rebuilds the whole bucket (slow) - Note gives the two-automaton workaround |
| MEX_With_Updates | own + 2 fixes | class made the constructor private (unusable); update put negative old values back: Mex m({-1}); update(0,5); mex() = -1 |
| Segment_Tree_2D | own + init-order fix | Default declared after t, which read it uninitialised |
| Monotonic_Queue | own, vectors | Fixed 3e5 arrays overflowed after 3e5 total pushes |
| MO | normalize struct | Globals, fixed vis[1e6]; shrank left before growing (negative counts break most add/remove) |
| Merge_Sort_Tree | own, cleaned | No logic bug; update is O(n), not O(log^2 n) as claimed |
| DSU_Rollback | own, solve() removed | No bug |
| Max_Flow_Dinic | own + fix | flow_through was vector<int>: per-edge flow > 2^31 truncated (capacity 4e12 reported wrong) |
| Min_Cost_Max_Flow | own + guard | No wrong answers; Bellman-Ford relaxed from unreachable nodes; header wrongly said undirected + negative costs work together |
| Hungarian | own + fixes | Clearing an unmatched row left r[] pointing back (1x2 / 2x1 sparse cases wrong); fixed [505][505] arrays (2 MB, n > 504 overflowed) |
| Online_Bridges | own + fix | make_root read the size from v, not its root: small-to-large broken (counts right, complexity not) |
| Segment_Tree | own + trivial | update took int (values > int truncated) |
| Segment_Tree_Dynamic | own + fixes | rx - lx overflowed int on default [-1e9,1e9]; sums in int overflowed |
| Games_On_Graph | own as struct | No algorithm bug; was globals only |
| Impartial_Games | own correct parts + normalize Grundy | Dropped 7 wrong functions: remoteness/suspense stubs returned 1 (conjunctive games meaningless); misere_selective wrong (single 1-pile said win); misere_disjunctive_long sum overflowed |
| All_Permutations | own, printing -> visitor | No bug |
| Bases | own + fixes | Negative input indexed with a negative; lowercase 'a' read as 42; "7FFFFFFFFFFFFFFF" overflowed |
| Bits | macros -> functions | 1 << i was 32-bit (bits >= 31 of ll wrong); popcount truncated ll; check_bit returned the mask |
| Gray_Code | own, g/rev_g -> gray/rev_gray | rev_g looped forever on negatives |
| Misc_Helpers | own + fixes | sumall overflowed past 3.04e9; sumeven(x) returned x(x+1) (now sum of evens <= x - meaning changed); get_log(1000,10) floors to 2 (added log_floor) |
| Geometry_Lib | own + 2 fixes | oriented_angle((0,0),(1,0),(0,-1)) gave pi/2 (want 3pi/2); seg_inters returned a point twice; #define X/Y broke variables named X, Y |
| Points_And_Polygons | own, header | No bug; exact ll geometry (kept next to float Geometry_Lib) |
| Convex_Hull | own, header | No bug |
| KMP | own + KMP_2 merged + fixes | build_non_overlapping wrong in every copy ("aaaa" gave 0, want 2) - REWRITTEN by the agent (NOI 2014 Zoo technique), review it; count(str) skipped str[0] and added 1 ("a" in "ba" = 2) |
| Hashing | own + fixes | {0} vs {0,0} equal; huge/negative values overflowed; free rng() clashed with mt19937 rng; both mods could be equal; Base=1 read past the end |
| Manacher | own, cleaned | No bug |
| Booth | own, headers | No bug |
| Aho_Corasick_Intervals | own + fixes | Copyable struct (double free); fail uninitialised; global counter |
| Combinatorics | own + fixes | CountLib(0) wrote out of bounds; stars_bars(0,0) returned 0 |
| Euler_Phi | own, defensive i <= n/i | No bug |
| Exponentiation | own + fixes | pow_mod(-2,1,7) = -2; pow_mod(5,0,1) = 1; base never reduced (1e12^2 overflow); bin_pow(2,62) UB |
| Extended_GCD | own, ll | find_any_solution(0,0,c) divided by zero |
| Fibonacci | own + modulus | No modulus; fib_pair in int (fib_pair(47) negative); fib(93+) overflow |
| Gaussian_Elimination | normalize version (Gauss + Gauss_XOR) | No zero-pivot check: singular systems returned NaN; could not report none/infinite |
| LDE_All_Solutions | own + zero-coefficient cases | a = 0 or b = 0 divided by zero |
| Mod_Arithmetic | macros -> functions | sub_mod(-5,3,7) = -1; base never reduced; Big_Mod overflowed for m > 9e17 |
| Centroid_Decomposition | own, cleaned | No bug |
| DSU_On_Tree | own as struct (normalize counter) | Recorded cnt[u] (count of the colour numbered u): n=1, col=5 gave 0, want 1; fixed 1e5 arrays |
| HLD | own + fixes | Did not compile (sz macro undefined); held the adjacency by reference (dangling). Added query_subtree |
| Tree_Isomorphism | normalize version | Treated index 0 as a node; n=2 popped an empty queue. Not O(n log n): a 5e4 path takes 5.6 s / 2.5 GB |
| Bipartite_Check | own as struct | Printed and called exit(0) inside the search |
| DFS | own, header | No bug |

## unverified/ (2026-11-18)

Rewrites whose tests could not be written (the writes were refused). The live templates are still main's
originals; move a file back only after it passes a brute-force test.

| File | What changed | Why it matters |
|---|---|---|
| Segment_Tree_Lazy | macros removed, `ll` values | cleanup only |
| Segment_Tree_Lazy_Iterative | pending map always combined, compared to identity | original treats `mod[p][0] == 0` as "no change": on [1..8], 3->5 then 5->7 over the whole range returns 1..8 unchanged |
| Segment_Tree_Lazy_Persistent | `cur()`, `revert()` from normalize | new methods |
| Segment_Tree_Persistent | value merge, no allocation in query, 64-bit midpoint | original `get()` does not compile; merge/query allocate nodes (467 MB at n = q = 2e5) |
| Partitions | new: p(0..n) by the pentagonal recurrence (Shohag) | no test yet |
| Aho_Corasick_Intervals | own + fixes | Copyable struct (double free); fail uninitialised; global counter |
| All_Permutations | own, printing -> visitor | No bug |
| Bases | own + fixes | Negative input indexed with a negative; lowercase 'a' read as 42; "7FFFFFFFFFFFFFFF" overflowed |
| Bipartite_Check | own as struct | Printed and called exit(0) inside the search |
| Bits | macros -> functions | 1 << i was 32-bit (bits >= 31 of ll wrong); popcount truncated ll; check_bit returned the mask |
| Booth | own, headers | No bug |
| Centroid_Decomposition | own, cleaned | No bug |
| Combinatorics | own + fixes | CountLib(0) wrote out of bounds; stars_bars(0,0) returned 0 |
| Convex_Hull | own, header | No bug |
| DFS | own, header | No bug |
| DSU_On_Tree | own as struct (normalize counter) | Recorded cnt[u] (count of the colour numbered u): n=1, col=5 gave 0, want 1; fixed 1e5 arrays |
| DSU_Rollback | own, solve() removed | No bug |
| Euler_Phi | own, defensive i <= n/i | No bug |
| Exponentiation | own + fixes | pow_mod(-2,1,7) = -2; pow_mod(5,0,1) = 1; base never reduced (1e12^2 overflow); bin_pow(2,62) UB |
| Extended_GCD | own, ll | find_any_solution(0,0,c) divided by zero |
| Fibonacci | own + modulus | No modulus; fib_pair in int (fib_pair(47) negative); fib(93+) overflow |
| Games_On_Graph | own as struct | No algorithm bug; was globals only |
| Gaussian_Elimination | normalize version (Gauss + Gauss_XOR) | No zero-pivot check: singular systems returned NaN; could not report none/infinite |
| Geometry_Lib | own + 2 fixes | oriented_angle((0,0),(1,0),(0,-1)) gave pi/2 (want 3pi/2); seg_inters returned a point twice; #define X/Y broke variables named X, Y |
| Gray_Code | own, g/rev_g -> gray/rev_gray | rev_g looped forever on negatives |
| HLD | own + fixes | Did not compile (sz macro undefined); held the adjacency by reference (dangling). Added query_subtree |
| Hashing | own + fixes | {0} vs {0,0} equal; huge/negative values overflowed; free rng() clashed with mt19937 rng; both mods could be equal; Base=1 read past the end |
| Hungarian | own + fixes | Clearing an unmatched row left r[] pointing back (1x2 / 2x1 sparse cases wrong); fixed [505][505] arrays (2 MB, n > 504 overflowed) |
| Impartial_Games | own correct parts + normalize Grundy | Dropped 7 wrong functions: remoteness/suspense stubs returned 1 (conjunctive games meaningless); misere_selective wrong (single 1-pile said win); misere_disjunctive_long sum overflowed |
| KMP | own + KMP_2 merged + fixes | build_non_overlapping wrong in every copy ("aaaa" gave 0, want 2) - REWRITTEN by the agent (NOI 2014 Zoo technique), review it; count(str) skipped str[0] and added 1 ("a" in "ba" = 2) |
| LDE_All_Solutions | own + zero-coefficient cases | a = 0 or b = 0 divided by zero |
| MEX_With_Updates | own + 2 fixes | class made the constructor private (unusable); update put negative old values back: Mex m({-1}); update(0,5); mex() = -1 |
| MO | normalize struct | Globals, fixed vis[1e6]; shrank left before growing (negative counts break most add/remove) |
| Manacher | own, cleaned | No bug |
| Max_Flow_Dinic | own + fix | flow_through was vector<int>: per-edge flow > 2^31 truncated (capacity 4e12 reported wrong) |
| Merge_Sort_Tree | own, cleaned | No logic bug; update is O(n), not O(log^2 n) as claimed |
| Min_Cost_Max_Flow | own + guard | No wrong answers; Bellman-Ford relaxed from unreachable nodes; header wrongly said undirected + negative costs work together |
| Misc_Helpers | own + fixes | sumall overflowed past 3.04e9; sumeven(x) returned x(x+1) (now sum of evens <= x - meaning changed); get_log(1000,10) floors to 2 (added log_floor) |
| Mod_Arithmetic | macros -> functions | sub_mod(-5,3,7) = -1; base never reduced; Big_Mod overflowed for m > 9e17 |
| Monotonic_Queue | own, vectors | Fixed 3e5 arrays overflowed after 3e5 total pushes |
| Online_Bridges | own + fix | make_root read the size from v, not its root: small-to-large broken (counts right, complexity not) |
| Points_And_Polygons | own, header | No bug; exact ll geometry (kept next to float Geometry_Lib) |
| Segment_Tree | own + trivial | update took int (values > int truncated) |
| Segment_Tree_2D | own + init-order fix | Default declared after t, which read it uninitialised |
| Segment_Tree_Dynamic | own + fixes | rx - lx overflowed int on default [-1e9,1e9]; sums in int overflowed |
| Tree_Isomorphism | normalize version | Treated index 0 as a node; n=2 popped an empty queue. Not O(n log n): a 5e4 path takes 5.6 s / 2.5 GB |

## New templates (2026-11-18)

| Template | Source | Notes |
|---|---|---|
| DSU_Range_Parallel | Library Checker range_parallel_unionfind reference (read only) | verified; judge.yosupo.jp/problem/range_parallel_unionfind |
| Li_Chao_Persistent | Shohag (Hamza's had a leaf-overwrite bug: lines y=-2x+1, 2x, 2x+2 on [0,2): query(0)=1 want 0) | verified; ACPC 2024 E, CEOI09 Harbingers |
| Mod_Int | 7oSkaaa (other/hossam) + fixes (negatives kept negative; += int unreduced; negative power returned 1) | verified; CSES 1079 |
| Matrix_Power | Mazen + Matrix_Jumper | -O2 PASS seen by me; sanitizer not yet; yosupo pow_of_matrix, CSES 1096 |
| Poly_Inverse | Shohag inverse on repo NTT | verified; yosupo inv_of_formal_power_series |
| Aho_Corasick_Automaton | cp-algorithms | verified; yosupo aho_corasick, CSES 2103, CF 696D |
| Block_Cut_Tree | Shohag | verified; yosupo biconnected_components |
| Euler_Path | normalize + KACTL edge ids | verified (normalize returned {} for an edgeless graph); yosupo eulerian_trail_directed/undirected, CSES 1691, 1693 |
