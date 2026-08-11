/*
    [1] Definition
    Square Root (SQRT) Decomposition with Lazy Propagation dividing an array into blocks of size O(sqrt(N)).
    Supports range addition updates and range sum queries in O(sqrt(N)) time.

    [2] Time & Space Complexity
    Build: O(N)
    Range Update: O(sqrt(N))
    Range / Prefix Query: O(sqrt(N))
    Space: O(N)

    [3] Notes
    Uses 0-based indexing for range queries and updates.
    Calculates block size as SQ = sqrt(N) + 1.
    Maintains lazy propagation tags to defer range addition updates across whole blocks.
*/

#include "../../core.h"

template <typename T>
struct SQRT {
    int n, SQ, BlocksNum;
    vector<T> arr, block, lazy;

    SQRT(int N) {
        n = N;
        SQ = sqrt(N) + 1;
        BlocksNum = (n + SQ - 1) / SQ;

        arr.assign(N, T(0));
        block.assign(BlocksNum, T(0));
        lazy.assign(BlocksNum, T(0));
    }

    SQRT(vector<T>& nums) {
        n = nums.size();
        SQ = sqrt(n) + 1;
        BlocksNum = (n + SQ - 1) / SQ;

        arr = nums;
        block.assign(BlocksNum, T(0));
        lazy.assign(BlocksNum, T(0));

        for (int i = 0; i < n; i++) block[i / SQ] += arr[i];
    }

    void init(int idx) {
        block[idx] = T(0);

        int l = idx * SQ;
        int r = min(n - 1, l + SQ - 1);

        for (int i = l; i <= r; i++) block[idx] += arr[i];
    }

    // Push lazy tag
    void propegate(int idx) {
        if (lazy[idx] == T(0)) return;

        int l = idx * SQ;
        int r = min(n - 1, l + SQ - 1);

        for (int i = l; i <= r; i++) arr[i] += lazy[idx];

        block[idx] += lazy[idx] * (r - l + 1);

        lazy[idx] = T(0);
    }

    // Range addition: a[i] += x for i in [l, r]
    void update(int l, int r, T x) {
        int st = l / SQ;
        int en = r / SQ;

        // Same block
        if (st == en) {
            propegate(st);

            for (int i = l; i <= r && i < n; i++) arr[i] += x;

            init(st);
            return;
        }

        propegate(st);
        propegate(en);

        for (int i = l; i < (st + 1) * SQ; i++) arr[i] += x;

        init(st);

        for (int i = st + 1; i < en; i++) lazy[i] += x;

        for (int i = en * SQ; i <= r && i < n; i++) arr[i] += x;

        init(en);
    }

    // Prefix sum [0, r]
    T query(int r) {
        if (r < 0) return T(0);

        T ans = T(0);

        int b = r / SQ;

        for (int i = 0; i < b; i++) {
            int l = i * SQ;
            int rr = min(n - 1, (i + 1) * SQ - 1);
            int len = rr - l + 1;

            ans += block[i] + lazy[i] * len;
        }

        for (int i = b * SQ; i <= r && i < n; i++) ans += arr[i] + lazy[b];

        return ans;
    }

    // Range sum [l, r]
    T query(int l, int r) {
        if (l > r) return T(0);

        int st = l / SQ;
        int en = r / SQ;

        T ans = T(0);

        if (st == en) {
            for (int i = l; i <= r; i++) ans += arr[i] + lazy[st];

            return ans;
        }

        for (int i = st + 1; i < en; i++) {
            int L = i * SQ;
            int R = min(n - 1, (i + 1) * SQ - 1);
            int len = R - L + 1;

            ans += block[i] + lazy[i] * len;
        }

        for (int i = l; i < (st + 1) * SQ && i < n; i++) ans += arr[i] + lazy[st];

        for (int i = en * SQ; i <= r && i < n; i++) ans += arr[i] + lazy[en];

        return ans;
    }
};
