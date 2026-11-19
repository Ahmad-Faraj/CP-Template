// Mod Int: an integer mod a fixed prime that does + - * / and powers with plain operators.
// Use when: "print the answer modulo 1e9+7 / 998244353", DP counts, combinatorics, expected values
// as p * q^-1, anywhere a forgotten % would overflow.
// Usage: using mint = ModInt<1000000007>; mint a = -5, b = 3; mint c = a * b / 2 + 1; c.val;
//        a.power(k); a.inverse(); 7 - a (plain integers mix in on either side); a == 3
// Handles: negative or huge (any ll) inputs, normalised into [0, MOD); negative exponents.
// Time: + - * O(1) | / inverse power O(log MOD)
// Indexing: none
// Note: MOD must be a prime below 2^31. inverse(0) = 0.
//       No stream operators: read an ll and assign it, print x.val.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <ll MOD = 1000000007> struct ModInt {
    ll val; // always in [0, MOD)

    ModInt(ll v = 0) : val(v % MOD) {
        if (val < 0) val += MOD;
    }

    ModInt &operator+=(const ModInt &o) {
        if ((val += o.val) >= MOD) val -= MOD;
        return *this;
    }
    ModInt &operator-=(const ModInt &o) {
        if ((val -= o.val) < 0) val += MOD;
        return *this;
    }
    ModInt &operator*=(const ModInt &o) {
        val = val * o.val % MOD;
        return *this;
    }
    ModInt &operator/=(const ModInt &o) { return *this *= o.inverse(); }

    friend ModInt operator+(ModInt a, const ModInt &b) { return a += b; }
    friend ModInt operator-(ModInt a, const ModInt &b) { return a -= b; }
    friend ModInt operator*(ModInt a, const ModInt &b) { return a *= b; }
    friend ModInt operator/(ModInt a, const ModInt &b) { return a /= b; }
    ModInt operator-() const { return ModInt(0) - *this; }

    ModInt &operator++() { return *this += 1; }
    ModInt &operator--() { return *this -= 1; }
    ModInt operator++(signed) { ModInt r = *this; ++*this; return r; }
    ModInt operator--(signed) { ModInt r = *this; --*this; return r; }

    friend bool operator==(const ModInt &a, const ModInt &b) { return a.val == b.val; }
    friend bool operator!=(const ModInt &a, const ModInt &b) { return a.val != b.val; }

    ModInt power(ll e) const { // e < 0 means inverse^|e|
        if (e < 0) return inverse().power(-e);
        ModInt a = *this, res = 1;
        for (; e > 0; e >>= 1, a *= a)
            if (e & 1) res *= a;
        return res;
    }
    ModInt inverse() const { return power(MOD - 2); }
};
using mint = ModInt<1000000007>;
