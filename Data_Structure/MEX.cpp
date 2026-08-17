// MEX: the smallest non-negative integer missing from an array.
// Use when: "smallest number not present", Sprague-Grundy values, the mex of a set of reachable states.
// Handles: negatives, duplicates, values of any size. See the MEX_* variants for ranges and updates.
// Time: O(n)
// Indexing: none
// Note: the answer always lies in [0, n], so anything outside that range cannot affect it and is skipped.

#include <bits/stdc++.h>
using namespace std;

int mex(const vector<int> &a) {
    int n = (int)a.size();
    vector<bool> seen(n + 2, false);
    for (int x : a)
        if (0 <= x && x <= n) seen[x] = true;
    int r = 0;
    while (seen[r]) r++;
    return r;
}

// Standard problem: report the mex of the given array
void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &x : a) cin >> x;
    cout << mex(a) << '\n';
}
