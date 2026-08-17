// Exponentiation: applies an associative operation n times in O(log n) - on integers, matrices or permutations.
// Use when: "a^n mod m", a linear recurrence at huge n, "where does each element land after k shuffles".
// Handles: modular and non-modular integers, square matrices of any size, permutations, exponent 0, and n up to 9e18.
// Time: O(log n) for integers | O(k^3 log n) for k x k matrices | O(size log n) for permutations
// Indexing: matrices are 0-based; permutations are 0-based and must be a bijection on 0..size-1
// Note: pow_int has no modulus and is silently wrong past 2^63. Use pow_mod unless the result is provably tiny.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll pow_int(ll base, ll exp) { // no modulus: overflows beyond 2^63, use only for small results
    ll res = 1;
    while (exp > 0) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

ll pow_mod(ll base, ll exp, ll m) {
    ll res = 1 % m;
    base %= m;
    if (base < 0) base += m; // conditional, so a modulus near 9e18 cannot overflow
    while (exp > 0) {
        if (exp & 1) res = (ll)((__int128)res * base % m);
        base = (ll)((__int128)base * base % m);
        exp >>= 1;
    }
    return res;
}

struct Matrix {
    int n;
    ll mod;
    vector<vector<ll>> a;

    Matrix(int n, ll mod) : n(n), mod(mod), a(n, vector<ll>(n, 0)) {}

    static Matrix identity(int n, ll mod) {
        Matrix I(n, mod);
        for (int i = 0; i < n; i++) I.a[i][i] = 1 % mod;
        return I;
    }

    Matrix operator*(const Matrix &o) const {
        Matrix c(n, mod);
        for (int i = 0; i < n; i++)
            for (int k = 0; k < n; k++) {
                if (!a[i][k]) continue; // skipping zeros is most of the speed here
                for (int j = 0; j < n; j++)
                    c.a[i][j] = (c.a[i][j] + (__int128)a[i][k] * o.a[k][j]) % mod;
            }
        return c;
    }
};

Matrix pow_matrix(Matrix base, ll exp) { // exp 0 gives the identity
    Matrix res = Matrix::identity(base.n, base.mod);
    while (exp > 0) {
        if (exp & 1) res = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

vector<int> apply_permutation(const vector<int> &a, const vector<int> &p) { // result[i] = a[p[i]]
    int n = (int)a.size();
    vector<int> res(n);
    for (int i = 0; i < n; i++) res[i] = a[p[i]];
    return res;
}

vector<int> pow_permutation(vector<int> a, vector<int> p, ll k) { // a after applying p k times
    while (k > 0) {
        if (k & 1) a = apply_permutation(a, p);
        p = apply_permutation(p, p);
        k >>= 1;
    }
    return a;
}

// Standard problem: a, n and m - report a^n mod m
void solve() {
    ll a, n, m;
    cin >> a >> n >> m;
    cout << pow_mod(a, n, m) << '\n';
}
