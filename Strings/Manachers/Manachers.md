# 1. Manacher's Algorithm

## 1.0 Mathematical Properties & Rules
Manacher's Algorithm transforms a string $S$ of length $N$ into a string $T$ of length $2N + 1$ by interleaving a special character (e.g. `#`).
It then computes an array `p` where `p[i]` is the "radius" of the longest palindrome centered at `T[i]`.
- **Time Complexity:** $O(N)$
- **Space Complexity:** $O(N)$
- **Radius to Length:** The actual length of the palindrome centered at `T[i]` in the original string $S$ is `p[i] - 1`.

---

This file contains advanced competitive programming variations (IGM level) and problem-specific methods. **All functions below are designed to be added directly inside `struct Manacher` defined in `Manachers.cpp` (or as standalone helpers).**

## 1.1 Fast Palindrome Checking
**Description:** $O(1)$ query to check if a substring $S[L \dots R]$ (0-indexed) is a palindrome.

```cpp
int getLongest(int cen, bool odd) {
    int pos = 2 * cen + 1 + (!odd);
    return p[pos] - 1;
}

bool checkPalindrome(int l, int r) {
    int len = r - l + 1;
    int cen = (l + r) / 2;
    if (len & 1) return getLongest(cen, true) >= len;
    return getLongest(cen, false) >= len;
}
```

## 1.2 Generic Array/Vector Manacher
**Description:** Adapts Manacher's to find palindromes in a sequence of integers or generic objects (e.g., matching a sequence of differences or custom structs) rather than characters.
*Note: Replace `string s` and `string t` with `vector<T>` inside the struct, and use a dummy value for the `#` separator (like `-1` or `T()`).*

```cpp
template <typename T>
void build_vector(const vector<T>& arr, T separator) {
    vector<T> t = {separator};
    for (auto val : arr) {
        t.push_back(val);
        t.push_back(separator);
    }
    int n = t.size();
    p.assign(n, 1);
    
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i <= r) p[i] = min(p[l + r - i], r - i + 1);
        while (i - p[i] >= 0 && i + p[i] < n && t[i - p[i]] == t[i + p[i]]) p[i]++;
        if (i + p[i] - 1 > r) {
            l = i - p[i] + 1;
            r = i + p[i] - 1;
        }
    }
}
```

## 1.3 Longest Palindromic Substring
**Description:** Returns the longest palindromic substring in $S$ and its starting index.

```cpp
string longest_palindromic_substring() {
    int mx = 0, idx = 0;
    for (int i = 0; i < p.size(); i++) {
        if (p[i] > mx) {
            mx = p[i];
            idx = i;
        }
    }
    int start = (idx - mx + 1) / 2;
    return s.substr(start, mx - 1);
}
```

## 1.4 Total Palindromic Substrings
**Description:** Counts the total number of palindromic substrings in the entire string in $O(N)$.

```cpp
long long count_total_palindromes() {
    long long total = 0;
    for (int i = 0; i < p.size(); i++) {
        total += p[i] / 2;
    }
    return total;
}
```

## 1.5 Palindromes by Exact Length
**Description:** Returns a vector where `ans[L]` is the total number of palindromes of exactly length `L` present in the string. Uses difference arrays in $O(N)$.

```cpp
vector<long long> count_palindromes_by_length() {
    int n = s.size();
    vector<long long> diff(n + 2, 0);
    
    for (int i = 0; i < p.size(); i++) {
        int max_len = p[i] - 1;
        if (max_len == 0) continue;
        
        // Palindromes centered here have lengths:
        // max_len, max_len - 2, max_len - 4... down to 1 or 2
        diff[max_len]++;
    }
    
    // Propagate the differences downwards (steps of 2)
    vector<long long> ans(n + 1, 0);
    for (int len = n; len >= 1; len--) {
        ans[len] += diff[len];
        if (len - 2 >= 1) {
            diff[len - 2] += diff[len];
        }
    }
    return ans;
}
```

