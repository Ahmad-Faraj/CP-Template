# 1. Suffix Array & LCP Array

## 1.0 Overview & Vector Compatibility
The Suffix Array sorts all suffixes of a string lexicographically in $O(N \log N)$ time. The associated LCP (Longest Common Prefix) array computes the prefix similarity between adjacent sorted suffixes in $O(N)$ time via Kasai's algorithm.

> [!TIP]
> **Using Suffix Array on `vector<int>`**
> The core `SuffixArray` struct is perfectly compatible with integer arrays! To convert it:
> 1. Change `string s;` to `vector<int> s;`
> 2. Instead of `s += "$";`, append a strictly smaller integer delimiter: `s.push_back(0);` or `s.push_back(-1);`
> 3. Adjust the substring extraction in `lower`/`upper` bound checks to iterate vectors instead of strings.
>
> **Why do this?** String matching on graph node paths, large compressed alphabets, or coordinate geometries!

---

## 1.0.1 Core Suffix Array Implementation ($O(N \log N)$)
**Description:** The base struct responsible for building the SA, Kasai's LCP array, and initializing the RMQ Sparse Table. Copy-paste this directly into your template.

```cpp
struct SuffixArray {
    string s;
    int n, LOG;
    vector<int> p, c, lcp;
    vector<vector<int>> mn;

    explicit SuffixArray() = default;

    explicit SuffixArray(string s) : s(s), n((int)s.size() + 1) {
        this->s += "$"; // '$' is strictly less than any character in the string
        p = c = lcp = vector<int>(n);
        build();
        build_lcp();
        build_sparse_table();
    }

    inline void count_sort() {
        vector<int> cnt(n, 0), pos(n, 0), p_new(n);
        for (auto x : c) cnt[x]++;
        for (int i = 1; i < n; i++) pos[i] = pos[i - 1] + cnt[i - 1];
        for (auto x : p) p_new[pos[c[x]]++] = x;
        p = p_new;
    }

    inline void build() {
        vector<pair<char, int>> a(n);
        for (int i = 0; i < n; i++) a[i] = {s[i], i};
        sort(a.begin(), a.end());
        for (int i = 0; i < n; i++) p[i] = a[i].second;
        c[p[0]] = 0;

        for (int i = 1; i < n; i++)
            c[p[i]] = c[p[i - 1]] + (a[i].first != a[i - 1].first);

        int k = 0;
        while ((1 << k) < n && p.back() != n - 1) {
            for (int i = 0; i < n; i++)
                p[i] = (p[i] - (1 << k) + n) % n;
            count_sort();
            vector<int> c_new(n);
            c_new[p[0]] = 0;
            for (int i = 1; i < n; i++) {
                pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << k)) % n]};
                pair<int, int> now = {c[p[i]], c[(p[i] + (1 << k)) % n]};
                c_new[p[i]] = c_new[p[i - 1]] + (prev != now);
            }
            c = c_new;
            k++;
        }
    }

    inline void build_lcp() {
        int k = 0;
        for (int i = 0; i < n - 1; i++) {
            int pi = c[i];
            int j = p[pi - 1];
            while (s[i + k] == s[j + k]) k++;
            lcp[pi] = k;
            k = max(k - 1, 0);
        }
    }

    inline void build_sparse_table() {
        LOG = 32 - __builtin_clz(n);
        mn = vector<vector<int>>(n, vector<int>(LOG));
        for (int i = 1; i < n; i++) mn[i][0] = lcp[i];
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                mn[i][j] = min(mn[i][j - 1], mn[i + (1 << (j - 1))][j - 1]);
            }
        }
    }

    inline int compare(const int i, const int j, const int l) {
        assert(0 <= i && i < n && 0 <= j && j < n);
        pair<int, int> a = {c[i], c[(i + l - 1) % n]};
        pair<int, int> b = {c[j], c[(j + l - 1) % n]};
        return a == b ? 0 : (a < b ? -1 : 1);
    }

    inline int compare(const int i, const string &pattern) {
        return s.compare(i, pattern.size(), pattern);
    }

    inline bool find(const string &pattern) {
        int l = 0, r = n - 1;
        while (l <= r) {
            int mid = (l + r) >> 1;
            int res = compare(p[mid], pattern);
            if (res == 0) return true;
            if (res < 0) l = mid + 1;
            else r = mid - 1;
        }
        return false;
    }

    inline int lower(const string &pattern) {
        return lower_bound(p.begin(), p.end(), pattern, [&](int i, const string &pat) {
            return s.substr(i, pat.size()) < pat;
        }) - p.begin();
    }

    inline int upper(const string &pattern) {
        return upper_bound(p.begin(), p.end(), pattern, [&](const string &pat, int i) {
            return s.substr(i, pat.size()) > pat;
        }) - p.begin() - 1;
    }

    inline int count(const string &pattern) {
        return upper(pattern) - lower(pattern) + 1;
    }
    
    // O(1) Longest Common Prefix between suffix starting at i and suffix starting at j
    inline int longest_common_prefix(int i, int j) {
        if (i == j) return n - i - 1;
        int l = min(c[i], c[j]), r = max(c[i], c[j]), len = r - l;
        int k = 31 - __builtin_clz(len);
        return min(mn[l + 1][k], mn[r - (1 << k) + 1][k]);
    }
};
```

