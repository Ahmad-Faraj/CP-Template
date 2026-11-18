// DSU Range Parallel: union a+i with b+i for all i in [0, len), one DSU per power-of-two block.
// Use when: "s[a..a+len) equals s[b..b+len)", palindrome / equal-substring constraints, "add edges
// (a+i, b+i) for i < k", then count classes (answer = 26^components) or merge sums online.
// Usage: DSU_Range_Parallel d(n); d.unite(a, b, len); d.find(x); d.components;
//        d.unite(a, b, len, [&](int keep, int gone) { val[keep] += val[gone]; }); // real merges
// Handles: len = 0, overlapping ranges, a == b, repeats; online (find is valid after any unite).
// Time: all unites together O((n log n + q) * alpha); f runs once per level-0 merge (<= n - 1)
// Indexing: 0-based, needs 0 <= a, b and a + len, b + len <= n
// Note: level k joins block [a, a+2^k) with [b, b+2^k); a level-k merge that is new is pushed to
// both halves at level k-1, so every level does at most n merges. f(keep, gone): keep = new root.
// Source: Library Checker range_parallel_unionfind reference solution (maspy), adapted.

#include <bits/stdc++.h>
using namespace std;

struct DSU_Range_Parallel {
    struct UF {
        vector<int> p; // parent, or -size for a root
        UF(int n = 0) : p(n, -1) {}
        int find(int x) {
            while (p[x] >= 0) {
                if (p[p[x]] < 0) return p[x];
                x = p[x] = p[p[x]];
            }
            return x;
        }
        bool unite(int a, int b) {
            a = find(a), b = find(b);
            if (a == b) return false;
            if (p[a] > p[b]) swap(a, b);
            p[a] += p[b], p[b] = a;
            return true;
        }
    };

    int n, components;
    vector<UF> uf; // uf[k] joins i, j iff block [i, i+2^k) equals block [j, j+2^k)

    DSU_Range_Parallel(int n) : n(n), components(n) {
        int lg = 1;
        while ((1 << lg) < n) lg++;
        for (int k = 0; k < lg; k++) uf.emplace_back(n >= (1 << k) ? n - (1 << k) + 1 : 0);
    }

    int find(int x) { return uf[0].find(x); }
    bool same(int x, int y) { return find(x) == find(y); }
    int size(int x) { return -uf[0].p[find(x)]; }

    template <class F> void unite(int a, int b, int len, F f) {
        if (len <= 0) return;
        if (len == 1) return go(0, a, b, f);
        int k = 31 - __builtin_clz(len - 1); // 2^k < len <= 2^(k+1): two blocks cover it
        go(k, a, b, f);
        go(k, a + len - (1 << k), b + len - (1 << k), f);
    }
    void unite(int a, int b, int len) {
        unite(a, b, len, [](int, int) {});
    }

    template <class F> void go(int k, int a, int b, F &f) {
        if (k == 0) {
            int x = uf[0].find(a), y = uf[0].find(b);
            if (x == y) return;
            uf[0].unite(x, y), components--;
            int keep = uf[0].find(x);
            f(keep, x ^ y ^ keep);
            return;
        }
        if (!uf[k].unite(a, b)) return; // these blocks were already equal
        go(k - 1, a, b, f);
        go(k - 1, a + (1 << (k - 1)), b + (1 << (k - 1)), f);
    }
};
