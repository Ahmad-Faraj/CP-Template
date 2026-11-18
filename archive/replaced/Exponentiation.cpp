#include <bits/stdc++.h>
using namespace std;
#define ll long long

ll bin_pow(ll x, ll n) {
    ll res = 1;
    while (n) {
        if (n & 1) res = res * x;
        x = x * x;
        n >>= 1;
    }
    return res;
}

ll pow_mod(ll x, ll n, ll mod) {
    ll res = 1;
    while (n) {
        if (n & 1) res = res * x % mod;
        x = x * x % mod;
        n >>= 1;
    }
    return res;
}

ll mod_inv(ll x, ll mod) { return pow_mod(x, mod - 2, mod); }

vector<int> apply_permutation(vector<int> a, vector<int> p) {
    int n = (int)a.size();
    vector<int> res(n);
    for (int i = 0; i < n; i++) {
        res[i] = a[p[i]];
    }
    return res;
}

vector<int> permute_k_times(vector<int> a, vector<int> p, ll k) {
    while (k > 0) {
        if (k & 1) a = apply_permutation(a, p);
        p = apply_permutation(p, p);
        k >>= 1;
    }
    return a;
}