---

## 1.1 Distinct Substrings
**Description:** Every substring is a prefix of some suffix. We can count the total number of distinct substrings in $O(N)$ time by subtracting the LCP from the total length of each suffix.

```cpp
long long distinct_substrings(SuffixArray& sa) {
    long long ans = 0;
    int n = sa.n;
    // p[i] is the starting index of the i-th lexicographically sorted suffix
    // It has length: (n - 1) - p[i]
    for (int i = 1; i < n; i++) {
        ans += (n - 1 - sa.p[i]) - sa.lcp[i];
    }
    return ans;
}
```

---

## 1.2 Longest Repeated Substring
**Description:** Finds the longest substring that appears at least twice in the string. The length of this substring is exactly the maximum value in the LCP array.

```cpp
string longest_repeated_substring(SuffixArray& sa) {
    int idx = max_element(sa.lcp.begin(), sa.lcp.end()) - sa.lcp.begin();
    return sa.s.substr(sa.p[idx], sa.lcp[idx]);
}
```

---

## 1.3 K-th Lexicographically Smallest Substring
**Description:** Finds the $K$-th substring in $O(N)$ time by traversing the explicitly sorted suffixes and skipping overlaps defined by the LCP array.

```cpp
string kth_substring(long long k, SuffixArray& sa) {
    int n = sa.s.size() - 1; // subtract delimiter
    for (int i = 1; i <= n; i++) {
        long long current_distinct_prefixes = (n - sa.p[i]) - sa.lcp[i];
        if (k <= current_distinct_prefixes) {
            return sa.s.substr(sa.p[i], sa.lcp[i] + k);
        }
        k -= current_distinct_prefixes;
    }
    return "No such substring.";
}
```

---

## 1.4 Longest Common Substring (LCS) of Two Strings
**Description:** Finds the longest common substring between strings A and B by concatenating them with unique delimiters `A + "#" + B + "$"`. The LCS is the maximum LCP between two adjacent suffixes that belong to different original strings!

```cpp
string longest_common_substring(const string &a, const string &b) {
    string combined = a + "#" + b;
    SuffixArray sa(combined); // internal '$' added automatically
    
    int lenA = a.size();
    int max_len = 0, best_idx = 0;
    
    for (int i = 1; i < sa.n; i++) {
        // Check if one suffix starts in A and the other in B
        bool inA1 = sa.p[i] < lenA;
        bool inA2 = sa.p[i - 1] < lenA;
        
        if (inA1 != inA2) {
            if (sa.lcp[i] > max_len) {
                max_len = sa.lcp[i];
                best_idx = sa.p[i];
            }
        }
    }
    return sa.s.substr(best_idx, max_len);
}
```

