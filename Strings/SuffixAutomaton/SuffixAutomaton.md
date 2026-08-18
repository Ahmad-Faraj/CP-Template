# Suffix Automaton (SAM) - The Ultimate IGM Reference

The **Suffix Automaton (SAM)** of a string $S$ is a minimal Directed Acyclic Graph (DAG) that accepts all suffixes of $S$. Consequently, any path from the root represents a valid substring of $S$. SAM is widely considered the crown jewel of string data structures due to its linear time $O(N)$ construction, linear space $O(N \cdot \Sigma)$ usage, and dual structural properties (DAG + Suffix Link Tree).

---

## 1. Core Properties & State Information

A Suffix Automaton consists of **States** and **Transitions**. Each state represents an **equivalence class of substrings** that share the exact same set of end-positions (`endpos`).

### State Attributes:
- `len`: The length of the longest substring belonging to this state's equivalence class. 
- `link` (Suffix Link): A pointer to the state that contains the longest suffix of this class that occurs in *different* contexts.
- `next[c]` (DAG Transitions): Points to the state representing the current substrings with the character `c` appended.



---

## 2. Fundamental Applications

### Application 1: Is $P$ a Substring of $S$?
*Determines if a given string $P$ is a contiguous substring of $S$ in $O(|P|)$ time.*
```cpp
bool isSubstring(const string& P) {
    int u = 0;
    for (char c : P) {
        if (st[u].next[c - 'a'] == -1) return false;
        u = st[u].next[c - 'a'];
    }
    return true;
}
```

### Application 2: Number of Distinct Substrings
*Computes the total number of unique substrings present in $S$ by analyzing state lengths.*
```cpp
long long distinctSubstrings() {
    long long total = 0;
    for (int i = 1; i < (int)st.size(); i++) {
        total += st[i].len - st[st[i].link].len;
    }
    return total;
}
```

### Application 3: Total Length of All Distinct Substrings
*Calculates the sum of lengths of all unique substrings in $S$ using dynamic programming on the DAG.*
```cpp
long long totalLengthOfDistinctSubstrings() {
    vector<long long> dp(st.size(), -1), len_sum(st.size(), 0);
    
    auto dfs = [&](auto& self, int u) -> void {
        if (dp[u] != -1) return;
        dp[u] = 1; 
        len_sum[u] = 0;
        for (int c = 0; c < 26; c++) {
            if (st[u].next[c] != -1) {
                int v = st[u].next[c];
                self(self, v);
                dp[u] += dp[v];
                len_sum[u] += len_sum[v] + dp[v];
            }
        }
    };
    
    dfs(dfs, 0);
    return len_sum[0];
}
```

---

## 3. Advanced / IGM-Level Applications

### Application 4: Number of Occurrences of Substrings (Frequency)
*Calculates exactly how many times every possible substring appears in $S$ by propagating state sizes up the Parent Tree.*
```cpp
void build_frequencies() {
    vector<int> cnt(st.size(), 0), order(st.size());
    for(int i = 0; i < (int)st.size(); i++) cnt[st[i].len]++;
    for(int i = 1; i < (int)cnt.size(); i++) cnt[i] += cnt[i-1];
    for(int i = 0; i < (int)st.size(); i++) order[--cnt[st[i].len]] = i;
    
    for(int i = (int)st.size() - 1; i > 0; i--) {
        int u = order[i];
        if(st[u].link != -1) {
            st[st[u].link].sz += st[u].sz;
        }
    }
}
```

### Application 5: Locating Occurrences (first_pos and all_pos)
*Finds the 0-indexed ending positions of all occurrences of a specific pattern $P$ in $S$.*
```cpp
void findAllOccurrences(const string& P) {
    int u = 0;
    for (char c : P) {
        if (st[u].next[c - 'a'] == -1) return;
        u = st[u].next[c - 'a'];
    }
    
    vector<vector<int>> link_tree(st.size());
    for(int i = 1; i < (int)st.size(); i++) {
        link_tree[st[i].link].push_back(i);
    }
    
    auto dfs = [&](auto& self, int curr) -> void {
        if (!st[curr].is_clone) {
            cout << "Occurrence ends at index: " << st[curr].first_pos << "\n";
        }
        for (int v : link_tree[curr]) {
            self(self, v);
        }
    };
    
    dfs(dfs, u);
}
```