## 1.6 Palindromic Prefixes & Suffixes
**Description:** Finds the length of the longest palindromic prefix or suffix in $O(N)$.

```cpp
int longest_palindromic_prefix() {
    int max_len = 0;
    for (int i = 0; i < p.size(); i++) {
        int len = p[i] - 1;
        int start = (i - p[i] + 1) / 2;
        if (start == 0) max_len = max(max_len, len);
    }
    return max_len;
}

int longest_palindromic_suffix() {
    int max_len = 0, n = s.size();
    for (int i = 0; i < p.size(); i++) {
        int len = p[i] - 1;
        int start = (i - p[i] + 1) / 2;
        if (start + len == n) max_len = max(max_len, len);
    }
    return max_len;
}
```

## 1.7 Split into Exactly Two Palindromes
**Description:** Checks if the string can be partitioned into exactly two non-empty palindromes $A$ and $B$ (i.e. $S = A + B$). Time: $O(N)$.

```cpp
bool can_form_two_palindromes() {
    int n = s.size();
    for (int i = 0; i < n - 1; i++) {
        if (checkPalindrome(0, i) && checkPalindrome(i + 1, n - 1)) {
            return true;
        }
    }
    return false;
}
```

## 1.8 Split into Exactly Three Palindromes
**Description:** Checks if the string can be partitioned into exactly three non-empty palindromes $A, B, C$ ($S = A + B + C$). Time: $O(N^2)$.

```cpp
bool can_form_three_palindromes() {
    int n = s.size();
    // Prefixes that are palindromes
    vector<int> valid_prefixes;
    for (int i = 0; i < n - 2; i++) {
        if (checkPalindrome(0, i)) valid_prefixes.push_back(i);
    }
    
    for (int i : valid_prefixes) {
        for (int j = i + 1; j < n - 1; j++) {
            if (checkPalindrome(i + 1, j) && checkPalindrome(j + 1, n - 1)) {
                return true;
            }
        }
    }
    return false;
}
```

## 1.9 Maximum Palindromes Ending/Starting at $i$
**Description:** Computes two arrays in $O(N)$. `left_max[i]` stores the length of the longest palindrome completely contained within $S[0 \dots i]$. `right_max[i]` stores the longest palindrome in $S[i \dots N-1]$.

```cpp
void max_palindromes_ending_starting(vector<int>& left_max, vector<int>& right_max) {
    int n = s.size();
    left_max.assign(n, 0);
    right_max.assign(n, 0);

    for (int i = 0; i < p.size(); i++) {
        int len = p[i] - 1;
        if (len == 0) continue;
        int start = (i - p[i] + 1) / 2;
        int end = start + len - 1;
        
        left_max[end] = max(left_max[end], len);
        right_max[start] = max(right_max[start], len);
    }

    for (int i = 1; i < n; i++) left_max[i] = max(left_max[i], left_max[i - 1]);
    for (int i = n - 2; i >= 0; i--) right_max[i] = max(right_max[i], right_max[i + 1]);
}
```

## 1.10 Maximum Palindrome Product
**Description:** Finds two non-overlapping palindromes in the string that maximize the product of their lengths $Len(A) \times Len(B)$. Time: $O(N)$.

```cpp
long long max_palindrome_product() {
    int n = s.size();
    vector<int> left_max, right_max;
    max_palindromes_ending_starting(left_max, right_max);
    
    long long max_prod = 0;
    for (int i = 0; i < n - 1; i++) {
        max_prod = max(max_prod, 1LL * left_max[i] * right_max[i + 1]);
    }
    return max_prod;
}
```

## 1.11 Number of Palindromes Ending and Starting at Each Index
**Description:** Computes two arrays in $O(N)$: `ends[i]` (number of palindromes ending exactly at $i$) and `starts[i]` (number of palindromes starting exactly at $i$). Uses difference arrays.

