/*
    [1] DESCRIPTION
    A static 0-based data structure for range queries on un-modifiable arrays.
    Supports O(1) range queries for idempotent operations (Min, Max, GCD, AND, OR)
    and O(log N) range queries for non-idempotent associative operations.

    [2] TIME & SPACE COMPLEXITY
    Build: O(N log N)
    One_query (Idempotent): O(1)
    query (General Range): O(log N)
    Space: O(N log N)

    [3] LIMITS
    Static data structure (No point/range updates supported).
    N <= 5e5, LOG = 23 (Pre-allocated table capacity).
    0-based indexing: 0 <= L <= R < N.
*/

#include "../../core.h"

const int N = 200005;
const int LOG = 23;
int sparse[N][LOG];

// Merge function (change for max, gcd, AND, OR, etc.)
int merge(int x, int y)
{
    return min(x, y);
}

// Build sparse table from a 0-based vector in O(N log N)
void build(vector<int> &nums)
{
    int n = sz(nums);

    for (int i = 0; i < n; i++)
        sparse[i][0] = nums[i];

    for (int k = 1; k < LOG; k++)
    {
        for (int i = 0; i + (1 << k) <= n; i++)
        {
            sparse[i][k] = merge(sparse[i][k - 1], sparse[i + (1LL << (k - 1))][k - 1]);
        }
    }
}

// O(log N) Range Query [l, r] using binary decomposition of range length
int query(int l, int r)
{
    int len = r - l + 1;
    int ans = INT_MAX;
    for (int i = LOG - 1; i >= 0; i--)
    {
        if (len & (1LL << i))
        {
            ans = merge(ans, sparse[l][i]);
            l += (1LL << i);
        }
    }
    return ans;
}

// don't forget resizing the const N