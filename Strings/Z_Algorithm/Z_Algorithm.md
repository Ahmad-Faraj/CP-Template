# 1. Z-Algorithm

## 1.0 Overview
The Z-algorithm computes an array `Z` of length $N$ (the same length as the string $S$) in $O(N)$ time. 
Each element $Z[i]$ represents the length of the longest substring starting from $S[i]$ which is also a prefix of $S$.

- **Definition:** $Z[i]$ is the length of the longest common prefix between $S$ and the suffix of $S$ starting at $i$.
- **Time Complexity:** $O(N)$
- **Space Complexity:** $O(N)$

---

## 1.0.1 Core Z-Algorithm Implementation ($O(N)$)
**Description:** The base struct responsible for building the Z-array. Copy-paste this directly into your template.

```cpp
struct ZAlgorithm {
    string s;
    int n;
    vector<int> z;

    explicit ZAlgorithm(const string& str) : s(str), n(str.size()) {
        z.assign(n, 0);
        for (int i = 1, l = 0, r = 0; i < n; i++) {
            if (i <= r) {
                z[i] = min(r - i + 1, z[i - l]);
            }
            while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
                z[i]++;
            }
            if (i + z[i] - 1 > r) {
                l = i;
                r = i + z[i] - 1;
            }
        }
    }

    int operator[](int i) const {
        return z[i];
    }
};
```

---

## 1.1 Basic Pattern Matching
**Description:** Finds all starting indices of `pattern` inside string `T`.

```cpp
vector<int> match(const string& T, const string& pattern) {
    vector<int> ans;
    string combined = pattern + "#" + T;
    ZAlgorithm zAlg(combined);
    
    int p_len = pattern.size();
    for (int i = p_len + 1; i < combined.size(); i++) {
        if (zAlg[i] >= p_len) {
            ans.push_back(i - p_len - 1);
        }
    }
    return ans;
}
```

---

## 1.2 Exact Periodicity (String Compression)
**Description:** A string can be compressed into repeating periods if and only if it satisfies a specific mathematical bound using the Z-array.

```cpp
// Returns the length of the shortest repeating period.
// If it returns S.size(), the string cannot be compressed.
int shortest_period(const string& s) {
    ZAlgorithm zAlg(s);
    int n = s.size();
    for (int i = 1; i < n; i++) {
        if (n % i == 0 && zAlg[i] == n - i) {
            return i;
        }
    }
    return n;
}
```

---

## 1.3 Minimum String Concatenation
**Description:** Finds the shortest string that contains $A$ as a prefix and $B$ as a suffix (i.e., finds the maximum suffix of $A$ that matches a prefix of $B$).

```cpp
string minString(const string& a, const string& b) {
    string combined = a + b;
    ZAlgorithm zAlg(combined);
    
    int maxMatch = 0;
    for (int i = 1; i < combined.size(); i++) {
        if (i >= a.size()) {
            if (zAlg[i] + i == combined.size()) {
                maxMatch = max(maxMatch, zAlg[i]);
            }
            if (zAlg[i] == a.size()) {
                return b;
            }
        }
    }
    return b + a.substr(maxMatch);
}
```

---

## 1.4 Prefix Occurrence Frequencies ($O(N)$)
**Description:** A legendary DP trick over the Z-array to count exactly how many times *every single prefix* of $S$ appears throughout the entire string!

```cpp
// Returns an array where ans[L] is the frequency of the prefix S[0...L-1]
vector<long long> prefix_frequencies(const string& s) {
    ZAlgorithm zAlg(s);
    int n = s.size();
    vector<long long> count(n + 2, 0);
    
    for (int i = 1; i < n; i++) {
        if (zAlg[i] > 0) {
            count[zAlg[i]]++;
        }
    }
    
    // Reverse prefix sum DP
    for (int i = n; i >= 1; i--) {
        count[i] += count[i + 1];
    }
    
    // Add 1 to each prefix because the prefix itself matches itself at index 0
    for (int i = 1; i <= n; i++) {
        count[i]++;
    }
    return count;
}
```

