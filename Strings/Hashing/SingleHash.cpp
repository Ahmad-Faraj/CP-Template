// بسم الله الرحمن الرحيم
#include <bits/stdc++.h>
using namespace std;

#define int long long
#define ll long long
#define nl '\n'

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int rand(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}

const int MOD = 1e9 + 7;
const int BASE = rand(31, 1e9);

// 0-based Indexing
struct Hash {
    int Base, Mod, inv, n;
    vector<ll> pw{1}, invpw{1}, pref{0};

    Hash() {
    }

    Hash(const string &s, int m = MOD, int b = BASE) {
        Mod = m;
        Base = b;
        n = s.size();

        // Calculate Base^(-1) using Fermat's Little Theorem
        inv = 1;
        ll curr = Base;
        int e = Mod - 2;

        while (e) {
            if (e & 1) inv = (inv * curr) % Mod;

            curr = (curr * curr) % Mod;
            e >>= 1;
        }

        // Build hash prefix
        for (int i = 0; i < n; i++) {
            pw.push_back(pw.back() * Base % Mod);
            invpw.push_back(invpw.back() * inv % Mod);

            ll ch = s[i] - 'a' + 1;

            pref.push_back((pref.back() + ch * pw[i]) % Mod);
        }
    }

    int query(int l, int r) {
        if (l == 0) return pref[r + 1];

        return (pref[r + 1] - pref[l] + Mod) % Mod * invpw[l] % Mod;
    }
};