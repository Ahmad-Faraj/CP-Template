# 1. Palindromic Tree (Eertree)

## 1.0 Overview
The Palindromic Tree natively handles palindromic substrings. It contains two root nodes (one for even length palindromes, one for odd length). Every node represents a unique palindrome. Transitions denote appending a character to both the front and back of the palindrome.

- **Time Complexity:** $O(N \log \Sigma)$ to build
- **Space Complexity:** $O(N \times \Sigma)$

---

## 1.1 Online Total Palindromes Using `num`
**Description:** Count the number of total palindromic substrings continuously as characters are added in $O(1)$ extra time per character.

```cpp
long long solve_online_palindromes() {
    string s; cin >> s;
    PalindromicTree pt(s.size(), 26, 'a');
    
    long long total_palindromes = 0;
    for (int i = 0; i < s.size(); i++) {
        pt.addChar(s[i], i + 1);
        // tree[last].num tracks the exact number of distinct palindromes ending at index i
        total_palindromes += pt.tree[pt.last].num; 
    }
    return total_palindromes;
}
```

---

## 1.2 Common Palindromes of Two Strings
**Description:** Find the total number of common palindromic substrings between String A and String B.
**Solution:** Insert String A into the Eertree, then propagate occurrences. Store these occurrences in an array. Reset the Eertree's state (clear occurrences, reset `last = 1`, and add a unique delimiter character), then process String B and propagate occurrences. Multiply the occurrences of valid states!

```cpp
void solve_common_palindromes() {
    string A, B; cin >> A >> B;
    PalindromicTree pt(A.size() + B.size() + 5, 27, 'a'); // 27 for the delimiter
    
    for (int i = 0; i < A.size(); i++) pt.addChar(A[i], i + 1);
    pt.propagateOccurrences();
    
    // Save frequencies of String A
    vector<long long> freqA(pt.tree.size(), 0);
    for (int i = 2; i < pt.tree.size(); i++) freqA[i] = pt.tree[i].occ;
    
    // Reset state for String B
    for (int i = 2; i < pt.tree.size(); i++) pt.tree[i].occ = 0;
    
    // Insert delimiter so B doesn't mix with A's physical string matches
    pt.addChar('{', A.size() + 1); 
    pt.last = 1; // Reset matching state to the odd root
    
    for (int i = 0; i < B.size(); i++) pt.addChar(B[i], A.size() + i + 2);
    pt.propagateOccurrences();
    
    long long common_palindromes = 0;
    for (int i = 2; i < pt.tree.size(); i++) {
        // Multiply occurrences of state i in String A and String B
        common_palindromes += freqA[i] * pt.tree[i].occ;
    }
    cout << common_palindromes << "\n";
}
```

---

## 1.3 Minimum Palindromic Factorization DP ($O(N \log N)$)
**Description:** Find the minimum number of palindromes required to partition a string perfectly. By extending the Eertree `Node` with `diff` (the difference in length between a node and its fail link) and `slink` (series link: points to the first fail link that has a different `diff`), we can jump across arithmetic progressions of palindromes, theoretically reducing the DP transition from $O(N^2)$ down to $O(N \log N)$!

```cpp
// 1. Add these to struct Node in Palindromic_Tree.cpp:
// int diff = 0;
// int slink = 0;

// 2. Update these inside addChar() right after step 4 (after link is set):
// tree[last].diff = tree[last].len - tree[tree[last].link].len;
// if (tree[last].diff == tree[tree[last].link].diff) {
//     tree[last].slink = tree[tree[last].link].slink;
// } else {
//     tree[last].slink = tree[last].link;
// }

void solve_min_factorization(string s) {
    int n = s.size();
    PalindromicTree pt(n, 26, 'a');
    
    vector<int> dp(n + 1, 1e9);
    vector<int> series_ans(n + 5, 1e9); // Tracks DP transitions over series links
    dp[0] = 0;
    
    for (int i = 1; i <= n; i++) {
        pt.addChar(s[i - 1], i);
        int v = pt.last;
        
        while (v > 1) { // 1 is odd root, 0 is even root
            series_ans[v] = dp[i - (pt.tree[pt.tree[v].slink].len + pt.tree[v].diff)];
            
            if (pt.tree[v].diff == pt.tree[pt.tree[v].link].diff) {
                series_ans[v] = min(series_ans[v], series_ans[pt.tree[v].link]);
            }
            
            dp[i] = min(dp[i], series_ans[v] + 1);
            v = pt.tree[v].slink;
        }
    }
    cout << dp[n] << "\n";
}
```

---

