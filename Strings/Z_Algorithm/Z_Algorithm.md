# 1. Z-Algorithm

## 1.0 Mathematical Properties & Rules
The Z-algorithm computes an array `Z` of length $N$ (the same length as the string $S$) in $O(N)$ time. 
Each element $Z[i]$ represents the length of the longest substring starting from $S[i]$ which is also a prefix of $S$.

- **Definition:** $Z[i]$ is the length of the longest common prefix between $S$ and the suffix of $S$ starting at $i$.
- **Time Complexity:** $O(N)$ where $N$ is the length of the string.
- **Space Complexity:** $O(N)$ to store the array.

**Common Applications:**
- **String Matching:** To find a pattern $P$ in a text $T$, we can build a new string $S = P + \# + T$ (where $\#$ is a character not present in $P$ or $T$). If $Z[i]$ for some $i$ is equal to $|P|$, it means $P$ occurs at that position in $T$.
- **Shortest Period:** Finding the shortest repeating period of a string.
- **String Compression:** Checking if a string can be constructed by repeating a substring.

---

This file contains modular functions and variations using the base `ZAlgorithm` structure defined in `Z_Algorithm.cpp`.

## 1.1 Basic Pattern Matching
**Description:** Finds all occurrences of `pattern` inside string `s`.

```cpp
#include "../../core.h"
#include "Z_Algorithm.cpp"

vector<int> match(const string& s, const string& pattern) {
    vector<int> ans;
    // Combine pattern and string with a separator
    string t = pattern + "#" + s;
    ZAlgorithm z(t);
    
    // Check where the Z-value matches the pattern length
    for (int i = sz(pattern) + 1; i < sz(t); i++) {
        if (z[i] >= sz(pattern)) {
            ans.push_back(i - sz(pattern) - 1);
        }
    }
    return ans;
}
```

## 1.2 Minimum String Concatenation
**Description:** Finds the shortest string that contains $A$ as a prefix and $B$ as a suffix, basically finding the maximum suffix of $A$ that matches a prefix of $B$.

```cpp
#include "../../core.h"
#include "Z_Algorithm.cpp"

string minString(const string& a, const string& b) {
    string s = a + b;
    ZAlgorithm zAlg(s);
    const vector<int>& z = zAlg.z;
    
    int maxMatch = 0;
    for (int i = 1; i < sz(s); i++) {
        if (i >= sz(a)) {
            // Match reaches the end of the combined string
            if (z[i] + i == sz(s)) {
                maxMatch = max(maxMatch, z[i]);
            }
            // A contains B
            if (z[i] == sz(a)) {
                return b;
            }
        }
    }
    return b + a.substr(maxMatch);
}
```
