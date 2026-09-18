#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define sz(x) (int)x.size()
#define nl '\n'

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int rand(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}

const int M1 = 1e9 + 7;
const int M2 = 1e9 + 9;

const int B1 = rand(31, 1e9);
const int B2 = rand(31, 1e9);

// 0-based Indexing
struct Hash {
    int inv1, inv2, n;

    vector<ll> pw1{1}, pw2{1};
    vector<ll> invpw1{1}, invpw2{1};
    vector<ll> pref1{0}, pref2{0};

    Hash() {
    }

    Hash(const string &s) {
        n = s.size();

        // Calculate B1^(-1) mod M1
        inv1 = 1;
        ll curr = B1;
        int e = M1 - 2;

        while (e) {
            if (e & 1) inv1 = (inv1 * curr) % M1;

            curr = (curr * curr) % M1;
            e >>= 1;
        }

        // Calculate B2^(-1) mod M2
        inv2 = 1;
        curr = B2;
        e = M2 - 2;

        while (e) {
            if (e & 1) inv2 = (inv2 * curr) % M2;

            curr = (curr * curr) % M2;
            e >>= 1;
        }

        // Build hash prefix
        for (int i = 0; i < n; i++) {
            ll ch = s[i] - 'a' + 1;

            pw1.push_back(pw1.back() * B1 % M1);
            pw2.push_back(pw2.back() * B2 % M2);

            invpw1.push_back(invpw1.back() * inv1 % M1);
            invpw2.push_back(invpw2.back() * inv2 % M2);

            pref1.push_back((pref1.back() + ch * pw1[i]) % M1);

            pref2.push_back((pref2.back() + ch * pw2[i]) % M2);
        }
    }

    pair<int, int> query(int l, int r) {
        int h1, h2;

        if (l == 0) {
            h1 = pref1[r + 1];
            h2 = pref2[r + 1];
        } else {
            h1 = (pref1[r + 1] - pref1[l] + M1) % M1;
            h1 = h1 * invpw1[l] % M1;

            h2 = (pref2[r + 1] - pref2[l] + M2) % M2;
            h2 = h2 * invpw2[l] % M2;
        }

        return {h1, h2};
    }
};