### Application 6: $K$-th Lexicographically Smallest Substring
*Finds the $K$-th lexicographically smallest substring of $S$, with or without counting duplicate occurrences.*
```cpp
string kth_substring(long long K, bool count_duplicates) {
    vector<long long> dp(st.size(), -1);
    
    auto dfs = [&](auto& self, int u) -> void {
        if (dp[u] != -1) return;
        dp[u] = count_duplicates ? st[u].sz : 1;
        for (int c = 0; c < 26; c++) {
            if (st[u].next[c] != -1) {
                int v = st[u].next[c];
                self(self, v);
                dp[u] += dp[v];
            }
        }
    };
    dfs(dfs, 0);
    
    string res = ""; 
    int u = 0;
    if (K > dp[0] - (count_duplicates ? st[0].sz : 1)) return "-1";
    
    while (K > 0) {
        for (int c = 0; c < 26; c++) {
            if (st[u].next[c] != -1) {
                int v = st[u].next[c];
                if (K <= dp[v]) {
                    res += (char)('a' + c);
                    u = v;
                    K -= (count_duplicates ? st[u].sz : 1);
                    break;
                } else {
                    K -= dp[v];
                }
            }
        }
    }
    return res;
}
```

### Application 7: Lexicographically Smallest Cyclic Shift
*Finds the lexicographically smallest cyclic shift of $S$ by traversing the DAG of $S+S$.*
```cpp
string smallest_cyclic_shift(const string& S) {
    SuffixAutomaton sam;
    for(char c : S) sam.extend(c);
    for(char c : S) sam.extend(c);
    
    string res = ""; 
    int u = 0;
    for (int i = 0; i < (int)S.length(); i++) {
        for (int c = 0; c < 26; c++) {
            if (sam.st[u].next[c] != -1) {
                res += (char)('a' + c); 
                u = sam.st[u].next[c]; 
                break;
            }
        }
    }
    return res;
}
```

### Application 8: Longest Common Substring (LCS) of Two Strings $S$ and $T$
*Finds the length of the longest contiguous substring common to both $S$ and $T$.*
```cpp
int LCS_2(const string& T) {
    int v = 0, l = 0, best = 0;
    for (char c : T) {
        while (v && st[v].next[c - 'a'] == -1) { 
            v = st[v].link; 
            l = st[v].len; 
        }
        if (st[v].next[c - 'a'] != -1) { 
            v = st[v].next[c - 'a']; 
            l++; 
        }
        best = max(best, l);
    }
    return best;
}
```

### Application 9: Longest Common Substring of $K$ Strings (Generalized LCS)
*Computes the length of the longest substring that appears in every single string in a given set.*
```cpp
int generalized_LCS(const vector<string>& strings) {
    vector<int> ans(st.size(), 1e9);
    
    vector<int> cnt(st.size(), 0), order(st.size());
    for(int i = 0; i < (int)st.size(); i++) cnt[st[i].len]++;
    for(int i = 1; i < (int)cnt.size(); i++) cnt[i] += cnt[i-1];
    for(int i = 0; i < (int)st.size(); i++) order[--cnt[st[i].len]] = i;
    
    for (const string& T : strings) {
        vector<int> mx(st.size(), 0);
        int v = 0, l = 0;
        for (char c : T) {
            while (v && st[v].next[c - 'a'] == -1) { 
                v = st[v].link; 
                l = st[v].len; 
            }
            if (st[v].next[c - 'a'] != -1) { 
                v = st[v].next[c - 'a']; 
                l++; 
            }
            mx[v] = max(mx[v], l);
        }
        for(int i = (int)st.size() - 1; i > 0; i--) {
            int u = order[i];
            if (st[u].link != -1) {
                mx[st[u].link] = max(mx[st[u].link], min(st[st[u].link].len, mx[u]));
            }
            ans[u] = min(ans[u], mx[u]);
        }
    }
    
    int global_max = 0;
    for(int i = 1; i < (int)st.size(); i++) {
        global_max = max(global_max, ans[i]);
    }
    return global_max;
}
```

