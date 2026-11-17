#include <bits/stdc++.h>
using namespace std;

// 0-based indexing
// O(nlogn) preprocessing // O(1) query // O(nlogn) memory
// used for range (sum, min, max, gcd, lcm, xor, and, or)

vector<vector<int>> sparse_table;
vector<int> arr;
int LOG = 20;
void solve() {
    int n;
    cin >> n;
    LOG = log2(n) + 1;
    arr.resize(n);
    sparse_table.resize(n, vector<int>(LOG));
    for (int i = 0; i < n; ++i) cin >> arr[i];
    for (int i = 0; i < n; ++i) sparse_table[i][0] = arr[i];
    for (int j = 1; j < LOG; ++j)
        for (int i = 0; i + (1 << j) <= n; ++i)
            sparse_table[i][j] = min(sparse_table[i][j - 1], sparse_table[i + (1 << (j - 1))][j - 1]);
    int q;
    cin >> q;
    while (q--) {
        int l, r;
        cin >> l >> r;
        int len = r - l + 1;
        int k = log2(len);
        cout << min(sparse_table[l][k], sparse_table[r - (1 << k) + 1][k]) << endl;
    }
}