---

## 1.5 Longest Palindromic Substring via Suffix Array
**Description:** By concatenating a string $S$ with its reverse $S^R$ using a delimiter (e.g., $S + \text{'\#'} + S^R$), you can find the longest palindrome in $O(N \log N)$ (or $O(N)$) using the sparse table RMQ `longest_common_prefix`.
- Odd Palindrome at $i$: `LCP(i, 2N - i)`
- Even Palindrome at $i$: `LCP(i, 2N - i + 1)`

```cpp
// This provides O(1) palindrome expansions, a powerful alternative to Manacher's!
int longest_palindrome(string s) {
    int len = s.size();
    string rev_s = s;
    reverse(rev_s.begin(), rev_s.end());
    
    string combined = s + "#" + rev_s;
    SuffixArray sa(combined);
    
    int max_pal = 0;
    for (int i = 0; i < len; i++) {
        // Odd length palindrome centered at i
        int odd_lcp = sa.longest_common_prefix(i, combined.size() - i - 1);
        max_pal = max(max_pal, 2 * odd_lcp - 1);
        
        // Even length palindrome centered between i and i+1
        if (i + 1 < len) {
            int even_lcp = sa.longest_common_prefix(i + 1, combined.size() - i - 1);
            max_pal = max(max_pal, 2 * even_lcp);
        }
    }
    return max_pal;
}
```

---

## 1.6 Burrows-Wheeler Transform (BWT)
**Description:** The BWT rearranges a character string into runs of similar characters. It is the core algorithm behind `bzip2` compression. You can extract the BWT directly from the Suffix Array in $O(N)$ time!
- **Formula:** `bwt[i] = s[p[i] - 1]`. If `p[i] == 0`, it wraps around to `s[N - 1]`.

```cpp
string get_bwt(SuffixArray& sa) {
    string bwt = "";
    int n = sa.n;
    for (int i = 0; i < n; i++) {
        if (sa.p[i] == 0) {
            bwt += sa.s[n - 1]; // The '$' delimiter
        } else {
            bwt += sa.s[sa.p[i] - 1];
        }
    }
    return bwt;
}
```

---

## 1.7 Longest Common Substring of $M$ Strings (Sliding Window + Monotonic Queue)
**Description:** The ultimate extension of the LCS problem. Find the longest substring that appears in ALL $M$ strings. We concatenate all strings using uniquely incrementing delimiters, then use a sliding window over the Suffix Array to maintain a segment containing at least one suffix from each string. A Monotonic Queue instantly extracts the minimum LCP in that window. Time Complexity: $O(N)$.

```cpp
string longest_common_substring_multi(vector<string>& strings) {
    int m = strings.size();
    if(m == 0) return "";
    if(m == 1) return strings[0];
    
    string combined = "";
    vector<int> start_idx;
    for(int i=0; i<m; i++){
        start_idx.push_back(combined.size());
        combined += strings[i];
        combined += (char)(1 + i); // Low ASCII values as unique delimiters
    }
    
    SuffixArray sa(combined);
    vector<int> id(sa.n, -1);
    for(int i=0; i<sa.n; i++){
        for(int j=0; j<m; j++){
            if(sa.p[i] >= start_idx[j] && sa.p[i] < start_idx[j] + strings[j].size()){
                id[i] = j; break;
            }
        }
    }
    
    vector<int> count(m, 0);
    int unique_strings = 0;
    int L = 0, max_lcp = 0, best_idx = -1;
    
    deque<int> min_q;
    for(int R = 0; R < sa.n; R++){
        if(id[R] != -1){
            if(count[id[R]] == 0) unique_strings++;
            count[id[R]]++;
        }
        
        while(!min_q.empty() && sa.lcp[min_q.back()] >= sa.lcp[R]) min_q.pop_back();
        min_q.push_back(R);
        
        while(unique_strings == m){
            int current_lcp = sa.lcp[min_q.front()];
            if(current_lcp > max_lcp){
                max_lcp = current_lcp;
                best_idx = sa.p[R];
            }
            
            if(id[L] != -1){
                count[id[L]]--;
                if(count[id[L]] == 0) unique_strings--;
            }
            L++;
            if(!min_q.empty() && min_q.front() <= L) min_q.pop_front();
        }
    }
    if(best_idx == -1) return "";
    return combined.substr(best_idx, max_lcp);
}
```