### Application 10: Substring Queries in a Range $[L, R]$ (SAM + Segment Tree Merge)
*Determines if pattern $P$ exists strictly within the index range $[L, R]$ of $S$ using dynamically merged segment trees.*
```cpp
struct EndposTree {
    vector<int> lc, rc, root;
    int node_cnt;

    EndposTree(int max_nodes, int states) {
        lc.assign(max_nodes, 0);
        rc.assign(max_nodes, 0);
        root.assign(states, 0);
        node_cnt = 0;
    }

    void insert(int &rt, int l, int r, int pos) {
        if (!rt) rt = ++node_cnt;
        if (l == r) return;
        int mid = (l + r) >> 1;
        if (pos <= mid) insert(lc[rt], l, mid, pos);
        else insert(rc[rt], mid + 1, r, pos);
    }

    int merge(int u, int v) {
        if (!u || !v) return u ? u : v;
        int rt = ++node_cnt;
        lc[rt] = merge(lc[u], lc[v]);
        rc[rt] = merge(rc[u], rc[v]);
        return rt;
    }

    bool query(int rt, int l, int r, int ql, int qr) {
        if (!rt || ql > r || qr < l) return false;
        if (ql <= l && r <= qr) return true;
        int mid = (l + r) >> 1;
        return query(lc[rt], l, mid, ql, qr) || query(rc[rt], mid + 1, r, ql, qr);
    }
};

void build_endpos_trees(EndposTree& seg, const vector<int>& order, int N) {
    for (int i = 1; i < (int)st.size(); i++) {
        if (!st[i].is_clone) seg.insert(seg.root[i], 0, N - 1, st[i].first_pos);
    }
    for (int i = (int)st.size() - 1; i > 0; i--) { 
        int u = order[i];
        if (st[u].link != -1) {
            seg.root[st[u].link] = seg.merge(seg.root[st[u].link], seg.root[u]);
        }
    }
}

bool pattern_in_range(const string& P, int L, int R, EndposTree& seg, int N) {
    int u = 0;
    for (char c : P) {
        if (st[u].next[c - 'a'] == -1) return false;
        u = st[u].next[c - 'a'];
    }
    return seg.query(seg.root[u], 0, N - 1, L + (int)P.length() - 1, R);
}
```

### Application 11: Shortest Non-Appearing Substring
*Finds the absolute shortest string over the alphabet that does not appear anywhere in $S$.*
```cpp
string shortest_missing() {
    queue<pair<int, string>> q; 
    q.push({0, ""});
    while (!q.empty()) {
        auto [u, s] = q.front(); 
        q.pop();
        for (int c = 0; c < 26; c++) {
            if (st[u].next[c] == -1) {
                return s + (char)('a' + c);
            }
            q.push({st[u].next[c], s + (char)('a' + c)});
        }
    }
    return "";
}
```

### Application 12: Online Pattern Counting (Dynamic SAM / LCT)
*Maintains the frequency of pattern $P$ online as characters are continually appended to $S$ using a Link-Cut Tree.*
```cpp
struct LCTNode {
    int ch[2], p;
    int val, sum;
    LCTNode() { ch[0] = ch[1] = p = val = sum = 0; }
};
vector<LCTNode> lct;

bool is_root(int x) { return lct[lct[x].p].ch[0] != x && lct[lct[x].p].ch[1] != x; }
void push_up(int x) { lct[x].sum = lct[lct[x].ch[0]].sum + lct[lct[x].ch[1]].sum + lct[x].val; }
void rotate(int x) {
    int y = lct[x].p, z = lct[y].p, k = (lct[y].ch[1] == x);
    if (!is_root(y)) lct[z].ch[lct[z].ch[1] == y] = x;
    lct[x].p = z;
    lct[y].ch[k] = lct[x].ch[k ^ 1];
    if (lct[x].ch[k ^ 1]) lct[lct[x].ch[k ^ 1]].p = y;
    lct[x].ch[k ^ 1] = y; lct[y].p = x;
    push_up(y); push_up(x);
}
void splay(int x) {
    while (!is_root(x)) {
        int y = lct[x].p, z = lct[y].p;
        if (!is_root(y)) rotate((lct[y].ch[0] == x) ^ (lct[z].ch[0] == y) ? x : y);
        rotate(x);
    }
}
void access(int x) {
    for (int y = 0; x; y = x, x = lct[x].p) {
        splay(x); lct[x].ch[1] = y; push_up(x);
    }
}
void link(int x, int y) {
    access(x); splay(x); lct[x].p = y;
}
void cut(int x) {
    access(x); splay(x);
    lct[lct[x].ch[0]].p = 0; lct[x].ch[0] = 0; push_up(x);
}
int query_freq(int u) {
    access(u); splay(u); return lct[u].sum;
}
```

