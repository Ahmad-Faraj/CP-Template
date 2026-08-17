// Suffix Array: every suffix sorted, with the LCP array and O(1) LCP between any two suffixes.
// Use when: "find / count a pattern", distinct substrings, longest repeated substring, longest common substring.
// Handles: pattern search and count, distinct substring count, longest repeated substring, LCP of any two suffixes.
// Time: build O(n log n) | find / count O(m log n) | longest_common_prefix O(1)
// Indexing: 0-based; a '$' sentinel is appended internally, so n = |s| + 1 and p[0] is always that sentinel
// Note: the stored s keeps the trailing '$', and every input character must sort strictly above '$' (0x24).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define sz(x) (int)(x).size()

struct SuffixArray {
    string s;
    int n, LOG;
    vector<int> p, c, lcp;   // p = suffixes in sorted order, c = rank of each suffix, lcp[i] = LCP(p[i-1], p[i])
    vector<vector<int>> mn;  // sparse table over lcp, for O(1) range minimum

    SuffixArray() = default;

    explicit SuffixArray(const string &str) : s(str + "$"), n((int)str.size() + 1) {
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
        for (int i = 0; i < n; i++) a[i] = {s[i], i};
        sort(a.begin(), a.end());
        for (int i = 0; i < n; i++) p[i] = a[i].second;
        c[p[0]] = 0;
        for (int i = 1; i < n; i++) c[p[i]] = c[p[i - 1]] + (a[i].first != a[i - 1].first);

        int k = 0;
        while (((1 << k) < n) and p.back() != n - 1) {
            for (int i = 0; i < n; i++) p[i] = (p[i] - (1 << k) + n) % n;
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

    void build_lcp() {
        int k = 0;
        for (int i = 0; i < n - 1; i++) {
            int pi = c[i], j = p[pi - 1];
            while (s[i + k] == s[j + k]) k++;
            lcp[pi] = k;
            k = max(k - 1, 0);
        }
    }

    void build_sparse_table() {
        LOG = 32 - __builtin_clz(n);
        mn = vector<vector<int>>(n, vector<int>(LOG));
        for (int i = 1; i < n; i++) mn[i][0] = lcp[i];
        for (int j = 1; j < LOG; j++)
            for (int i = 0; i + (1 << j) <= n; i++) mn[i][j] = min(mn[i][j - 1], mn[i + (1 << (j - 1))][j - 1]);
    }

    int compare(int i, const string &pattern) { return s.compare(i, sz(pattern), pattern); }

    bool find(const string &pattern) { // does pattern occur in the string
        int l = 0, r = n - 1;
        while (l <= r) {
            int mid = (l + r) >> 1, res = compare(p[mid], pattern);
            if (res == 0) return true;
            if (res < 0)
                l = mid + 1;
            else
                r = mid - 1;
        }
        return false;
    }

    int lower(const string &pattern) { // first index in p whose suffix starts with pattern
        return (int)(lower_bound(p.begin(), p.end(), pattern,
                                 [&](int i, const string &pat) { return s.compare(i, sz(pat), pat) < 0; }) -
                     p.begin());
    }

    int upper(const string &pattern) { // last index in p whose suffix starts with pattern
        return (int)(upper_bound(p.begin(), p.end(), pattern,
                                 [&](const string &pat, int i) { return s.compare(i, sz(pat), pat) > 0; }) -
                     p.begin()) -
               1;
    }

    int count(const string &pattern) { return upper(pattern) - lower(pattern) + 1; } // occurrences of pattern

    ll distinct_substrings() { // number of distinct non-empty substrings
        ll ans = 0;
        for (int i = 1; i < n; i++) ans += n - (1 + p[i] + lcp[i]);
        return ans;
    }

    string longest_repeated_substring() { // longest substring that occurs at least twice
        int idx = (int)(max_element(lcp.begin(), lcp.end()) - lcp.begin());
        return s.substr(p[idx], lcp[idx]);
    }

    int longest_common_prefix(int i, int j) { // LCP of the suffixes starting at i and j
        if (i == j) return n - i - 1;
        int l = min(c[i], c[j]), r = max(c[i], c[j]), len = r - l;
        int k = 31 - __builtin_clz(len);
        return min(mn[l + 1][k], mn[r - (1 << k) + 1][k]);
    }

    static string longest_common_substring(const string &a, const string &b) { // longest substring shared by a and b
        string st = a + "%" + b; // '%' must sort above the '$' sentinel and below the alphabet
        SuffixArray sa(st);
        int m = sz(a), ans = 0, idx = 0;
        for (int i = 1; i < sa.n; i++)
            if ((sa.p[i] < m) != (sa.p[i - 1] < m) && sa.p[i] != m && sa.p[i - 1] != m)
                if (ans < sa.lcp[i]) ans = sa.lcp[i], idx = sa.p[i];
        return st.substr(idx, ans);
    }
};

// Standard problem: print the suffix array, the number of distinct substrings, and the count of each query pattern
void solve() {
    string s;
    int q;
    cin >> s >> q;
    SuffixArray sa(s);
    for (int i = 1; i < sa.n; i++) cout << sa.p[i] << ' ';
    cout << '\n' << sa.distinct_substrings() << '\n';
    while (q--) {
        string pat;
        cin >> pat;
        cout << sa.count(pat) << '\n';
    }
}