```cpp
void count_palindromes_endpoints(vector<long long>& starts, vector<long long>& ends) {
    int n = s.size();
    starts.assign(n + 1, 0);
    ends.assign(n + 1, 0);
    
    for (int i = 0; i < p.size(); i++) {
        int len = p[i] - 1;
        if (len == 0) continue;
        int start = (i - p[i] + 1) / 2;
        int end = start + len - 1;
        
        int cen_idx = i / 2;
        
        // Palindromes ending at 'end', 'end-1' ... down to 'cen_idx'
        ends[cen_idx]++;
        ends[end + 1]--;
        
        // Palindromes starting at 'start', 'start+1' ... up to 'cen_idx' (or cen_idx+1 for even)
        int mid_start = (i % 2 == 1) ? cen_idx : cen_idx + 1;
        starts[start]++;
        starts[mid_start + 1]--;
    }
    
    for (int i = 1; i < n; i++) {
        ends[i] += ends[i - 1];
        starts[i] += starts[i - 1];
    }
    ends.pop_back();
    starts.pop_back();
}
```

## 1.12 Count Adjacent Palindromic Pairs
**Description:** Counts the number of pairs of palindromic substrings $(A, B)$ such that $A$ ends exactly where $B$ begins (i.e. $B$ starts at index $i+1$ and $A$ ends at index $i$). Time: $O(N)$.

```cpp
long long count_palindromic_pairs() {
    int n = s.size();
    vector<long long> starts, ends;
    count_palindromes_endpoints(starts, ends);
    
    long long total_pairs = 0;
    for (int i = 0; i < n - 1; i++) {
        total_pairs += ends[i] * starts[i + 1];
    }
    return total_pairs;
}
```

## 1.13 Min Cuts for Palindrome Partitioning
**Description:** Uses $O(1)$ `checkPalindrome` to find the minimum number of cuts required to partition $S$ into palindromes. Time: $O(N^2)$.

```cpp
int min_cuts_partitioning() {
    int n = s.size();
    vector<int> dp(n, 1e9);
    
    for (int i = 0; i < n; i++) {
        if (checkPalindrome(0, i)) {
            dp[i] = 0;
            continue;
        }
        for (int j = 0; j < i; j++) {
            if (checkPalindrome(j + 1, i)) {
                dp[i] = min(dp[i], dp[j] + 1);
            }
        }
    }
    return dp[n - 1];
}
```

## 1.14 Longest Palindromic Substring in Range $[L, R]$ (Sparse Table + Binary Search)
**Description:** Given queries $[L, R]$, find the length of the longest palindrome fully contained in $S[L \dots R]$. 
**Implementation:** Build a Sparse Table over the `p` array. For a query, binary search the answer length $K$. Check if there exists a center in the valid center-range for $[L, R]$ that has $p[i] - 1 \ge K$. Time: $O(N \log N + Q \log N)$.

```cpp
// Check if a palindrome of length K exists completely within S[L...R]
bool can_form_palindrome(int L, int R, int K, auto& ST) {
    if (K > R - L + 1) return false;
    
    int min_start = L;
    int max_start = R - K + 1;
    
    int min_pos = 2 * min_start + K;
    int max_pos = 2 * max_start + K;
    
    return ST.query(min_pos, max_pos) - 1 >= K;
}

int query_longest_palindrome(int L, int R, auto& ST) {
    int low = 1, high = R - L + 1, ans = 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (can_form_palindrome(L, R, mid, ST)) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}
```

## 1.15 Total Palindromic Substrings in Range $[L, R]$
**Description:** Given $Q$ queries, find the total number of palindromes fully contained in $S[L \dots R]$.
**Implementation:** A highly advanced $O((N + Q) \log N)$ offline algorithm. It maps the Manacher radii to a 2D constraint and uses two Fenwick trees with sweep-line to sum the exact boundaries of palindromes completely enveloped by the query range.

