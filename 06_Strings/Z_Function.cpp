// Z Function: z[i] = length of the longest common prefix of s and the suffix of s starting at i.
// Use when: "all occurrences of p in s", LCP of a string with each of its suffixes, periods, borders.
// Usage: Zfunctions z; z.build(s); z[i] / z.z[i];  vector<int> pos = z.match(text, pattern);
// Handles: any alphabet; match() reports overlapping occurrences; minString(a, b) as below.
// Time: build O(n) | match O(|text| + |pattern|) | minString O(|a| + |b|)
// Indexing: 0-based; z[0] = 0; match() returns 0-based start positions in the text
// Note: match() builds z over pattern + '#' + text, so '#' must not occur in either string; pattern
//       must be non-empty. match() overwrites z. minString(a, b) = shortest string that starts with b
//       and contains a (b, or b + the part of a not covered by the longest suffix of b = prefix of a).

#include <bits/stdc++.h>
using namespace std;

struct Zfunctions {
    vector<int> z;

    void build(const string &s) {
        int n = s.size();
        z.assign(n, 0);
        for (int i = 1, l = 0, r = 0; i < n; i++) {
            if (i <= r) z[i] = min(r - i + 1, z[i - l]);
            while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
            if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
        }
    }

    int operator[](int i) { return z[i]; }

    vector<int> match(const string &s, const string &pattern) { // start positions of pattern in s
        vector<int> ans;
        int m = pattern.size();
        string t = pattern + "#" + s;
        build(t);
        for (int i = m + 1; i < (int)t.size(); i++)
            if (z[i] >= m) ans.push_back(i - m - 1);
        return ans;
    }

    string minString(const string &a, const string &b) { // shortest string = b + ..., containing a
        string s = a + b;
        int n = s.size(), na = a.size(), maxMatch = 0;
        vector<int> z(n);
        for (int i = 1, l = 0, r = 0; i < n; i++) {
            if (i <= r) z[i] = min(z[i - l], r - i + 1);
            while (i + z[i] < n && s[z[i]] == s[i + z[i]]) l = i, r = i + z[i], z[i]++;
            if (i >= na) {
                if (z[i] >= na) return b; // a occurs inside b
                if (z[i] + i == n) maxMatch = max(maxMatch, z[i]);
            }
        }
        return b + a.substr(maxMatch);
    }

    void print() {
        for (int x : z) cout << x << ' ';
        cout << '\n';
    }
};