---

## 1.8 Maximizing `Frequency * Length` (Monotonic Stack on LCP)
**Description:** Equivalent to finding the "Largest Rectangle in a Histogram", this $O(N)$ algorithm leverages a Monotonic Stack to find the specific repeating substring whose product of `length` multiplied by its `total occurrences` is maximized.

```cpp
long long max_freq_length(SuffixArray& sa) {
    long long max_ans = sa.s.size() - 1; // At least the entire string occurs once
    int n = sa.n;
    stack<int> st;
    vector<int> L(n), R(n);
    
    for(int i = 1; i < n; i++){
        while(!st.empty() && sa.lcp[st.top()] >= sa.lcp[i]) st.pop();
        L[i] = st.empty() ? 1 : st.top() + 1;
        st.push(i);
    }
    while(!st.empty()) st.pop();
    for(int i = n - 1; i >= 1; i--){
        while(!st.empty() && sa.lcp[st.top()] >= sa.lcp[i]) st.pop();
        R[i] = st.empty() ? n - 1 : st.top() - 1;
        st.push(i);
    }
    
    for(int i = 1; i < n; i++){
        // Frequency is (R - L + 2) because LCP between adjacent elements implies
        // they span across (R - L + 2) actual suffixes.
        long long freq = R[i] - L[i] + 2;
        max_ans = max(max_ans, freq * sa.lcp[i]);
    }
    return max_ans;
}
```

---

## 1.9 $O(\log N)$ Arbitrary Substring Frequency Query (Binary Search)
**Description:** Instead of running $O(|P| \log N)$ string matching, if you already know the starting index `pos` and `len` of a substring, you can instantly find how many times it occurs in the whole string by Binary Searching the boundaries over the Sparse Table RMQ!

```cpp
int substring_frequency(SuffixArray& sa, int pos, int len) {
    int rank = sa.c[pos];
    int n = sa.n;
    
    // Binary search left bound
    int low = 1, high = rank, left_bound = rank;
    while(low <= high){
        int mid = low + (high - low) / 2;
        if(sa.longest_common_prefix(sa.p[mid], sa.p[rank]) >= len){
            left_bound = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    
    // Binary search right bound
    low = rank, high = n - 1, right_bound = rank;
    while(low <= high){
        int mid = low + (high - low) / 2;
        if(sa.longest_common_prefix(sa.p[rank], sa.p[mid]) >= len){
            right_bound = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return right_bound - left_bound + 1;
}
```

---

## 1.10 $O(1)$ Arbitrary Substring Lexicographical Comparison
**Description:** Compare any two completely arbitrary substrings (defined by their starting index and length) in pure $O(1)$ time using the RMQ Sparse Table.

```cpp
// Compare S[i .. i+L1-1] and S[j .. j+L2-1]
int compare_substrings(SuffixArray& sa, int i, int L1, int j, int L2) {
    int lcp = sa.longest_common_prefix(i, j);
    if(lcp >= min(L1, L2)){
        if(L1 == L2) return 0;
        return L1 < L2 ? -1 : 1;
    }
    return sa.s[i + lcp] < sa.s[j + lcp] ? -1 : 1;
}
```

---

## 1.11 Lempel-Ziv 77 (LZ77) Factorization ($O(N \log N)$)
**Description:** LZ77 breaks a string into compressed blocks, where each block is the longest prefix that has appeared previously in the string. By maintaining an active `std::set` of Suffix Array ranks, we can query the closest lexicographical neighbors to instantly find the absolute longest previous match!

