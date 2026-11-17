// Suffix Array: all suffixes sorted, with the LCP array and O(1) LCP of any two suffixes.
// Use when: "find / count a pattern", distinct substrings, longest repeated substring, longest common
//           substring of two strings, k-th substring, comparing two substrings fast.
// Usage: SuffixArray sa(s);  sa.p[r] = start of the r-th smallest suffix, sa.c[i] = rank of suffix i,
//        sa.lcp[r] = LCP(p[r-1], p[r]);  sa.count(pat), sa.longest_common_prefix(i, j), ...
// Handles: find, count, lower/upper, distinct_substrings, longest_repeated_substring, compare,
//          longest_common_substring(a, b), kth_substring, min_common_substring (all listed below).
// Time: build O(n log n) | find / count O(m log n) | longest_common_prefix, compare(i, j, l) O(1)
// Indexing: 0-based; n = |s| + 1: the end of the string (the '\0' that s[|s|] reads) is the sentinel
//           suffix, so p[0] = |s| and lcp[0] = 0. The stored s has no sentinel character appended.
// Note: characters must be > '\0' as signed char (no '\0', no bytes >= 128).
//       Sparse table is n * 21 ints: about 85 MB at n = 1e6.
//       longest_common_substring / min_common_substring join a + '#' + b + '$': neither may contain
//       '#' or '$'. kth_substring is O(n + k): counts substrings WITH repetition, k in [1, n(n+1)/2].

#include <bits/stdc++.h>
using namespace std;

struct SuffixArray {
    string s;
    int n, LOG;
    vector<int> p, c, lcp; // p = sorted suffix starts, c = rank of each suffix, lcp[i] = LCP(p[i-1], p[i])
    vector<vector<int>> mn; // sparse table over lcp

    SuffixArray() = default;

    explicit SuffixArray(const string &str) : s(str), n((int)str.size() + 1) {
        p = c = lcp = vector<int>(n);
        build();
        build_lcp();
        build_sparse_table();
    }

    void count_sort() {
        vector<int> cnt(n, 0), pos(n, 0), p_new(n);
        for (auto x : c) cnt[x]++;
        for (int i = 1; i < n; i++) pos[i] = pos[i - 1] + cnt[i - 1];
        for (auto x : p) p_new[pos[c[x]]++] = x;
        p = p_new;
    }