---

## 1.5 Convert Z-Array to KMP Pi-Array ($O(N)$)
**Description:** The Z-array can dynamically construct the KMP Prefix Function (Pi-array) in $O(N)$ time.

```cpp
vector<int> z_to_pi(const vector<int>& z) {
    int n = z.size();
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++) {
        if (z[i] > 0) {
            // For the substring of length z[i] starting at i, the longest 
            // proper prefix that is also a suffix is updated!
            for (int j = z[i] - 1; j >= 0 && pi[i + j] == 0; j--) {
                pi[i + j] = j + 1;
            }
        }
    }
    return pi;
}
```

---

## 1.6 Lexicographical Comparison against Prefix ($O(1)$)
**Description:** Determines if the suffix $S[i\dots N]$ is lexicographically smaller than the entire string $S$ instantly.

```cpp
// Returns -1 if S[i...] < S, 0 if S[i...] == S, 1 if S[i...] > S
int compare_suffix_to_prefix(const string& s, const ZAlgorithm& zAlg, int i) {
    int lcp = zAlg[i];
    if (i + lcp == s.size()) {
        return -1; // Suffix is fully a prefix but shorter
    }
    return s[i + lcp] < s[lcp] ? -1 : 1;
}
```

---

## 1.7 Longest Palindromic Prefix ($O(N)$)
**Description:** Instead of Manacher's algorithm, you can simply build the Z-array on $S + \text{'\#'} + S^R$.

```cpp
int longest_palindromic_prefix(const string& s) {
    string rev_s = s;
    reverse(rev_s.begin(), rev_s.end());
    string combined = s + "#" + rev_s;
    ZAlgorithm zAlg(combined);
    
    int n = s.size();
    int best = 0;
    // Iterate over the reversed string part in the combined string
    for (int i = n + 1; i < combined.size(); i++) {
        int length = zAlg[i];
        // If the match reaches the very end of the combined string, it's a prefix palindrome!
        if (i + length == combined.size()) {
            best = max(best, length);
        }
    }
    return best;
}
```

---

## 1.8 String Matching with EXACTLY 1 Mismatch ($O(N)$ via Dual Z-Arrays)
**Description:** Finds all occurrences of Pattern $P$ in Text $T$ with at most ONE mismatch. Instead of a messy $O(N \cdot K)$ LCP jump, we construct a Forward Z-array and a Backward Z-array. For any starting candidate, we jump past the forward match and check if the backward suffix match safely covers the remainder of the pattern!

```cpp
vector<int> match_with_1_mismatch(string T, string P) {
    string forward_str = P + "#" + T;
    ZAlgorithm zForward(forward_str);
    
    string rev_T = T, rev_P = P;
    reverse(rev_T.begin(), rev_T.end());
    reverse(rev_P.begin(), rev_P.end());
    string backward_str = rev_P + "#" + rev_T;
    ZAlgorithm zBackward(backward_str);
    
    vector<int> valid_starts;
    int p_len = P.size(), t_len = T.size();
    
    for (int i = 0; i <= t_len - p_len; i++) {
        int forward_match = zForward[p_len + 1 + i];
        
        int back_idx_in_revT = t_len - (i + p_len);
        int backward_match = zBackward[p_len + 1 + back_idx_in_revT];
        
        // The total matched characters from front and back
        if (forward_match + backward_match >= p_len - 1) {
            valid_starts.push_back(i);
        }
    }
    return valid_starts;
}
```

---

## 1.9 Generic Array/Vector Z-Algorithm
**Description:** Just like KMP and Suffix Arrays, the Z-Algorithm logic translates perfectly to integer vectors. Use this when performing sequence matching on geometry points, tree paths, or large coordinate-compressed arrays.

```cpp
vector<int> z_function_vector(const vector<int>& s) {
    int n = s.size();
    vector<int> z(n, 0);
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}
```

---

