#include "../../core.h"
/*
    [1] Definition
        Prefix Sum -> fast rectangle-sum queries.
        Difference Array -> fast rectangle updates.

    [2] Time & Space Complexity
        Prefix: Build O(n*m), Query O(1), Space O(n*m).
        Difference: Update O(1), Build O(n*m), Space O(n*m).

    [3] Tips
        Use 1-based indexing for easier queries.
        Use ll for large sums and handle x1 > x2 / y1 > y2.
*/

// returns 1-based prefix
vector<vector<ll>> prefix_sum(const vector<vector<int>> &arr) {
    int n = arr.size();
    int m = arr[0].size();

    vector<vector<ll>> prefix(n + 1, vector<ll>(m + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            prefix[i][j] = arr[i - 1][j - 1] + prefix[i - 1][j] + prefix[i][j - 1] - prefix[i - 1][j - 1];
        }
    }

    return prefix;
}

ll query(int x1, int y1, int x2, int y2, const vector<vector<ll>> &pre) {
    if (x1 > x2) swap(x1, x2);

    if (y1 > y2) swap(y1, y2);

    return pre[x2][y2] - pre[x1 - 1][y2] - pre[x2][y1 - 1] + pre[x1 - 1][y1 - 1];
}

// returns 0-based difference array
vector<vector<ll>> partial_sum(vector<vector<ll>> arr, int q) {
    int n = arr.size();
    int m = arr[0].size();

    vector<vector<ll>> diff(n + 2, vector<ll>(m + 2, 0));

    while (q--) {
        int x1, y1, x2, y2;
        ll val;

        cin >> x1 >> y1 >> x2 >> y2 >> val;

        if (x1 > x2) swap(x1, x2);

        if (y1 > y2) swap(y1, y2);

        diff[x1][y1] += val;
        diff[x2 + 1][y1] -= val;
        diff[x1][y2 + 1] -= val;
        diff[x2 + 1][y2 + 1] += val;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) diff[i][j] += diff[i - 1][j];
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) diff[i][j] += diff[i][j - 1];
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) arr[i - 1][j - 1] += diff[i][j];
    }

    return arr;
}