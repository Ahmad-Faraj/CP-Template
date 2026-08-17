#include "../../core.h"
/*
    [1] Definition
    Square Root (SQRT) Decomposition dividing an array of size N into blocks of size O(sqrt(N)).
    Supports O(1) point assignment updates and O(sqrt(N)) prefix and range sum queries.

    [2] Time & Space Complexity
    Build: O(N)
    Point Update: O(1)
    Prefix / Range Query: O(sqrt(N))
    Space: O(N)

    [3] Notes
    Uses 0-based indexing for queries and updates.
    Calculates block size as SQ = sqrt(N) + 1.
    Maintains prefix and range sums over generic type T with identity T(0).
*/

template <typename T> struct SQRT {
    int n, SQ, BlocksNum;
    vector<T> arr, block;

    SQRT(int N) {
        n = N;
        SQ = sqrt(n) + 1;
        BlocksNum = (n + SQ - 1) / SQ;
        arr.assign(n, T(0));
        block.assign(BlocksNum, T(0));
    }

    SQRT(const vector<T> &nums) {
        n = nums.size();
        SQ = sqrt(n) + 1;
        BlocksNum = (n + SQ - 1) / SQ;
        arr = nums;
        block.assign(BlocksNum, T(0));
        for (int i = 0; i < n; i++) block[i / SQ] += arr[i];
    }

    void update(int idx, T val) {
        block[idx / SQ] -= arr[idx];
        arr[idx] = val;
        block[idx / SQ] += arr[idx];
    }

    T query(int r) {
        if (r < 0) return T(0);
        T ans = T(0);
        for (int i = 0; i < r / SQ; i++) ans += block[i];
        for (int i = (r / SQ) * SQ; i <= r && i < n; i++) ans += arr[i];
        return ans;
    }

    T query(int l, int r) {
        if (l > r) return T(0);
        int st = l / SQ;
        int en = r / SQ;
        T ans = T(0);
        if (st == en) {
            for (int i = l; i <= r; i++) ans += arr[i];
            return ans;
        }
        for (int i = st + 1; i < en; i++) ans += block[i];
        for (int i = l; i < (st + 1) * SQ && i < n; i++) ans += arr[i];
        for (int i = en * SQ; i <= r && i < n; i++) ans += arr[i];
        return ans;
    }
};