### Application 13: Lowest Common Ancestor (LCA) in Suffix Link Tree
*Finds the length of the longest common suffix of two prefixes of $S$ using $O(1)$ LCA queries on the Parent Tree.*
```cpp
const int LOG = 21;
vector<vector<int>> up;
vector<int> depth;

void build_lca(const vector<int>& order) {
    up.assign(st.size(), vector<int>(LOG, 0));
    depth.assign(st.size(), 0);
    
    for (int i = (int)st.size() - 1; i > 0; i--) {
        int u = order[i]; // Top-down
        if (st[u].link != -1) {
            depth[u] = depth[st[u].link] + 1;
            up[u][0] = st[u].link;
            for (int j = 1; j < LOG; j++) {
                up[u][j] = up[up[u][j - 1]][j - 1];
            }
        }
    }
}

int get_lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    for (int i = LOG - 1; i >= 0; i--) {
        if (depth[u] - (1 << i) >= depth[v]) u = up[u][i];
    }
    if (u == v) return u;
    for (int i = LOG - 1; i >= 0; i--) {
        if (up[u][i] != up[v][i]) {
            u = up[u][i]; v = up[v][i];
        }
    }
    return up[u][0];
}
```

### Application 14: Suffix Array & Suffix Tree Construction in Linear Time
*Constructs the Suffix Array of $S$ in linear time by traversing the Suffix Link tree of the reversed string $S^R$.*
```cpp
vector<int> SA;
void build_SA(const string& S) {
    int N = S.length();
    vector<vector<pair<char, int>>> sorted_link_tree(st.size());
    for (int i = 1; i < (int)st.size(); i++) {
        char edge_char = S[N - 1 - st[i].first_pos + st[st[i].link].len];
        sorted_link_tree[st[i].link].push_back({edge_char, i});
    }
    for (int i = 0; i < (int)st.size(); i++) {
        sort(sorted_link_tree[i].begin(), sorted_link_tree[i].end());
    }
    
    auto dfs = [&](auto& self, int u) -> void {
        if (!st[u].is_clone) SA.push_back(N - 1 - st[u].first_pos);
        for (auto [c, v] : sorted_link_tree[u]) self(self, v);
    };
    dfs(dfs, 0);
}
```

### Application 15: String Periods and Borders
*Identifies all border lengths and valid periods of $S$ by walking up the Suffix Link tree from the terminal state.*
```cpp
void find_borders(int N) {
    int curr = st[last].link; 
    while (curr > 0) {
        cout << "Border of length: " << st[curr].len << " implies period " << N - st[curr].len << "\n";
        curr = st[curr].link;
    }
}
```

---

## 4. Master / LGM-Level Applications

### Application 16: Finding all Maximal Repeats
*Finds all substrings that occur multiple times and cannot be extended left or right without losing frequency.*
```cpp
void find_maximal_repeats() {
    vector<vector<int>> link_tree(st.size());
    for(int i = 1; i < (int)st.size(); i++) {
        link_tree[st[i].link].push_back(i);
    }
    
    for (int i = 1; i < (int)st.size(); i++) {
        if (link_tree[i].size() >= 2) {
            cout << "Maximal repeat represented by state " << i << " of length up to " << st[i].len << "\n";
        }
    }
}
```

### Application 17: $O(1)$ Frequency Query of $S[L..R]$ (Binary Lifting)
*Answers $O(1)$ offline queries for the exact frequency of $S[L..R]$ using binary lifting on the Parent Tree.*
```cpp
int get_substring_frequency(int L, int R, const vector<int>& pos_array) { 
    int u = pos_array[R];
    int req_len = R - L + 1;
    for (int i = LOG - 1; i >= 0; i--) {
        if (up[u][i] && st[up[u][i]].len >= req_len) u = up[u][i];
    }
    return st[u].sz;
}
```

### Application 18: String Matching with $K$ Mismatches
*Finds all occurrences of pattern $P$ in $S$ allowing up to $K$ mismatched characters using heavily pruned DFS.*
```cpp
int match_with_mismatches(const string& P, int K) {
    auto dfs = [&](auto& self, int u, int idx, int k_left) -> int {
        if (idx == (int)P.size()) return st[u].sz;
        int ans = 0;
        for (int c = 0; c < 26; c++) {
            if (st[u].next[c] != -1) {
                int cost = (c != P[idx] - 'a');
                if (k_left >= cost) {
                    ans += self(self, st[u].next[c], idx + 1, k_left - cost);
                }
            }
        }
        return ans;
    };
    return dfs(dfs, 0, 0, K);
}
```