## 1.4 Binary Lifting on Fail Tree ($O(\log N)$ Substring Queries)
**Description:** By building a sparse table (binary lifting) over the `link` (fail links) of the Eertree, you can query whether *any* arbitrary substring $S[L..R]$ is a palindrome, and instantly fetch its total occurrences in the entire string in $O(\log N)$ time online!

```cpp
// 1. Add these to your struct or global scope:
// int up[MAXN][20];
// int pos_node[MAXN]; // Maps text index i to the node ending at i

// 2. Update inside addChar() right after tree[last].link is assigned:
/*
    up[last][0] = tree[last].link;
    for (int k = 1; k < 20; k++) {
        up[last][k] = up[up[last][k-1]][k-1];
    }
    pos_node[pos] = last;
*/

// Query: Is S[L..R] a palindrome? (1-based indices)
bool is_palindrome(int L, int R) {
    int len = R - L + 1;
    int u = pos_node[R];
    
    // Jump up the fail tree until we find the longest palindromic suffix <= len
    for (int k = 19; k >= 0; k--) {
        if (tree[up[u][k]].len >= len) {
            u = up[u][k];
        }
    }
    return tree[u].len == len;
}

// Query: Frequency of palindrome S[L..R] in the entire string
long long get_frequency(int L, int R) {
    int len = R - L + 1;
    int u = pos_node[R];
    for (int k = 19; k >= 0; k--) {
        if (tree[up[u][k]].len >= len) {
            u = up[u][k];
        }
    }
    if (tree[u].len == len) return tree[u].occ;
    return 0; // The requested substring is not even a palindrome
}
```

---

## 1.5 Double-Ended Palindromic Tree (Deque Eertree)
**Description:** The standard Eertree only allows appending characters to the back of the string. A Deque Eertree is an IGM-level variation that allows `push_front` AND `push_back` dynamically.
**Implementation Trick:** We maintain a large character array centered at `MAXN`, and explicitly track two states: `last_front` and `last_back`. If an inserted character makes the *entire* current string a palindrome, the front and back pointers must synchronize.

```cpp
struct DequeEertree {
    struct Node {
        int next[26];
        int link, len;
        Node(int L = 0) : link(0), len(L) { memset(next, 0, sizeof(next)); }
    };
    
    vector<Node> tree;
    char s[200005]; // Large array to allow front/back expansion
    int L, R;
    int last_front, last_back;

    DequeEertree() {
        tree.emplace_back(-1); // Imaginary Root
        tree.emplace_back(0);  // Empty Root
        tree[0].link = tree[1].link = 0;
        L = 100000; R = 99999; 
        last_front = 1; last_back = 1;
    }

    void push_back(char ch) {
        s[++R] = ch;
        int cur = last_back, c = ch - 'a';
        while (s[R - tree[cur].len - 1] != ch) cur = tree[cur].link;
        
        if (!tree[cur].next[c]) {
            int nw = tree.size();
            tree.emplace_back(tree[cur].len + 2);
            int lnk = tree[cur].link;
            while (s[R - tree[lnk].len - 1] != ch) lnk = tree[lnk].link;
            tree[nw].link = (tree[nw].len == 1) ? 1 : tree[lnk].next[c];
            tree[cur].next[c] = nw;
        }
        last_back = tree[cur].next[c];
        
        // Critical Sync: If the entire string became a single palindrome
        if (tree[last_back].len == R - L + 1) last_front = last_back;
    }

    void push_front(char ch) {
        s[--L] = ch;
        int cur = last_front, c = ch - 'a';
        while (s[L + tree[cur].len + 1] != ch) cur = tree[cur].link;
        
        if (!tree[cur].next[c]) {
            int nw = tree.size();
            tree.emplace_back(tree[cur].len + 2);
            int lnk = tree[cur].link;
            while (s[L + tree[lnk].len + 1] != ch) lnk = tree[lnk].link;
            tree[nw].link = (tree[nw].len == 1) ? 1 : tree[lnk].next[c];
            tree[cur].next[c] = nw;
        }
        last_front = tree[cur].next[c];
        
        // Critical Sync: If the entire string became a single palindrome
        if (tree[last_front].len == R - L + 1) last_back = last_front;
    }
};
```

---

## 1.6 Eertree on a Graph / Trie (Tree-Structured Text)
**Description:** Instead of a linear string, the text is a path in a rooted Tree (e.g., a Trie of strings). The Eertree can effortlessly build palindromes over a graph by mapping the DFS recursion stack to the Eertree's `last` state.