```cpp
vector<pair<int, int>> lz77_factorization(SuffixArray& sa) {
    int n = sa.s.size() - 1; // Ignoring delimiter
    vector<pair<int, int>> blocks; // {start_index, length}
    
    set<int> active_ranks;
    active_ranks.insert(sa.c[0]);
    
    int i = 1;
    while(i < n){
        int rank = sa.c[i];
        active_ranks.insert(rank);
        
        auto it = active_ranks.find(rank);
        int max_lcp = 0, best_pos = -1;
        
        // Check the closest lexicographical neighbor smaller than rank
        if(it != active_ranks.begin()){
            auto prev_it = prev(it);
            int cand_pos = sa.p[*prev_it];
            int l = sa.longest_common_prefix(cand_pos, i);
            if(l > max_lcp) { max_lcp = l; best_pos = cand_pos; }
        }
        // Check the closest lexicographical neighbor larger than rank
        if(next(it) != active_ranks.end()){
            auto next_it = next(it);
            int cand_pos = sa.p[*next_it];
            int l = sa.longest_common_prefix(cand_pos, i);
            if(l > max_lcp) { max_lcp = l; best_pos = cand_pos; }
        }
        
        if(max_lcp == 0){
            blocks.push_back({i, 1}); // No previous match, output single char block
            i++;
        } else {
            blocks.push_back({best_pos, max_lcp});
            // Add all skipped positions to active_ranks so they can be matched later
            for(int j = 1; j < max_lcp; j++) active_ranks.insert(sa.c[i + j]);
            i += max_lcp;
        }
    }
    return blocks;
}
```

---

## 1.12 Cartesian Tree on LCP (Suffix Tree Internal Nodes)
**Description:** Building a Suffix Tree directly using Ukkonen's algorithm is notoriously painful and buggy. However, the internal nodes of a Suffix Tree perfectly map to a **Cartesian Tree** built over the LCP array! You can construct the Suffix Tree topology natively in $O(N)$ time using a monotonic stack.

```cpp
// Returns the left_child and right_child arrays defining the Cartesian Tree
void build_suffix_tree_topology(SuffixArray& sa) {
    int n = sa.n;
    vector<int> left_child(n, -1), right_child(n, -1), parent(n, -1);
    stack<int> st;
    
    for (int i = 1; i < n; i++) {
        int last_popped = -1;
        // Maintain a strictly increasing stack based on LCP values
        while (!st.empty() && sa.lcp[st.top()] > sa.lcp[i]) {
            last_popped = st.top();
            st.pop();
        }
        if (last_popped != -1) left_child[i] = last_popped;
        if (!st.empty()) right_child[st.top()] = i;
        st.push(i);
    }
    
    // The roots of this forest form the backbone of the Suffix Tree!
}
```

---

## 1.13 String Matching with $K$ Mismatches ($O(|T| \cdot K)$)
**Description:** If you need to find all occurrences of a Pattern $P$ in Text $T$, but you are allowed up to $K$ character mismatches per occurrence, standard $O(N^2)$ DP is too slow. By concatenating $T + \text{'\#'} + P$ and using $O(1)$ LCP jumps, we can skip matching blocks instantly and only process the mismatches!

```cpp
vector<int> match_with_k_mismatches(string T, string P, int K) {
    string combined = T + "#" + P + "$";
    SuffixArray sa(combined);
    vector<int> matches;
    
    int t_len = T.size(), p_len = P.size();
    int p_start = t_len + 1; // Index where P starts in combined string
    
    for (int i = 0; i <= t_len - p_len; i++) {
        int mismatches = 0;
        int t_idx = i, p_idx = p_start;
        int matched = 0;
        
        while (matched < p_len && mismatches <= K) {
            // Instantly skip the matching prefix
            int lcp = sa.longest_common_prefix(t_idx, p_idx);
            
            // Do not exceed the pattern length boundary
            if (matched + lcp > p_len) lcp = p_len - matched;
            
            matched += lcp; t_idx += lcp; p_idx += lcp;
            
            // If we haven't finished matching the pattern, we hit a mismatch
            if (matched < p_len) {
                mismatches++;
                matched++; t_idx++; p_idx++;
            }
        }
        if (mismatches <= K) matches.push_back(i);
    }
    return matches;
}
```

