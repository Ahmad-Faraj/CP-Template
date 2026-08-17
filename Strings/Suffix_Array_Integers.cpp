// Suffix Array (integers): the suffixes of an int array sorted, together with the LCP array.
// Use when: the alphabet is integers rather than characters - compressed values, or a sequence of large numbers.
// Handles: any strictly positive values, duplicates, arbitrary magnitude. Use Suffix_Array.cpp for plain strings.
// Time: build O(n log n) | build_lcp O(n)
// Indexing: 0-based; a 0 sentinel is appended, so n = |a| + 1 and p[0] is always that sentinel
// Note: every value must be strictly positive - compress first if the array holds 0 or negative numbers.

#include <bits/stdc++.h>
using namespace std;
#define sz(x) (int)(x).size()

struct SuffixArray {
    vector<int> s;
    int n;
    vector<int> p, c, lcp; // p = suffixes in sorted order, c = rank of each suffix, lcp[i] = LCP(p[i-1], p[i])

    SuffixArray() = default;

    explicit SuffixArray(vector<int> _s) : s(_s), n(sz(_s) + 1) {
        s.push_back(0); // sentinel, smaller than every real value
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

        int k = 0;
        while (((1 << k) < n) && p.back() != n - 1) {
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
};

// Standard problem: print the suffix array of an integer sequence, then its LCP array
void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &x : a) cin >> x;
    SuffixArray sa(a);
    for (int i = 1; i < sa.n; i++) cout << sa.p[i] << ' ';
    cout << '\n';
    for (int i = 2; i < sa.n; i++) cout << sa.lcp[i] << ' ';
    cout << '\n';
}