```cpp
struct Query {
    int id, L, R, l, r, mid;
    long long ans;
};

struct Fenwick {
    int n;
    vector<long long> tree;
    Fenwick(int n) : n(n), tree(n + 1, 0) {}
    void add(int i, long long delta) {
        for (; i <= n; i += i & -i) tree[i] += delta;
    }
    long long query(int i) {
        long long sum = 0;
        for (; i > 0; i -= i & -i) sum += tree[i];
        return sum;
    }
    long long query(int l, int r) {
        if (l > r) return 0;
        return query(r) - query(l - 1);
    }
};

vector<long long> count_palindromes_in_ranges(const vector<pair<int, int>>& queries) {
    int m = p.size(); // 2N + 1
    int Q = queries.size();
    
    vector<Query> qs(Q);
    for (int i = 0; i < Q; i++) {
        qs[i].id = i;
        qs[i].L = queries[i].first;
        qs[i].R = queries[i].second;
        qs[i].l = 2 * qs[i].L + 1;
        qs[i].r = 2 * qs[i].R + 1;
        qs[i].mid = qs[i].L + qs[i].R + 1;
        qs[i].ans = 0;
    }
    
    // Part 1: i in [l, mid], Threshold V = -2L. X_i = A_i - i
    vector<pair<int, int>> X(m);
    for (int i = 1; i < m; i++) X[i] = {p[i] - 1 - i, i};
    sort(X.begin() + 1, X.end());
    
    auto sort_by_L = [](const Query& a, const Query& b) { return -2 * a.L < -2 * b.L; };
    sort(qs.begin(), qs.end(), sort_by_L);
    
    Fenwick bit_A(m), bit_i(m), bit_cnt(m);
    for (int i = 1; i < m; i++) {
        bit_i.add(i, i);
        bit_cnt.add(i, 1);
    }
    
    int ptr = 1;
    for (auto& q : qs) {
        if (q.l > q.mid) continue;
        int V = -2 * q.L;
        while (ptr < m && X[ptr].first <= V) {
            int idx = X[ptr].second;
            bit_cnt.add(idx, -1);
            bit_i.add(idx, -idx);
            bit_A.add(idx, p[idx] - 1);
            ptr++;
        }
        q.ans += bit_A.query(q.l, q.mid) + bit_i.query(q.l, q.mid) - 2LL * q.L * bit_cnt.query(q.l, q.mid);
    }
    
    // Part 2: i in [mid+1, r], Threshold V = 2R+2. Y_i = A_i + i
    vector<pair<int, int>> Y(m);
    for (int i = 1; i < m; i++) Y[i] = {p[i] - 1 + i, i};
    sort(Y.begin() + 1, Y.end());
    
    auto sort_by_R = [](const Query& a, const Query& b) { return 2 * a.R + 2 < 2 * b.R + 2; };
    sort(qs.begin(), qs.end(), sort_by_R);
    
    Fenwick bit_A2(m), bit_i2(m), bit_cnt2(m);
    for (int i = 1; i < m; i++) {
        bit_i2.add(i, i);
        bit_cnt2.add(i, 1);
    }
    
    ptr = 1;
    for (auto& q : qs) {
        if (q.mid + 1 > q.r) continue;
        int V = 2 * q.R + 2;
        while (ptr < m && Y[ptr].first <= V) {
            int idx = Y[ptr].second;
            bit_cnt2.add(idx, -1);
            bit_i2.add(idx, -idx);
            bit_A2.add(idx, p[idx] - 1);
            ptr++;
        }
        q.ans += bit_A2.query(q.mid + 1, q.r) + 1LL * V * bit_cnt2.query(q.mid + 1, q.r) - bit_i2.query(q.mid + 1, q.r);
    }
    
    // Restore original query order and add parity
    vector<long long> final_ans(Q);
    for (auto& q : qs) {
        long long odd_count = (q.r + 1) / 2 - q.l / 2;
        final_ans[q.id] = (q.ans + odd_count) / 2;
    }
    return final_ans;
}
```
