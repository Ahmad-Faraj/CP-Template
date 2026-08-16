# 1. KMP (Knuth-Morris-Pratt) Algorithm

## 1.0 Mathematical Properties & Rules
The KMP algorithm computes an array `lp` (also known as the prefix function or $\pi$ array) in $O(N)$ time.
- **Definition:** `lp[i]` is the length of the **longest proper prefix** of the substring `pattern[0...i]` that is also a suffix of this substring.
- **Time Complexity:** $O(N)$ to build the array.
- **Space Complexity:** $O(N)$ to store the `lp` array.

---

This file contains competitive programming variations and problem-specific methods. **All functions below are designed to be added directly inside `struct KMP` defined in `KMP.cpp` (or as standalone helpers leveraging the structure).**

## 1.1 Match Pattern in Text
**Description:** Finds all starting indices (0-based) of `pattern` in a given string `text`. Time: $O(N + M)$.

```cpp
vector<int> match(const string &text) {
    int n = text.size(), m = pattern.size();
    vector<int> ret;
    for (int i = 0, k = 0; i < n; i++) {
        k = failure(k, text[i]);
        if (k == m) {
            ret.push_back(i - m + 1); // 0-based indexing
        }
    }
    return ret;
}
```

## 1.2 Online Matching (Streaming Characters)
**Description:** Processes characters one by one from a stream. It maintains the current state `k` (the number of matched characters) and updates it for the new character `c`.

```cpp
// Call this repeatedly for each character in a stream
// Initialize k = 0 before the stream starts.
// Returns the new state k. If k == pattern.size(), a match is found!
int process_char(int k, char c) {
    return failure(k, c);
}
```

## 1.3 Smallest Period of a String
**Description:** A string $S$ of length $N$ has a period length $K$ if $S$ can be formed by concatenating a substring of length $K$ multiple times.
- **Theorem:** The smallest repeating period length is $K = N - lp[N-1]$. If $N \pmod K == 0$, the string is perfectly periodic.

```cpp
// Returns the length of the smallest repeating period of the pattern.
// If it's not perfectly periodic, it returns N.
int get_smallest_period() {
    int n = pattern.size();
    int k = n - lp[n - 1];
    if (n % k == 0) return k;
    return n; 
}
```

## 1.4 Get All Valid Periods of a String
**Description:** A period doesn't have to perfectly divide the string (e.g., `aba` has a period of 2: `ab|a`). This function returns all valid lengths $P$ such that $S[i] == S[i + P]$ for all valid $i$.

```cpp
vector<int> get_all_periods() {
    int n = pattern.size();
    vector<int> periods;
    // Every border of length L gives a period of length N - L
    for (int i = lp[n - 1]; i > 0; i = lp[i - 1]) {
        periods.push_back(n - i);
    }
    periods.push_back(n); // The string itself is always a period
    return periods;
}
```

## 1.5 Count Prefix Occurrences in the Pattern Itself
**Description:** Computes an array `ans` where `ans[i]` is the number of times the prefix of length `i` appears within the `pattern` itself. Uses dynamic programming on the failure tree from leaves to root.

```cpp
vector<int> count_prefixes_in_pattern() {
    int n = pattern.size();
    vector<int> ans(n + 1, 1); // Every prefix appears at least once (itself)
    ans[0] = 0;
    
    // DP down the failure tree
    for (int i = n - 1; i > 0; i--) {
        if (lp[i] > 0) {
            ans[lp[i]] += ans[i + 1];
        }
    }
    return ans;
}
```

## 1.6 Count Prefix Occurrences in Another String
**Description:** Counts the number of times each prefix of `pattern` appears in a different string `t`.

```cpp
vector<int> countInAnother(const string &t) {
    string s = pattern + "#" + t;
    int n = s.size();
    KMP kmp(s); // Create a new KMP instance for the combined string
    
    vector<int> cnt(n + 1, 0);
    for (int i = pattern.size() + 1; i < n; i++) cnt[kmp.lp[i]]++;
    
    // DP down the failure tree
    for (int i = pattern.size(); i > 0; i--) {
        if (kmp.lp[i - 1] > 0) {
            cnt[kmp.lp[i - 1]] += cnt[i];
        }
    }
    
    vector<int> ans;
    for (int i = 1; i <= pattern.size(); i++) ans.push_back(cnt[i]);
    return ans;
}
```

## 1.7 Get All Borders (Prefixes that are Suffixes)
**Description:** A border of a string is a prefix that is also a suffix but not the whole string. Returns lengths of all valid borders in increasing order.

```cpp
vector<int> get_borders() {
    int m = pattern.size();
    vector<int> indexes = {m};
    for (int i = lp[m - 1]; i > 0; i = lp[i - 1]) {
        indexes.push_back(i);
    }
    sort(indexes.begin(), indexes.end());
    return indexes;
}
```

## 1.8 Build Non-Overlapping Longest Suffix
**Description:** Modifies the KMP algorithm to find the longest proper prefix that is also a suffix, with the strict condition that they do **not overlap**.
*Note: This replaces the standard `build()` logic.*

