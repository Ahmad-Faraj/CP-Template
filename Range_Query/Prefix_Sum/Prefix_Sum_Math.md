# Prefix Sum Math

## All Subarrays Sum of a Subarray

Sum of sums of all subarrays of a given subarray `[L, R]`.

```cpp
vector<int> v(n + 1);
for (int i = 1; i <= n; i++) cin >> v[i];
for (int i = 1; i <= n; i++) v[i] += v[i - 1]; // First prefix sum

vector<int> pref(n + 1), pref2(n + 1);
for (int i = 1; i <= n; i++) {
    pref[i] = v[i] * (i + 1);
    pref2[i] = v[i] * i;
    if (i) {
        pref[i] += pref[i - 1];
        pref2[i] += pref2[i - 1];
    }
}
for (int i = 1; i <= n; i++) v[i] += v[i - 1]; // Second prefix sum

while (q--) {
    int l, r;
    cin >> l >> r;
    
    int res = pref[r] - pref[l - 1];
    res -= (v[r] - v[l - 1]) * l;
    
    int temp = (v[r - 1] - (l - 2 == -1 ? 0 : v[l - 2])) * r;
    temp -= (pref2[r - 1] - (l - 2 == -1 ? 0 : pref2[l - 2]));
    
    cout << res - temp << '\n';
}
```