---

## 1.14 Offline Range Substring Occurrences Queries (2D Counting)
**Description:** Given queries of the form: "How many times does Pattern $P$ occur strictly within $S[L..R]$?", we can solve this offline using the Suffix Array + Merge Sort Tree (or Persistent Segment Tree).
1. Binary search the SA to find the range `[left_bound, right_bound]` of suffixes starting with $P$.
2. The problem perfectly reduces to: Count how many values $SA[i]$ (for $i \in [\text{left\_bound}, \text{right\_bound}]$) fall strictly within the physical index range $[L, R - |P| + 1]$.
3. This is solved in $O(\log^2 N)$ per query using a Merge Sort Tree on the Suffix Array!

```cpp
struct MergeSortTree {
    vector<vector<int>> tree;
    int n;
    
    MergeSortTree(const vector<int>& a) {
        n = a.size();
        tree.resize(4 * n);
        build(1, 0, n - 1, a);
    }
    
    void build(int node, int start, int end, const vector<int>& a) {
        if (start == end) {
            tree[node].push_back(a[start]);
            return;
        }
        int mid = (start + end) / 2;
        build(2 * node, start, mid, a);
        build(2 * node + 1, mid + 1, end, a);
        merge(tree[2 * node].begin(), tree[2 * node].end(),
              tree[2 * node + 1].begin(), tree[2 * node + 1].end(),
              back_inserter(tree[node]));
    }
    
    // Returns number of elements in a[L..R] that fall within [valL, valR]
    int query(int node, int start, int end, int l, int r, int valL, int valR) {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) {
            return upper_bound(tree[node].begin(), tree[node].end(), valR) -
                   lower_bound(tree[node].begin(), tree[node].end(), valL);
        }
        int mid = (start + end) / 2;
        return query(2 * node, start, mid, l, r, valL, valR) + 
               query(2 * node + 1, mid + 1, end, l, r, valL, valR);
    }
};

// Evaluates a single query in O( |P| log N + log^2 N )
int query_substring_range(SuffixArray& sa, MergeSortTree& mst, const string& pattern, int L, int R) {
    int p_len = pattern.size();
    
    // 1. Binary search the SA array to find the contiguous range of suffixes 
    int sa_left = sa.lower(pattern);
    int sa_right = sa.upper(pattern);
    
    if (sa_left > sa_right) return 0; // Pattern doesn't exist in the whole string
    
    // 2. Valid starting indices for the pattern must be >= L and <= R - p_len + 1
    int valL = L;
    int valR = R - p_len + 1;
    
    if (valL > valR) return 0; // The query range is smaller than the pattern itself
    
    // 3. 2D Query: How many physical text indices in sa.p[sa_left ... sa_right] fall inside [valL, valR]?
    return mst.query(1, 0, sa.n - 1, sa_left, sa_right, valL, valR);
}

// Inside solve():
// SuffixArray sa(S);
// MergeSortTree mst(sa.p);
```

---

## 1.15 Minimal Cyclic Shift (Lexicographically Smallest Rotation)
**Description:** Booth's algorithm finds the minimal cyclic shift in $O(N)$. However, if you already have a Suffix Array built, you can find it instantly by concatenating the string to itself $S + S$. The minimal rotation is simply the very first suffix in the SA whose starting index is less than $|S|$!

```cpp
string minimal_rotation(string s) {
    int n = s.size();
    string combined = s + s + "$";
    SuffixArray sa(combined);
    
    // Scan the sorted suffixes from smallest to largest
    for (int i = 1; i < sa.n; i++) {
        // The first one starting in the first half of S+S is the minimal rotation!
        if (sa.p[i] < n) {
            return combined.substr(sa.p[i], n);
        }
    }
    return "";
}
```