```cpp
void build_non_overlapping() {
    int n = pattern.size();
    lp.assign(n, 0);
    for (int i = 1; i < n; i++) {
        int j = lp[i - 1];
        while (j > 0 && pattern[j] != pattern[i]) j = lp[j - 1];
        lp[i] = j + (pattern[j] == pattern[i]);
        
        // Non-overlapping condition check
        if (lp[i] * 2 > i + 1) {    
            lp[i] = lp[lp[i] - 1];
        }
    }
}
```

## 1.9 The Failure Tree (Prefix Tree)
**Description:** The array `lp` defines a tree where the parent of node `i` is `lp[i-1]`. In this tree, ancestors of node `i` correspond to all borders (prefix-suffixes) of `pattern[0...i]`. 

```cpp
// Builds the directed Failure Tree (edges from lp[i-1] to i)
vector<vector<int>> build_failure_tree() {
    int n = pattern.size();
    vector<vector<int>> adj(n + 1);
    
    // The root is node 0 (representing the empty prefix)
    for (int i = 1; i <= n; i++) {
        adj[lp[i - 1]].push_back(i);
    }
    return adj;
}
```

## 1.10 Maximum Overlap of Two Strings (Shortest Superstring)
**Description:** Given two strings $A$ and $B$, finds the length of the longest suffix of $A$ that exactly matches a prefix of $B$.
*Note: This is a standalone helper.*

```cpp
int max_overlap(const string& a, const string& b) {
    KMP kmp(b);
    int k = 0;
    for (char c : a) {
        k = kmp.failure(k, c);
    }
    return k; 
}
```

## 1.11 Constructing a Valid String from a $\pi$-Array
**Description:** Given a valid $\pi$-array (KMP `lp` array), constructs the lexicographically smallest string.

```cpp
string build_from_pi(const vector<int>& pi) {
    int n = pi.size();
    if (n == 0) return "";
    string s = "a";
    for (int i = 1; i < n; i++) {
        if (pi[i] > 0) {
            s += s[pi[i] - 1];
        } else {
            vector<bool> used(26, false);
            int j = pi[i - 1];
            while (j > 0) {
                used[s[j] - 'a'] = true;
                j = pi[j - 1];
            }
            used[s[0] - 'a'] = true;
            char c = 'b';
            while (used[c - 'a']) c++;
            s += c;
        }
    }
    return s;
}
```

## 1.12 Generic Array/Vector Matching
**Description:** Adapts KMP to match a sequence of integers or generic objects (e.g., matching a sequence of differences).
*Note: Replace `string pattern` with `vector<T> pattern` in `struct KMP`.*

```cpp
template <typename T>
vector<int> match_vector(const vector<T>& text) {
    int n = text.size(), m = pattern.size();
    vector<int> ret;
    for (int i = 0, k = 0; i < n; i++) {
        while (k > 0 && pattern[k] != text[i]) {
            k = lp[k - 1];
        }
        k += (pattern[k] == text[i]);
        if (k == m) {
            ret.push_back(i - m + 1);
        }
    }
    return ret;
}
```

## 1.13 Count Distinct Substrings in $O(N^2)$
**Description:** Computes the total number of unique substrings in $O(N^2)$ using KMP.
*Note: Standalone helper.*

```cpp
long long count_distinct_substrings(string s) {
    long long distinct_count = 0;
    string rev_pref = "";
    for (int i = 0; i < s.size(); i++) {
        rev_pref = s[i] + rev_pref; // Add to front to reverse
        
        int m = rev_pref.size();
        vector<int> pi(m, 0);
        int max_pi = 0;
        for (int j = 1; j < m; j++) {
            int k = pi[j - 1];
            while (k > 0 && rev_pref[k] != rev_pref[j]) k = pi[k - 1];
            pi[j] = k + (rev_pref[k] == rev_pref[j]);
            max_pi = max(max_pi, pi[j]);
        }
        distinct_count += (m - max_pi);
    }
    return distinct_count;
}
```

## 1.14 Count Non-Overlapping Occurrences (Greedy Match)
**Description:** Finds the maximum number of times the pattern can appear in a text such that no two occurrences overlap.

```cpp
int count_non_overlapping(const string &text) {
    int n = text.size(), m = pattern.size();
    int count = 0;
    for (int i = 0, k = 0; i < n; i++) {
        k = failure(k, text[i]);
        if (k == m) {
            count++;
            k = 0; // Reset state greedily
        }
    }
    return count;
}
```

## 1.15 String Rotation Equivalence
**Description:** Checks if string $A$ is a cyclic shift (rotation) of string $B$.
*Note: Standalone helper.*

```cpp
bool is_rotation(const string& a, const string& b) {
    if (a.size() != b.size()) return false;
    string text = a + a;
    KMP kmp(b);
    vector<int> matches = kmp.match(text);
    return !matches.empty();
}
```

## 1.16 Periodic Prefix Checking
**Description:** For every prefix of the pattern, determines if it is perfectly periodic and returns the length of its shortest period.

```cpp
vector<int> get_periodic_prefixes() {
    int n = pattern.size();
    vector<int> periods(n);
    for (int i = 0; i < n; i++) {
        int len = i + 1;
        int k = len - lp[i];
        if (len % k == 0) {
            periods[i] = k;
        } else {
            periods[i] = len;
        }
    }
    return periods;
}
```
