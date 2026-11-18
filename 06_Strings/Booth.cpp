// Booth: the lexicographically least rotation of a string, in O(n).
// Use when: "smallest cyclic shift", canonical form of a necklace / circular string.
// Usage: string r = least_rotation(s);
// Handles: empty string, repeated characters, fully periodic strings.
// Time: O(n)
// Indexing: none (returns the rotation; k at the end is its 0-based start in s)
// Note: compares plain char: for bytes > 127 the order differs from std::string's (unsigned).

#include <bits/stdc++.h>
using namespace std;

string least_rotation(string s) {
    s += s;
    vector<int> f(s.size(), -1);
    int k = 0;
    for (int j = 1; j < (int)s.size(); j++) {
        char sj = s[j];
        int i = f[j - k - 1];
        while (i != -1 && sj != s[k + i + 1]) {
            if (sj < s[k + i + 1]) k = j - i - 1;
            i = f[i];
        }
        if (sj != s[k + i + 1]) {
            if (sj < s[k]) k = j;
            f[j - k] = -1;
        } else
            f[j - k] = i + 1;
    }
    return s.substr(k, s.size() / 2);
}