## 1.10 All Fractional Periods (Borders)
**Description:** Unlike exact string compression (which requires $N \% P == 0$), a string can have fractional periods (e.g., `ababa` has a fractional period of 2: `ab` repeated 2.5 times). The Z-array instantly finds all lengths $P$ by which the string can be shifted to perfectly overlap with itself!

```cpp
// Returns all lengths P where shifting the string by P perfectly overlaps.
vector<int> all_periods(const string& s) {
    ZAlgorithm zAlg(s);
    int n = s.size();
    vector<int> periods;
    for (int i = 1; i < n; i++) {
        // If the match starting at i reaches the very end of the string
        if (i + zAlg[i] == n) {
            periods.push_back(i); // i is a valid fractional period
        }
    }
    periods.push_back(n); // The string itself is trivially a period of length N
    return periods;
}
```

---

## 1.11 Cyclic Equivalence (Rotation Match)
**Description:** Determines if String $B$ is a cyclic shift / rotation of String $A$ in purely $O(|A|)$ time.

```cpp
bool is_cyclic_shift(const string& a, const string& b) {
    if (a.size() != b.size()) return false;
    // Concatenate a + delimiter + b + b
    string combined = a + "#" + b + b;
    ZAlgorithm zAlg(combined);
    
    int n = a.size();
    for (int i = n + 1; i < combined.size(); i++) {
        if (zAlg[i] == n) {
            return true; // We found the exact start of 'a' inside 'b+b'
        }
    }
    return false;
}
```

---

## 1.12 Longest Prefix of $A$ that is a Suffix of $B$
**Description:** This solves the maximum valid overlap between two strings (useful for building Shortest Superstrings or gluing words together). It strictly forces the matching prefix of $A$ to touch the absolute end of $B$.

```cpp
int max_prefix_suffix_overlap(const string& a, const string& b) {
    string combined = a + "#" + b;
    ZAlgorithm zAlg(combined);
    
    int max_overlap = 0;
    int a_len = a.size();
    for (int i = a_len + 1; i < combined.size(); i++) {
        int length = zAlg[i];
        // The match must touch the very end of the combined string!
        if (i + length == combined.size()) {
            max_overlap = max(max_overlap, length);
        }
    }
    return max_overlap;
}
```

---

## 1.13 Pattern Matching with Wildcard Blocks
**Description:** If your pattern contains `*` wildcards (which match any arbitrary sequence of characters, including empty sequences), you can split the pattern into solid blocks and match them greedily from left to right using the Z-Algorithm!

```cpp
// Matches a pattern with '*' wildcards (e.g., "AB*C*D") inside a text.
bool match_with_wildcards(const string& text, const string& pattern) {
    vector<string> blocks;
    string current = "";
    for(char c : pattern) {
        if(c == '*') {
            if(!current.empty()) blocks.push_back(current);
            current = "";
        } else {
            current += c;
        }
    }
    if(!current.empty()) blocks.push_back(current);
    if(blocks.empty()) return true; // Pattern is entirely wildcards
    
    int t_idx = 0;
    for(int i = 0; i < blocks.size(); i++) {
        string block = blocks[i];
        string combined = block + "#" + text.substr(t_idx);
        ZAlgorithm zAlg(combined);
        
        bool found = false;
        int b_len = block.size();
        for(int j = b_len + 1; j < combined.size(); j++) {
            if(zAlg[j] == b_len) {
                found = true;
                // If it's the FIRST block and pattern didn't start with '*', 
                // it MUST match exactly at index 0.
                if(i == 0 && pattern[0] != '*' && j != b_len + 1) return false;
                
                t_idx += (j - (b_len + 1)) + b_len;
                break;
            }
        }
        if(!found) return false;
    }
    
    // If the pattern doesn't end with '*', the last block MUST match exactly at the end.
    if(pattern.back() != '*') {
        string last_block = blocks.back();
        if(text.size() < last_block.size()) return false;
        if(text.substr(text.size() - last_block.size()) != last_block) return false;
    }
    return true;
}
```
