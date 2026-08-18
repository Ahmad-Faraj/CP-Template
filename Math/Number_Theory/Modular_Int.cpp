// Modular Int: a number type that carries the modulus, so ordinary + - * / on it are already reduced.
// Use when: any modular DP or counting problem - it removes every hand-written % MOD, which is where the bugs live.
// Handles: negative input, subtraction below zero, division by an invertible value, powers, streaming in and out.
// Time: O(1) for + - *, O(log mod) for division and inverse
// Indexing: not applicable
// Note: division needs a PRIME modulus, since the inverse uses Fermat. Products go through long long, never int.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <int MOD = 1000000007> struct Modular_Int {
    int value;

    Modular_Int(ll v = 0) { value = (int)((v % MOD + MOD) % MOD); } // negatives land in range too

    static int mod() { return MOD; }
    explicit operator int() const { return value; }

    Modular_Int &operator+=(const Modular_Int &o) {
        if ((value += o.value) >= MOD) value -= MOD;
        return *this;
    }
    Modular_Int &operator-=(const Modular_Int &o) {
        if ((value -= o.value) < 0) value += MOD;
        return *this;
    }
    Modular_Int &operator*=(const Modular_Int &o) {
        value = (int)((ll)value * o.value % MOD); // ll, or this overflows for MOD past 46341
        return *this;
    }
    Modular_Int &operator/=(const Modular_Int &o) { return *this *= o.inverse(); }

    friend Modular_Int operator+(Modular_Int a, const Modular_Int &b) { return a += b; }
    friend Modular_Int operator-(Modular_Int a, const Modular_Int &b) { return a -= b; }
    friend Modular_Int operator*(Modular_Int a, const Modular_Int &b) { return a *= b; }
    friend Modular_Int operator/(Modular_Int a, const Modular_Int &b) { return a /= b; }

    Modular_Int operator-() const { return Modular_Int(-value); }
    Modular_Int &operator++() { return *this += 1; }
    Modular_Int &operator--() { return *this -= 1; }

    friend bool operator==(const Modular_Int &a, const Modular_Int &b) { return a.value == b.value; }
    friend bool operator!=(const Modular_Int &a, const Modular_Int &b) { return a.value != b.value; }

    Modular_Int power(ll e) const {
        Modular_Int base = *this, result = 1;
        if (e < 0) return power(-e).inverse();
        while (e) {
            if (e & 1) result *= base;
            base *= base;
            e >>= 1;
        }
        return result;
    }

    Modular_Int inverse() const { return power(MOD - 2); } // Fermat, so MOD must be prime

    friend istream &operator>>(istream &in, Modular_Int &m) {
        ll v;
        in >> v;
        m = Modular_Int(v);
        return in;
    }
    friend ostream &operator<<(ostream &out, const Modular_Int &m) { return out << m.value; }
};

using mint = Modular_Int<1000000007>;

// factorials and binomials over the same type, built once
struct Mod_Combinatorics {
    vector<mint> fact, inv_fact;
    Mod_Combinatorics(int n) : fact(n + 1), inv_fact(n + 1) {
        fact[0] = 1;
        for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i;
        inv_fact[n] = fact[n].inverse();
        for (int i = n; i > 0; i--) inv_fact[i - 1] = inv_fact[i] * i;
    }
    mint nCr(int n, int r) { return (r < 0 || r > n) ? mint(0) : fact[n] * inv_fact[r] * inv_fact[n - r]; }
    mint nPr(int n, int r) { return (r < 0 || r > n) ? mint(0) : fact[n] * inv_fact[n - r]; }
};

// Standard problem: read n and r, print n! and nCr modulo 1e9+7
void solve() {
    int n, r;
    cin >> n >> r;
    Mod_Combinatorics c(max(n, 1));
    cout << c.fact[n] << ' ' << c.nCr(n, r) << '\n';
}
