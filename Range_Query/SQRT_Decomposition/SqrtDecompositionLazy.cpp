#include "../../core.h"

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

// 0-based Indexing
template <typename T> struct SQRT {
    int n, SQ, BlocksNum;

    vector<T> arr, block, lazy;

    SQRT(int N) {
        n = N;
        SQ = sqrt(n) + 1;
        BlocksNum = (n + SQ - 1) / SQ;

        arr.assign(n, T(0));
        block.assign(BlocksNum, T(0));
        lazy.assign(BlocksNum, T(0));
    }

    SQRT(const vector<T> &nums) {
        n = nums.size();
        SQ = sqrt(n) + 1;
        BlocksNum = (n + SQ - 1) / SQ;

        arr = nums;
        block.assign(BlocksNum, T(0));
        lazy.assign(BlocksNum, T(0));

        for (int i = 0; i < n; i++) {
            block[i / SQ] += arr[i];
        }
    }

    // Get number of elements inside block b
    int block_size(int b) const {
        int L = b * SQ;
        int R = min(n, (b + 1) * SQ);

        return R - L;
    }

    // Rebuild block b
    void rebuild(int b) {
        block[b] = T(0);

        int L = b * SQ;
        int R = min(n, (b + 1) * SQ);

        for (int i = L; i < R; i++) {
            block[b] += arr[i];
        }
    }

    // Range addition: a[i] += x for i in [l, r]
    void update(int l, int r, T x) {
        if (l > r) return;

        int bl = l / SQ;
        int br = r / SQ;

        // Same block
        if (bl == br) {
            for (int i = l; i <= r; i++) {
                arr[i] += x;
                block[bl] += x;
            }

            return;
        }

        int left_end = min(n, (bl + 1) * SQ);

        for (int i = l; i < left_end; i++) {
            arr[i] += x;
            block[bl] += x;
        }

        for (int b = bl + 1; b < br; b++) {
            lazy[b] += x;
            block[b] += x * block_size(b);
        }

        int right_start = br * SQ;

        for (int i = right_start; i <= r; i++) {
            arr[i] += x;
            block[br] += x;
        }
    }

    T query(int r) const {
        if (r < 0) return T(0);
        if (r >= n) r = n - 1;

        T ans = T(0);

        int b = r / SQ;

        for (int i = 0; i < b; i++) {
            ans += block[i];
        }

        int L = b * SQ;

        for (int i = L; i <= r; i++) {
            ans += arr[i] + lazy[b];
        }

        return ans;
    }

    // Range Query
    T query(int l, int r) const {
        if (l > r) return T(0);

        int bl = l / SQ;
        int br = r / SQ;

        T ans = T(0);

        if (bl == br) {
            for (int i = l; i <= r; i++) {
                ans += arr[i] + lazy[bl];
            }

            return ans;
        }

        int left_end = min(n, (bl + 1) * SQ);

        for (int i = l; i < left_end; i++) {
            ans += arr[i] + lazy[bl];
        }

        for (int b = bl + 1; b < br; b++) {
            ans += block[b];
        }

        int right_start = br * SQ;

        for (int i = right_start; i <= r; i++) {
            ans += arr[i] + lazy[br];
        }

        return ans;
    }
};
