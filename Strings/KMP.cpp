// KMP: prefix function of a pattern, and every occurrence of that pattern inside a text.
// Use when: "find all occurrences of p in s", periodicity, borders, "shortest string with s as both prefix and suffix".
// Handles: all matches, all borders, per-prefix occurrence counts, non-overlapping borders. Any alphabet.
// Time: build O(m) | match O(n) | count_in O(n + m)
// Indexing: match() returns 1-based positions in the text; lp[] is 0-based over the pattern
// Note: build() and build_non_overlapping() both fill lp[] - call exactly one of them.

#include <bits/stdc++.h>
using namespace std;
#define sz(x) (int)(x).size()

struct KMP {
    string pattern;
    vector<int> lp; // lp[i] = length of the longest proper border of pattern[0..i]

    KMP(const string &str = "") : pattern(str) { build(); }

    void build() {
        int n = sz(pattern);
        lp.assign(n, 0);
        for (int i = 1; i < n; i++) lp[i] = failure(lp[i - 1], pattern[i]);
    }

    // for "ababab" the non-overlapping borders are "", "", "a", "ab", "a", "ab"
    void build_non_overlapping() {
        int n = sz(pattern);
        lp.assign(n, 0);
        for (int i = 1; i < n; i++) {
            int j = lp[i - 1];
            while (j > 0 && pattern[j] != pattern[i]) j = lp[j - 1];
            lp[i] = j + (pattern[j] == pattern[i]);
            if (lp[i] * 2 == i + 1) lp[i] = lp[lp[i] - 1];
        }
    }

    int failure(int idx, char nxt) { // next automaton state from idx on character nxt
        while (idx > 0 && pattern[idx] != nxt) idx = lp[idx - 1];
        return idx + (pattern[idx] == nxt);
    }

    vector<int> match(const string &str) { // 1-based start positions of every occurrence in str
        int n = sz(str), m = sz(pattern);
        vector<int> ret;
        for (int i = 0, k = 0; i < n; i++) {
            k = failure(k, str[i]);
            if (k == m) ret.push_back(i - m + 2);
        }
        return ret;
    }

    vector<int> count_in(const string &t) { // ans[i] = occurrences of pattern's prefix of length i+1 inside t
        string s = pattern + "#" + t;
        int n = sz(s);
        KMP kmp(s);
        vector<int> cnt(n);
        for (int i = sz(pattern) + 1; i < n; i++) cnt[kmp.lp[i]]++;
        for (int i = sz(pattern); i > 0; i--) cnt[kmp.lp[i - 1]] += cnt[i];
        vector<int> ans;
        for (int i = 1; i <= sz(pattern); i++) ans.push_back(cnt[i]);
        return ans;
    }

    vector<int> get_borders() { // every border length of the pattern, increasing, ending with the whole pattern
        vector<int> indexes = {sz(pattern)};
        for (int i = lp[sz(pattern) - 1]; i > 0; i = lp[i - 1]) indexes.push_back(i);
        sort(indexes.begin(), indexes.end());
        return indexes;
    }
};

// Standard problem: print every border length of s, then every 1-based occurrence of p in s
void solve() {
    string s, p;
    cin >> s >> p;
    KMP kmp(s);
    for (int x : kmp.get_borders()) cout << x << ' ';
    cout << '\n';
    KMP pat(p);
    for (int x : pat.match(s)) cout << x << ' ';
    cout << '\n';
}
