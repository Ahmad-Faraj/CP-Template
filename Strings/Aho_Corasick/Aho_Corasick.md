# 1. Aho-Corasick Automaton (Advanced IGM Implementations)

The Aho-Corasick algorithm transforms a dictionary of strings into a Deterministic Finite Automaton (DFA). Because it is a DFA, it supports powerful graph algorithms (BFS, DFS, Matrix Exponentiation) and Dynamic Programming.

Below are the **complete, copy-pasteable implementations** for the highest-level Competitive Programming variations of Aho-Corasick.

> [!IMPORTANT]
> **Prerequisite for Variant 1 Functions:**
> For the algorithms below that run on the static pointer-based `Aho_Corasick.cpp`, you should track the BFS order and terminal nodes. Add these two arrays to your `struct AhoCorasick`:
> ```cpp
> vector<Node*> bfs_order;     // Track BFS order in build()
> vector<Node*> pattern_nodes; // Track terminal node of each pattern in insert()
> ```
> Update `insert()` to: `pattern_nodes.push_back(cur);`
> Update `build()` to: `bfs_order.push_back(next);` when pushing to queue.

---

## 1.1 Fast Offline Occurrence Counting (Fail Tree DP)
**Description:** If you need to count the frequency of all $K$ patterns in a massive text $T$, jumping up fail links for every character can cause TLE ($O(|T| \times K)$ worst case). 
**Solution:** Traverse the text once, incrementing a frequency counter at the matched DFA state. Then, push all frequencies UP the fail tree in reverse BFS order. Time: $O(|T| + \sum |S|)$.

```cpp
void solve_frequencies() {
    int n; cin >> n;
    vector<string> patterns(n);
    for (int i = 0; i < n; i++) cin >> patterns[i];
    
    AhoCorasick ac(patterns);
    
    string text; cin >> text;
    vector<long long> freq(nextNodeId, 0);
    
    // 1. Traverse text and tag DFA states
    auto cur = ac.root;
    for (char c : text) {
        int idx = c - 'a';
        cur = cur->child[idx]; // Because DFA transitions are fully built
        freq[cur->id]++;
    }
    
    // 2. Push frequencies up the fail tree (Reverse Topological Order)
    for (int i = ac.bfs_order.size() - 1; i >= 0; i--) {
        auto u = ac.bfs_order[i];
        if (u->fail) {
            freq[u->fail->id] += freq[u->id];
        }
    }
    
    // 3. Answer for each pattern
    for (int i = 0; i < n; i++) {
        cout << "Pattern " << i << " occurs " << freq[ac.pattern_nodes[i]->id] << " times.\n";
    }
}
```

---

