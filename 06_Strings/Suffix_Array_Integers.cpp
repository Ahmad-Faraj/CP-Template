// Suffix Array (integers): sorts all suffixes of an integer sequence and builds the LCP array.
// Use when: the "string" is a sequence of numbers (compressed values, ids), or the alphabet is huge.
// Usage: SuffixArray sa(v);  sa.p[1..n-1] = sorted suffix starts, sa.c[i] = rank of suffix i,
//        sa.lcp[r] = LCP(suffix p[r-1], suffix p[r])
// Time: build O(n log n) (about 0.1 s at n = 2e5, 1 s at n = 1e6) | lcp O(n)
// Indexing: 0-based; a 0 sentinel is appended, so n = |v| + 1, p[0] = n - 1 and lcp[0] = 0
// Note: every value must be >= 1 (0 is the sentinel and must be the unique smallest). Compress first.
//       Same struct name as Suffix_Array.cpp: paste only one of the two.

#include <bits/stdc++.h>
using namespace std;

struct SuffixArray {
    vector<int> s;
    int n;
    vector<int> p, c, lcp;

    SuffixArray() = default;

    explicit SuffixArray(const vector<int> &v) : s(v), n((int)v.size() + 1) {
        s.push_back(0);
        p = c = lcp = vector<int>(n);
        build();
        build_lcp();
    }

    void count_sort() {
        vector<int> cnt(n, 0), pos(n, 0), p_new(n);
        for (auto &x : c) cnt[x]++;
        for (int i = 1; i < n; i++) pos[i] = pos[i - 1] + cnt[i - 1];
        for (auto &x : p) p_new[pos[c[x]]++] = x;
        p = p_new;
    }

    void build() {
        vector<pair<int, int>> a(n);
        for (int i = 0; i < n; i++) a[i] = {s[i], i};
        sort(a.begin(), a.end());
        for (int i = 0; i < n; i++) p[i] = a[i].second;
        c[p[0]] = 0;
        for (int i = 1; i < n; i++) c[p[i]] = c[p[i - 1]] + (a[i].first != a[i - 1].first);
        for (int k = 0; (1 << k) < n; k++) {
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
        }
    }

    void build_lcp() {
        int k = 0;
        for (int i = 0; i < n - 1; i++) {
            int pi = c[i], j = p[pi - 1];
            while (s[i + k] == s[j + k]) k++;
            lcp[pi] = k;
            if (k > 0) k--;
        }
    }
};
