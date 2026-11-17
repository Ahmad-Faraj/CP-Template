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
