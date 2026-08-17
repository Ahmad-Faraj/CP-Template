#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define sz(x) (int)x.size()
#define nl '\n'

/*
    [1] Definition
        Find the sum of sums of all subarrays inside [l, r].

    [2] Time & Space Complexity
        Preprocessing: O(n), Query: O(1), Space: O(n).
        Uses prefix sums + weighted prefix sums.

    [3] Tips
        First prefix -> sum of a[i].
        Second prefix -> sum of prefix sums.
        Key formula: a[i] * (i-l+1) * (r-i+1).
*/

int main() {

    int n;
    cin >> n;
    vector<ll> v(n + 1);

    for (int i = 1; i <= n; i++) cin >> v[i];

    // First prefix sum
    for (int i = 1; i <= n; i++) v[i] += v[i - 1];

    vector<ll> pre1(n + 1), pre2(n + 1);

    for (int i = 1; i <= n; i++) {
        pre1[i] = v[i] * (i + 1);
        pre2[i] = v[i] * i;

        pre1[i] += pre1[i - 1];
        pre2[i] += pre2[i - 1];
    }

    // Second prefix sum
    for (int i = 1; i <= n; i++) v[i] += v[i - 1];

    int q;
    cin >> q;
    while (q--) {
        int l, r;
        cin >> l >> r;

        ll ans = pre1[r] - pre1[l - 1];

        ans -= (v[r] - v[l - 1]) * l;

        ll tmp = (v[r - 1] - (l == 1 ? 0 : v[l - 2])) * r;

        tmp -= (pre2[r - 1] - (l == 1 ? 0 : pre2[l - 2]));

        cout << ans - tmp << nl;
    }

    return 0;
}