#include <bits/stdc++.h>
using namespace std;
#define ll long long

// get mod for arthimitic operations
#define add_mod(a, b, m) (((a % m) + (b % m)) % m)
#define sub_mod(a, b, m) (((a % m) - (b % m) + m) % m)
#define mul_mod(a, b, m) (((a % m) * (b % m)) % m)

const int MOD = 1e9 + 7;

// get a mod for big int
ll Big_Mod(string s, ll mod) {
    ll res = 0;
    for (auto &c : s) res = (res * 10 + (c - '0')) % mod;
    return res;
}

ll mod_pow(ll a, ll b, ll m = MOD) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

ll mod_inv(ll a, ll m = MOD) {
    return mod_pow(a, m - 2, m); // Fermat
}