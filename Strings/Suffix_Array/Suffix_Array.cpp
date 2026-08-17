#include "../../core.h"

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
            return s.compare(i, pat.size(), pat) < 0;
        }) - p.begin();
    }

    inline int upper(const string &pattern) {
        return upper_bound(p.begin(), p.end(), pattern, [&](const string &pat, int i) {
            return s.compare(i, pat.size(), pat) > 0;
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