```cpp
int physical_string[100005]; // The path from root to current node

void dfs_tree_eertree(int u, int p, int depth, int last_node_state, PalindromicTree& pt) {
    char ch = physical_string[depth]; // The character for the current edge
    int cur = last_node_state;
    int c = ch - 'a';
    
    // Custom match logic jumping up the fail links
    while (true) {
        int curlen = pt.tree[cur].len;
        // Check if the physical character before the palindrome matches
        if (depth - 1 - curlen >= 1 && physical_string[depth - 1 - curlen] == ch)
            break;
        cur = pt.tree[cur].link;
    }
    
    int next_state;
    if (pt.tree[cur].next[c]) {
        next_state = pt.tree[cur].next[c];
    } else {
        next_state = pt.tree.size();
        // ... (Standard Eertree node creation and suffix linking)
    }
    
    for (auto& edge : adj[u]) { // Standard Graph DFS
        int v = edge.to;
        if (v == p) continue;
        physical_string[depth + 1] = edge.char_weight;
        dfs_tree_eertree(v, u, depth + 1, next_state, pt);
    }
}
```

---

## 1.7 Synchronized DFS for Eertree Intersection
**Description:** Instead of physically concatenating two strings with a delimiter (which costs $O(|A| + |B|)$ memory and state-resetting), you can independently build an Eertree for String A and an Eertree for String B. Because the transitions strictly build identical palindromes, you can perform a synchronized DFS to multiply matching occurrences!

```cpp
long long intersect_dfs(int u1, int u2, PalindromicTree& ptA, PalindromicTree& ptB) {
    // If the node represents a valid palindrome (length > 0)
    long long common = 0;
    if (ptA.tree[u1].len > 0) {
        common = ptA.tree[u1].occ * ptB.tree[u2].occ;
    }
    
    // Traverse identical outbound character transitions
    for (int c = 0; c < 26; c++) {
        int v1 = ptA.tree[u1].next[c];
        int v2 = ptB.tree[u2].next[c];
        if (v1 && v2) {
            common += intersect_dfs(v1, v2, ptA, ptB);
        }
    }
    return common;
}

// Call inside solve():
// ptA.propagateOccurrences(); ptB.propagateOccurrences();
// long long total_common = intersect_dfs(0, 0, ptA, ptB) + intersect_dfs(1, 1, ptA, ptB);
```

---

## 1.8 Total Ways to Partition into Palindromes DP ($O(N \log N)$)
**Description:** An elite variation of the Minimum Factorization DP. Given a string $S$, find the total number of ways to partition it into valid palindromic substrings. Using the `slink` and `diff` quick-links, we compress the arithmetic progressions of palindromes to do this in $O(N \log N)$ instead of $O(N^2)$.

```cpp
const int MOD = 1e9 + 7;

void solve_total_partitions(string s) {
    int n = s.size();
    PalindromicTree pt(n, 26, 'a');
    
    vector<long long> dp(n + 1, 0);
    vector<long long> series_dp(n + 5, 0); 
    dp[0] = 1; // 1 way to partition an empty string
    
    for (int i = 1; i <= n; i++) {
        pt.addChar(s[i - 1], i);
        int v = pt.last;
        
        while (v > 1) { 
            series_dp[v] = dp[i - (pt.tree[pt.tree[v].slink].len + pt.tree[v].diff)];
            
            if (pt.tree[v].diff == pt.tree[pt.tree[v].link].diff) {
                series_dp[v] = (series_dp[v] + series_dp[pt.tree[v].link]) % MOD;
            }
            
            dp[i] = (dp[i] + series_dp[v]) % MOD;
            v = pt.tree[v].slink;
        }
    }
    cout << dp[n] << "\n";
}
```

---

## 1.9 Find All Ending Indices of a Specific Palindrome
**Description:** While `tree[u].occ` tells you *how many* times a palindrome occurs, it doesn't give you the indices. By building the explicit Fail Tree and aggregating states, you can fetch every exact ending index in $O(1)$ amortized time per index!

```cpp
// 1. Add `vector<int> ends;` to struct Node.
// 2. In addChar(), add: `tree[last].ends.push_back(pos);`

void fetch_all_occurrences(PalindromicTree& pt) {
    int V = pt.tree.size();
    vector<vector<int>> fail_tree(V);
    
    // Build Fail Tree
    for (int i = 2; i < V; i++) {
        fail_tree[pt.tree[i].link].push_back(i);
    }
    
    // Bottom-Up DFS to merge ending indices from children into parents
    function<void(int)> dfs = [&](int u) {
        for (int v : fail_tree[u]) {
            dfs(v);
            // In C++17, you can use std::set or vector insertion
            pt.tree[u].ends.insert(pt.tree[u].ends.end(), 
                                   pt.tree[v].ends.begin(), pt.tree[v].ends.end());
        }
    };
    
    dfs(0); dfs(1);
    
    // Now pt.tree[u].ends contains every physical ending index of the palindrome 'u'!
}
```
