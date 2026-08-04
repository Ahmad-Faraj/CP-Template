#include "../core.h"

/*
 * Topic: DP - Sum Over Subsets (SOS DP)
 * Description: Efficiently computes the sum of a function over all subsets or supersets 
 * of bitmasks in O(N 2^N) instead of O(3^N).
 * Input: An array representing function values for each bitmask (e.g., frequencies).
 * Output: An array where the i-th element contains the sum over all submasks/supermasks of i.
 * Problem link: https://cses.fi/problemset/task/1654
 */

const int B = 20;
int a[1 << B], f[1 << B], g[1 << B];
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        f[a[i]]++;
        g[a[i]]++;
    }

    // sum over subsets
    for (int i = 0; i < B; i++) {
        for (int mask = 0; mask < (1 << B); mask++) {
            if ((mask & (1 << i)) != 0) {
                f[mask] += f[mask ^ (1 << i)];
            }
        }
    }

    // sum over supersets
    for (int i = 0; i < B; i++) {
        for (int mask = (1 << B) - 1; mask >= 0; mask--) {
            if ((mask & (1 << i)) == 0) g[mask] += g[mask ^ (1 << i)];
        }
    }

    for (int i = 1; i <= n; i++) {
        cout << f[a[i]] << ' ' << g[a[i]] << ' ' << n - f[(1 << B) - 1 - a[i]] << '\n';
    }
    return 0;
}
// https://cses.fi/problemset/task/1654