    void build() {
        vector<pair<char, int>> a(n);
        for (int i = 0; i < n; i++) a[i] = {s[i], i}; // s[n - 1] is the '\0' terminator
        sort(a.begin(), a.end());
        for (int i = 0; i < n; i++) p[i] = a[i].second;
        c[p[0]] = 0;
        for (int i = 1; i < n; i++) c[p[i]] = c[p[i - 1]] + (a[i].first != a[i - 1].first);
        for (int k = 0; (1 << k) < n; k++) {
            for (int i = 0; i < n; i++) p[i] = (p[i] - (1 << k) + n) % n; // cyclic shift
            count_sort();
            vector<int> c_new(n);
            c_new[p[0]] = 0;
            for (int i = 1; i < n; i++) {
                pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << k)) % n]};
                pair<int, int> now = {c[p[i]], c[(p[i] + (1 << k)) % n]};
                c_new[p[i]] = c_new[p[i - 1]] + (prev != now);
            }
            c = c_new;
        }
    }

    void build_lcp() {
        int k = 0;
        for (int i = 0; i < n - 1; i++) {
            int pi = c[i], j = p[pi - 1]; // previous suffix in sorted order
            while (s[i + k] == s[j + k]) k++;
            lcp[pi] = k;
            if (k > 0) k--;
        }
    }

    void build_sparse_table() {
        LOG = 32 - __builtin_clz(n);
        mn = vector<vector<int>>(n, vector<int>(LOG));
        for (int i = 1; i < n; i++) mn[i][0] = lcp[i];
        for (int j = 1; j < LOG; j++)
            for (int i = 0; i + (1 << j) <= n; i++)
                mn[i][j] = min(mn[i][j - 1], mn[i + (1 << (j - 1))][j - 1]);
    }

    // LCP of the suffixes starting at positions i and j of s
    int longest_common_prefix(int i, int j) {
        if (i == j) return n - i - 1;
        int l = min(c[i], c[j]), r = max(c[i], c[j]), len = r - l;
        int k = 31 - __builtin_clz(len);
        return min(mn[l + 1][k], mn[r - (1 << k) + 1][k]);
    }

    // compare s.substr(i, l) with s.substr(j, l): -1, 0 or 1 (a shorter cut-off piece is smaller)
    int compare(int i, int j, int l) {
        int L = longest_common_prefix(i, j);
        if (i == j || L >= l) return 0;
        return s[i + L] < s[j + L] ? -1 : 1;
    }

    int compare(int i, const string &pattern) { // s.substr(i, |pattern|) vs pattern, as string::compare
        return s.compare(i, pattern.size(), pattern);
    }

    bool find(const string &pattern) { // does pattern occur in s
        int l = 0, r = n - 1;
        while (l <= r) {
            int mid = (l + r) >> 1, res = compare(p[mid], pattern);
            if (res == 0) return true;
            if (res < 0) l = mid + 1;
            else r = mid - 1;
        }
        return false;
    }

    int lower(const string &pattern) { // first rank whose suffix starts with pattern
        return lower_bound(p.begin(), p.end(), pattern, [&](int i, const string &pat) {
                   return s.compare(i, pat.size(), pat) < 0;
               }) - p.begin();
    }

    int upper(const string &pattern) { // last rank whose suffix starts with pattern
        return upper_bound(p.begin(), p.end(), pattern, [&](const string &pat, int i) {
                   return s.compare(i, pat.size(), pat) > 0;
               }) - p.begin() - 1;
    }

    int count(const string &pattern) { return upper(pattern) - lower(pattern) + 1; } // occurrences

    long long distinct_substrings() { // distinct non-empty substrings
        long long ans = 0;
        for (int i = 1; i < n; i++) ans += n - (1 + p[i] + lcp[i]);
        return ans;
    }

    string longest_repeated_substring() { // longest substring occurring at least twice ("" if none)
        int idx = max_element(lcp.begin(), lcp.end()) - lcp.begin();
        return s.substr(p[idx], lcp[idx]);
    }

    // longest common substring of a and b ("" if none)
    static string longest_common_substring(const string &a, const string &b) {
        string st = a + "#" + b + "$";
        SuffixArray sa(st);
        int m = a.size(), ans = 0, idx = 0;
        for (int i = 1; i < sa.n; i++)
            if ((sa.p[i] < m && sa.p[i - 1] > m) || (sa.p[i] > m && sa.p[i - 1] < m))
                if (ans < sa.lcp[i]) ans = sa.lcp[i], idx = sa.p[i];
        return st.substr(idx, ans);
    }

    // k-th smallest substring of str counting repeats (k is 1-based); call as kth_substring(k, sa.s, sa.p)
    static string kth_substring(long long k, const string &s, const vector<int> &p) {
        vector<int> pos = p, cur = p;
        int piv = 0, n = s.size();
        while (k) {
            char c = s[cur[piv]];
            int diff = cur[piv] - pos[piv];
            for (int i = piv; i < (int)pos.size() && k; i++) {
                if (cur[i] < n && c == s[cur[i]]) {
                    if (i == piv) cur[i]++, k--;
                    else if (cur[i] - pos[i] == diff) cur[i]++, k--;
                    else break;
                } else break;
            }
            if (!k) return s.substr(pos[piv], cur[piv] - pos[piv]);
            while (cur[piv] == n) piv++;
        }
        return "";
    }

    // shortest length of a substring occurring exactly once in a and exactly once in b, -1 if none
    static int min_common_substring(const string &a, const string &b) {
        string st = a + "#" + b + "$";
        SuffixArray sa(st);
        sa.lcp.push_back(0);
        int m = a.size(), ans = INT_MAX;
        for (int i = 1; i < (int)sa.lcp.size() - 1; i++) {
            if ((sa.p[i] < m && sa.p[i - 1] > m) || (sa.p[i] > m && sa.p[i - 1] < m)) {
                int mx = max(sa.lcp[i - 1], sa.lcp[i + 1]);
                if (sa.lcp[i] > mx) ans = min(ans, mx + 1);
            }
        }
        return ans == INT_MAX ? -1 : ans;
    }
};
