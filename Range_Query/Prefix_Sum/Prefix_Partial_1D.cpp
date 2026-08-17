#include "../../core.h"

/*
    [1] Definition
        Prefix Sum -> fast range-sum queries.
        Difference Array -> fast range updates.

    [2] Time & Space Complexity
        Prefix: Build O(n), Query O(1), Space O(n).
        Difference: Update O(1), Build O(n), Space O(n).

    [3] Tips
        Use 1-based indexing for easier queries.
        Use ll for large sums and always handle l > r.
*/

// returns 1-based prefix
vector<ll> prefix_sum(vector<int> arr) {
    int n = arr.size();

    vector<ll> prefix(n + 1, 0);

    for (int i = 1; i <= n; i++) prefix[i] = prefix[i - 1] + arr[i - 1];

    return prefix;
}

ll query(int l, int r, const vector<ll> &pre) {
    if (l > r) swap(l, r);

    return pre[r] - pre[l - 1];
}

vector<ll> partial_sum(vector<ll> arr, int q) {
    int n = arr.size();

    vector<ll> diff(n + 1, 0);

    while (q--) {
        int l, r;
        ll val;

        cin >> l >> r >> val;

        if (l > r) swap(l, r);

        diff[l] += val;
        diff[r + 1] -= val;
    }

    for (int i = 1; i <= n; i++) diff[i] += diff[i - 1];

    for (int i = 1; i <= n; i++) arr[i - 1] += diff[i];

    return arr;
}