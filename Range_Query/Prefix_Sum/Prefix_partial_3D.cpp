#include "../../core.h"
/*
    [1] Definition
        Prefix Sum -> fast cuboid-sum queries.
        Difference Array -> fast cuboid updates.

    [2] Time & Space Complexity
        Prefix: Build O(n*m*k), Query O(1), Space O(n*m*k).
        Difference: Update O(1), Build O(n*m*k), Space O(n*m*k).

    [3] Tips
        Use 1-based indexing for easier queries.
        Use ll and remember: D dimensions -> 2^D corners.
*/

// returns 1-based prefix
vector<vector<vector<ll>>> build_prefix_sum(const vector<vector<vector<ll>>> &arr) {
    int n = arr.size();
    int m = arr[0].size();
    int k = arr[0][0].size();

    vector<vector<vector<ll>>> prefix(n + 1, vector<vector<ll>>(m + 1, vector<ll>(k + 1, 0)));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            for (int z = 1; z <= k; z++) {
                prefix[i][j][z] = arr[i - 1][j - 1][z - 1] + prefix[i - 1][j][z] + prefix[i][j - 1][z] +
                                  prefix[i][j][z - 1] - prefix[i - 1][j - 1][z] - prefix[i - 1][j][z - 1] -
                                  prefix[i][j - 1][z - 1] + prefix[i - 1][j - 1][z - 1];
            }
        }
    }
    return prefix;
}

ll query(int x1, int y1, int z1, int x2, int y2, int z2, const vector<vector<vector<ll>>> &pre) {
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);
    if (z1 > z2) swap(z1, z2);

    return pre[x2][y2][z2] - pre[x1 - 1][y2][z2] - pre[x2][y1 - 1][z2] - pre[x2][y2][z1 - 1] + pre[x1 - 1][y1 - 1][z2] +
           pre[x1 - 1][y2][z1 - 1] + pre[x2][y1 - 1][z1 - 1] - pre[x1 - 1][y1 - 1][z1 - 1];
}

// returns 0-based difference array
void partial_sum(vector<vector<vector<ll>>> &arr, int q) {
    int n = arr.size();
    int m = arr[0].size();
    int k = arr[0][0].size();

    vector<vector<vector<ll>>> diff(n + 2, vector<vector<ll>>(m + 2, vector<ll>(k + 2, 0)));

    while (q--) {
        int x1, y1, z1, x2, y2, z2;
        ll val;
        cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> val;

        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        if (z1 > z2) swap(z1, z2);

        diff[x1][y1][z1] += val;

        diff[x2 + 1][y1][z1] -= val;
        diff[x1][y2 + 1][z1] -= val;
        diff[x1][y1][z2 + 1] -= val;

        diff[x2 + 1][y2 + 1][z1] += val;
        diff[x2 + 1][y1][z2 + 1] += val;
        diff[x1][y2 + 1][z2 + 1] += val;

        diff[x2 + 1][y2 + 1][z2 + 1] -= val;
    }

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            for (int z = 1; z <= k; z++) diff[i][j][z] += diff[i - 1][j][z];

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            for (int z = 1; z <= k; z++) diff[i][j][z] += diff[i][j - 1][z];

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            for (int z = 1; z <= k; z++) diff[i][j][z] += diff[i][j][z - 1];

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            for (int z = 1; z <= k; z++) arr[i - 1][j - 1][z - 1] += diff[i][j][z];
}