## 1.2 Infinite String Avoiding Patterns (Cycle Detection)
**Description:** Determines if it is possible to construct an infinite string that does not contain any of the dictionary patterns.
**Solution:** Run DFS on the DFA. If we find a cycle composed entirely of "safe" states (states that don't contain a pattern), an infinite string exists.

```cpp
bool dfs_cycle(AhoCorasick::Node* u, vector<int>& state) {
    state[u->id] = 1; // 1 = currently exploring (in current path)
    
    for (int c = 0; c < 26; c++) {
        auto v = u->child[c];
        if (!v || !v->patIdx.empty()) continue; // Skip invalid transitions
        
        if (state[v->id] == 1) return true; // Cycle detected!
        if (state[v->id] == 0) {
            if (dfs_cycle(v, state)) return true;
        }
    }
    
    state[u->id] = 2; // 2 = fully visited and safe
    return false;
}

void solve_infinite_string() {
    int n; cin >> n;
    vector<string> patterns(n);
    for (int i = 0; i < n; i++) cin >> patterns[i];
    
    AhoCorasick ac(patterns);
    
    vector<int> state(nextNodeId, 0);
    if (ac.root->patIdx.empty() && dfs_cycle(ac.root, state)) {
        cout << "YES, infinite string exists\n";
    } else {
        cout << "NO\n";
    }
}
```

---

## 1.3 Count Strings of Length $L$ Avoiding Patterns (Matrix Exponentiation)
**Description:** Computes the total number of strings of length $L \le 10^{18}$ that do not contain any dictionary patterns.
**Time Complexity:** $O(V^3 \log L)$ where $V$ is the number of nodes in the Trie.

```cpp
const int MOD = 1e9 + 7;
struct Matrix {
    int n;
    vector<vector<long long>> mat;
    Matrix(int n) : n(n), mat(n, vector<long long>(n, 0)) {}
    Matrix operator*(const Matrix& other) const {
        Matrix res(n);
        for (int i = 0; i < n; i++)
            for (int k = 0; k < n; k++)
                for (int j = 0; j < n; j++)
                    res.mat[i][j] = (res.mat[i][j] + mat[i][k] * other.mat[k][j]) % MOD;
        return res;
    }
};

Matrix power(Matrix a, long long b) {
    Matrix res(a.n);
    for (int i = 0; i < a.n; i++) res.mat[i][i] = 1;
    while (b > 0) {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}

void solve_matrix_dp() {
    int n; long long L;
    cin >> n >> L;
    vector<string> patterns(n);
    for (int i = 0; i < n; i++) cin >> patterns[i];
    
    AhoCorasick ac(patterns);
    int V = nextNodeId;
    Matrix adj(V);
    
    // Build transitions for all valid states
    vector<AhoCorasick::Node*> all_nodes = ac.bfs_order;
    all_nodes.push_back(ac.root); // root might not be in bfs_order
    
    for (auto u : all_nodes) {
        if (!u->patIdx.empty()) continue; // Cannot transition out of an invalid state
        
        for (int c = 0; c < 26; c++) {
            auto v = u->child[c];
            if (v && v->patIdx.empty()) {
                adj.mat[u->id][v->id]++;
            }
        }
    }
    
    Matrix res = power(adj, L);
    long long total_valid_strings = 0;
    for (int i = 0; i < V; i++) {
        total_valid_strings = (total_valid_strings + res.mat[ac.root->id][i]) % MOD;
    }
    cout << total_valid_strings << "\n";
}
```

---

## 1.4 Shortest Superstring Containing All Patterns (BFS + Bitmask)
**Description:** Finds the length of the shortest string that contains *all* $K$ patterns as substrings ($K \le 20$). 

```cpp
void solve_shortest_superstring() {
    int n; cin >> n;
    vector<string> patterns(n);
    for (int i = 0; i < n; i++) cin >> patterns[i];
    
    AhoCorasick ac(patterns);
    int V = nextNodeId;
    int target_mask = (1 << n) - 1;
    
    vector<vector<int>> dist(V, vector<int>(1 << n, 1e9));
    queue<pair<AhoCorasick::Node*, int>> q;
    
    q.push({ac.root, 0});
    dist[ac.root->id][0] = 0;
    
    while (!q.empty()) {
        auto [u, mask] = q.front();
        q.pop();
        
        if (mask == target_mask) {
            cout << "Shortest superstring length: " << dist[u->id][mask] << "\n";
            return;
        }
        
        for (int c = 0; c < 26; c++) {
            auto v = u->child[c];
            if (!v) continue;
            
            int next_mask = mask;
            for (int idx : v->patIdx) next_mask |= (1 << idx);
            
            if (dist[u->id][mask] + 1 < dist[v->id][next_mask]) {
                dist[v->id][next_mask] = dist[u->id][mask] + 1;
                q.push({v, next_mask});
            }
        }
    }
    cout << "Impossible\n";
}
```

---

## 1.5 DP on Automaton: Max Value String of Length L
**Description:** Each pattern has a value. Find a string of length $L$ maximizing the total value of pattern occurrences.
**Solution:** $O(V \times L)$ DP. `dp[len][state] = max value`.

```cpp
void solve_max_value_dp() {
    int n, L; cin >> n >> L;
    vector<string> patterns(n);
    vector<int> values(n);
    for (int i = 0; i < n; i++) cin >> patterns[i] >> values[i];
    
    AhoCorasick ac(patterns);
    int V = nextNodeId;
    
    // Compute total value for landing on each state
    vector<int> state_value(V, 0);
    vector<AhoCorasick::Node*> all_nodes = ac.bfs_order;
    all_nodes.push_back(ac.root);
    for (auto u : all_nodes) {
        for (int idx : u->patIdx) {
            state_value[u->id] += values[idx];
        }
    }
    
    vector<vector<long long>> dp(L + 1, vector<long long>(V, -1e18));
    dp[0][ac.root->id] = 0;
    
    for (int len = 0; len < L; len++) {
        for (auto u : all_nodes) {
            if (dp[len][u->id] < -1e17) continue;
            
            for (int c = 0; c < 26; c++) {
                auto v = u->child[c];
                if (v) {
                    dp[len + 1][v->id] = max(dp[len + 1][v->id], 
                                           dp[len][u->id] + state_value[v->id]);
                }
            }
        }
    }
    
    long long ans = -1e18;
    for (int i = 0; i < V; i++) ans = max(ans, dp[L][i]);
    cout << ans << "\n";
}
```

---

## 1.6 Dynamic Aho-Corasick ($\log N$ technique) usage
**File:** `Aho_Corasick_Dynamic.cpp`
**Usage:** When you have online queries mixing `insert(string)` and `count(string)`, declare `aho_corasick ac;`. 
- `ac.insert(s)` executes in amortized $O(|S| \log K)$.
- `ac.count(s)` queries all active blocks in $O(|T| \log K)$.

```cpp
void solve_dynamic() {
    aho_corasick ac;
    ac.insert("he");
    ac.insert("she");
    cout << ac.count("ushers") << "\n"; // Output matches from online insertion
    ac.delete_word("he");               // Lazy deletion
    cout << ac.count("ushers") << "\n"; 
}
```

---

## 1.7 Aho-Corasick on a Tree (Tree Text Matching)
**Description:** Instead of searching for patterns in a linear string, the text is a path in a tree (or the entire tree rooted at 1, where each edge has a character). 
**Solution:** Run DFS on the tree, advancing the DFA state when stepping down, and implicitly reverting the state when backtracking.

```cpp
vector<pair<int, char>> tree[100005];
long long total_matches[100005];
int match_count_at_state[100005]; // Precomputed: match_count_at_state[u->id] = u->patIdx.size() + fail matches

void dfs_tree_text(int u, int p, AhoCorasick::Node* dfa_state) {
    total_matches[u] += match_count_at_state[dfa_state->id];
    
    for (auto& edge : tree[u]) {
        int v = edge.first;
        char c = edge.second;
        if (v == p) continue;
        
        dfs_tree_text(v, u, dfa_state->child[c - 'a']);
    }
}

// Call inside solve: dfs_tree_text(1, 0, ac.root);
```

---

## 1.8 Fail Tree + Fenwick Tree (Euler Tour for Online Queries)
**Description:** Given a dynamic set of dictionary strings (Add/Remove) and queries to count how many dictionary strings occur in a text $T$.
**Solution:** 
1. Build the fail tree explicitly. A pattern ending at state $U$ matches any text state $V$ in its subtree.
2. Maintain a Fenwick tree on the Euler Tour (in/out times) of the Fail Tree.
3. **Add/Remove Pattern:** Range update $+1$/$-1$ on the subtree of its terminal node.
4. **Query Text $T$:** Traverse the DFA for $T$. For each state $V$ visited, point query the Fenwick tree.

```cpp
struct FenwickTree {
    int n;
    vector<long long> tree;
    FenwickTree(int n) : n(n), tree(n + 1, 0) {}
    void add(int i, long long delta) {
        for (; i <= n; i += i & -i) tree[i] += delta;
    }
    long long query(int i) {
        long long sum = 0;
        for (; i > 0; i -= i & -i) sum += tree[i];
        return sum;
    }
};

void solve_fail_tree_fenwick() {
    int n; cin >> n;
    vector<string> initial_patterns(n);
    for (int i = 0; i < n; i++) cin >> initial_patterns[i];
    
    AhoCorasick ac(initial_patterns);
    
    // 1. Build Fail Tree
    int V = nextNodeId;
    vector<vector<int>> fail_tree(V);
    for (auto u : ac.bfs_order) {
        if (u->fail) fail_tree[u->fail->id].push_back(u->id);
    }
    
    // 2. Euler Tour
    vector<int> in(V), out(V);
    int timer = 0;
    
    function<void(int)> dfs_euler = [&](int u) {
        in[u] = ++timer;
        for (int v : fail_tree[u]) dfs_euler(v);
        out[u] = timer;
    };
    dfs_euler(ac.root->id);
    
    FenwickTree fenwick(timer + 5);
    
    // 3. Mark initially active patterns
    vector<int> is_active(n, 0);
    auto toggle_pattern = [&](int pattern_idx) {
        int term_id = ac.pattern_nodes[pattern_idx]->id;
        int delta = is_active[pattern_idx] ? -1 : 1;
        is_active[pattern_idx] ^= 1;
        
        // Range update on the subtree (adds pattern to all its fail-tree descendants)
        fenwick.add(in[term_id], delta);
        fenwick.add(out[term_id] + 1, -delta);
    };
    
    for (int i = 0; i < n; i++) toggle_pattern(i); // Activate all initial patterns
    
    // 4. Process Online Queries
    int q; cin >> q;
    while (q--) {
        int type; cin >> type;
        if (type == 1) { // Toggle a dictionary pattern by index
            int pattern_idx; cin >> pattern_idx; // 0-indexed
            toggle_pattern(pattern_idx);
        } else if (type == 2) { // Count occurrences in a new text
            string text; cin >> text;
            long long matches = 0;
            auto cur = ac.root;
            for (char c : text) {
                cur = cur->child[c - 'a']; // Using fully resolved DFA transitions
                // Point query gives the sum of all active dictionary prefixes ending here
                matches += fenwick.query(in[cur->id]); 
            }
            cout << matches << "\n";
        }
    }
}
```

---

## 1.9 Minimum Characters to Change to Avoid Patterns
**Description:** Given a string $S$, find the minimum number of characters you must change so that the string contains none of the dictionary patterns.
**Solution:** DP on the DFA. `dp[len][state]` = min changes.

```cpp
int solve_min_changes(string& text, AhoCorasick& ac) {
    int N = text.size();
    int V = nextNodeId;
    vector<vector<int>> dp(N + 1, vector<int>(V, 1e9));
    dp[0][ac.root->id] = 0;
    
    vector<AhoCorasick::Node*> all_nodes = ac.bfs_order;
    all_nodes.push_back(ac.root);
    
    for (int i = 0; i < N; i++) {
        for (auto u : all_nodes) {
            if (dp[i][u->id] >= 1e9) continue;
            
            for (int c = 0; c < 26; c++) {
                auto v = u->child[c];
                if (v && v->patIdx.empty()) { // Valid transition
                    int cost = (c == (text[i] - 'a')) ? 0 : 1;
                    dp[i + 1][v->id] = min(dp[i + 1][v->id], dp[i][u->id] + cost);
                }
            }
        }
    }
    
    int ans = 1e9;
    for (int i = 0; i < V; i++) ans = min(ans, dp[N][i]);
    return ans;
}
```