### Application 19: Sliding Window Suffix Automaton
*Tracks whether pattern $P$ occurs within a sliding window of fixed length over an infinite stream.*
```cpp
bool is_substring_in_window(const string& P, int W_L, int W_R, EndposTree& seg, int N) {
    int u = 0;
    for (char c : P) {
        if (st[u].next[c - 'a'] == -1) return false;
        u = st[u].next[c - 'a'];
    }
    return seg.query(seg.root[u], 0, N - 1, W_L + (int)P.length() - 1, W_R);
}
```

### Application 20: Generating the LCP Array in Linear Time
*Generates the Longest Common Prefix (LCP) array in purely linear time directly alongside the Suffix Array.*
```cpp
vector<int> LCP;
void build_LCP(const string& S) { 
    int N = S.length();
    vector<vector<pair<char, int>>> sorted_link_tree(st.size());
    for (int i = 1; i < (int)st.size(); i++) {
        char edge_char = S[N - 1 - st[i].first_pos + st[st[i].link].len];
        sorted_link_tree[st[i].link].push_back({edge_char, i});
    }
    for (int i = 0; i < (int)st.size(); i++) {
        sort(sorted_link_tree[i].begin(), sorted_link_tree[i].end());
    }

    int last_leaf = -1;
    auto dfs = [&](auto& self, int u) -> void {
        if (!st[u].is_clone) {
            if (last_leaf != -1) {
                LCP.push_back(st[get_lca(last_leaf, u)].len);
            }
            last_leaf = u;
        }
        for (auto [c, v] : sorted_link_tree[u]) self(self, v);
    };
    dfs(dfs, 0);
}
```

---

## 5. Legendary / Research-Level Applications

### Application 21: Longest Substring Occurring at Least $K$ Times
*Finds the length of the longest contiguous substring that appears at least $K$ distinct times in $S$.*
```cpp
int longest_at_least_k(int K) {
    int max_len = 0;
    for (int i = 1; i < (int)st.size(); i++) {
        if (st[i].sz >= K) {
            max_len = max(max_len, st[i].len);
        }
    }
    return max_len;
}
```

### Application 22: Shortest Unique Substring
*Finds the absolute shortest substring of $S$ that occurs exactly once.*
```cpp
int shortest_unique() {
    int min_len = 1e9;
    for (int i = 1; i < (int)st.size(); i++) {
        if (st[i].sz == 1) {
            min_len = min(min_len, st[st[i].link].len + 1);
        }
    }
    return min_len == 1e9 ? -1 : min_len;
}
```

### Application 23: Finding all Tandem Repeats (Squares like $WW$)
*Finds all tandem repeats (adjacent identical substrings like $WW$) in $O(N \log N)$ by merging `endpos` sets.*
```cpp
void find_squares(const vector<int>& order) {
    vector<set<int>> endpos_set(st.size());
    for (int i = 1; i < (int)st.size(); i++) {
        if (!st[i].is_clone) endpos_set[i].insert(st[i].first_pos);
    }
    
    for (int i = (int)st.size() - 1; i > 0; i--) {
        int u = order[i];
        if (st[u].link != -1) {
            int p = st[u].link;
            if (endpos_set[u].size() > endpos_set[p].size()) {
                swap(endpos_set[u], endpos_set[p]);
            }
            for (int pos : endpos_set[u]) {
                if (endpos_set[p].count(pos - st[p].len) || endpos_set[p].count(pos + st[p].len)) {
                    cout << "Square found of length " << 2 * st[p].len << "\n";
                }
                endpos_set[p].insert(pos);
            }
        }
    }
}
```

### Application 24: Computing the Z-Array in $O(1)$ per query
*Answers $O(1)$ queries for the Z-array (LCP of $S$ and suffix $S[i..N]$) using Suffix Link Tree LCA.*
```cpp
int query_Z(int i, int N, const vector<int>& pos_rev) { 
    return st[get_lca(pos_rev[N], pos_rev[N - i + 1])].len; 
}
```

