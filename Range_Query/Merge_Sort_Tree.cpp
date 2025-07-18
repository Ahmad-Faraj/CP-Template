#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define sz(st) int(st.size())
#define all(st) st.begin(), st.end()
// Merge Sort Tree using SQRT
template <typename T> class Merge_Sort_Tree {
    // 1-Based
    int n;
    static const int MAX_N = 2e5 + 5; // TODO
    static const int len = 450;       // TODO
    vector<T> a;
    vector<vector<T>> b;
    int blk_idx(int idx) { return idx / len; }
    int get(int idx, T val, bool is_block = 0) // TODO
    {
        if (is_block) {
            idx = blk_idx(idx);
            return sz(b[idx]) - (lower_bound(all(b[idx]), val) - b[idx].begin());
        }
        return a[idx] >= val;
    }

    Merge_Sort_Tree(int N, const vector<T> &vec) {
        n = N, a = vec;
        b = vector<vector<T>>(len + 5);
        for (int i = 1; i <= n; i++) b[blk_idx(i)].push_back(a[i]);
        for (int i = 0; i <= len; i++) sort(all(b[i]));
    }

    void update(int idx, T val) {
        int k = blk_idx(idx);
        int j = lower_bound(all(b[k]), a[idx]) - b[k].begin();
        b[k][j] = a[idx] = val;
        sort(all(b[k]));
    }

    int query(int L, int R, T x) {
        int res = 0;
        while (L <= R) {
            if (L % len == 0 && L + len - 1 <= R) {
                res += get(L, x, 1);
                L += len;
            } else {
                res += get(L++, x);
            }
        }
        return res;
    }
};