### Application 25: Burrows-Wheeler Transform (BWT) in Linear Time
*Generates the Burrows-Wheeler Transform string directly from the Suffix Link Tree in linear time.*
```cpp
string get_BWT(const string& S) {
    int N = S.length();
    vector<vector<pair<char, int>>> sorted_link_tree(st.size());
    for (int i = 1; i < (int)st.size(); i++) {
        char edge_char = S[N - 1 - st[i].first_pos + st[st[i].link].len];
        sorted_link_tree[st[i].link].push_back({edge_char, i});
    }
    for (int i = 0; i < (int)st.size(); i++) {
        sort(sorted_link_tree[i].begin(), sorted_link_tree[i].end());
    }

    string BWT = "";
    auto dfs = [&](auto& self, int u) -> void {
        if (!st[u].is_clone) {
            int idx = N - 1 - st[u].first_pos;
            BWT += (idx == 0) ? '$' : S[idx - 1]; 
        }
        for (auto [c, v] : sorted_link_tree[u]) self(self, v);
    };
    dfs(dfs, 0);
    return BWT;
}
```

### Application 26: Suffix Automaton as an Aho-Corasick Alternative
*Computes the exact frequency of multiple independent patterns in text $S$ in $O(|P_i|)$ per pattern.*
```cpp
int count_occurrences_of_pattern(const string& P) {
    int u = 0;
    for (char c : P) {
        if (st[u].next[c - 'a'] == -1) return 0;
        u = st[u].next[c - 'a'];
    }
    return st[u].sz;
}
```

### Application 27: Smallest Period of all Prefixes (KMP $\pi$ Array Recovery)
*Recovers the KMP $\pi$ array (prefix function) of $S$ by analyzing the `endpos` boundaries in the Parent Tree.*
```cpp
vector<int> recover_kmp(EndposTree& seg, int N) {
    vector<int> pi(N + 1, 0);
    for (int i = 1; i < (int)st.size(); i++) {
        if (seg.query(seg.root[i], 0, N - 1, 1, 1)) {
            pi[st[i].first_pos] = max(pi[st[i].first_pos], st[i].len);
        }
    }
    return pi;
}
```

### Application 28: Longest Palindromic Substring via SAM
*Finds the length of the longest palindromic substring in $S$ by intersecting $S$ and $S^R$ on the Parent Tree.*
```cpp
int longest_palindrome(int N, const vector<int>& pos, const vector<int>& pos_rev) {
    int max_pal = 1;
    for (int i = 1; i <= N; i++) {
        int lca_state = get_lca(pos[i], pos_rev[N - i + 1]);
        int match_len = min({st[lca_state].len, i, N - i + 1});
        max_pal = max(max_pal, 2 * match_len - 1);
    }
    return max_pal;
}
```

---

## 6. Generalized Suffix Automaton (GSAM)

```cpp
void extend_generalized(char c, int &last_node) {
    if (st[last_node].next[c - 'a'] != -1) {
        int q = st[last_node].next[c - 'a'];
        if (st[q].len == st[last_node].len + 1) { 
            last_node = q; 
            return; 
        }
        int clone = st.size(); 
        st.emplace_back();
        st[clone].len = st[last_node].len + 1;
        for (int i = 0; i < 26; ++i) st[clone].next[i] = st[q].next[i];
        st[clone].link = st[q].link;
        st[clone].first_pos = st[q].first_pos;
        st[clone].sz = 0;
        st[clone].is_clone = true;
        
        while (last_node != -1 && st[last_node].next[c - 'a'] == q) {
            st[last_node].next[c - 'a'] = clone; 
            last_node = st[last_node].link;
        }
        st[q].link = clone;
        last_node = clone; 
        return;
    }
    
    int cur = st.size();
    st.emplace_back();
    st[cur].len = st[last_node].len + 1;
    st[cur].first_pos = st[cur].len - 1;
    st[cur].sz = 1;

    int p = last_node;
    while (p != -1 && st[p].next[c - 'a'] == -1) {
        st[p].next[c - 'a'] = cur;
        p = st[p].link;
    }

    if (p == -1) {
        st[cur].link = 0;
    } else {
        int q = st[p].next[c - 'a'];
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        } else {
            int clone = st.size();
            st.emplace_back();
            st[clone].len = st[p].len + 1;
            for (int i = 0; i < 26; ++i) st[clone].next[i] = st[q].next[i];
            st[clone].link = st[q].link;
            st[clone].first_pos = st[q].first_pos;
            st[clone].sz = 0;
            st[clone].is_clone = true;

            while (p != -1 && st[p].next[c - 'a'] == q) {
                st[p].next[c - 'a'] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last_node = cur;
}
```
