// Gaussian Elimination Mod: linear algebra over the prime field Z_p (solve, rank, det, inverse).
// Use when: equations / determinant / rank / inverse "modulo 998244353" or any prime, counting
//           solutions (p^(m - rank)), spanning trees mod p (det of the Laplacian minor).
// Usage: Gauss_Mod g(p);  g.rank(a);  g.det(a);  g.inverse(a, inv) -> false if singular;
//        g.solve(A, b, x, basis) -> false if no solution, else x = one solution and
//        basis = null space of A (every solution is x + any combination of basis).
// Handles: rectangular n x m, singular systems, zero rows/columns, entries outside [0, p).
// Not: composite moduli (pivots need inverses); use Gaussian_Elimination.cpp for doubles / XOR.
// Time: O(n * m * min(n, m)); inverse O(n^3). n = m = 500 runs in well under a second.
// Indexing: 0-based; A is n rows of m columns, b has n entries, x and basis vectors have m.
// Note: p must be prime and < 2^31 (products fit in long long). Matrices are taken by value.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Gauss_Mod {
    ll mod;
    explicit Gauss_Mod(ll p = 998244353) : mod(p) {}

    ll power(ll b, ll e) const {
        ll r = 1 % mod;
        for (b %= mod; e; e >>= 1, b = b * b % mod)
            if (e & 1) r = r * b % mod;
        return r;
    }

    vector<int> rref(vector<vector<ll>> &a, int lim, ll &det) const {
        int n = a.size(), row = 0;
        vector<int> piv;
        det = 1 % mod;
        for (auto &r : a)
            for (auto &v : r) v = (v % mod + mod) % mod;
        for (int c = 0; c < lim && row < n; c++) {
            int p = row;
            while (p < n && a[p][c] == 0) p++;
            if (p == n) {
                det = 0;
                continue;
            }
            if (p != row) swap(a[p], a[row]), det = (mod - det) % mod;
            det = det * a[row][c] % mod;
            ll inv = power(a[row][c], mod - 2);
            int w = a[row].size();
            for (int j = c; j < w; j++) a[row][j] = a[row][j] * inv % mod;
            for (int i = 0; i < n; i++) {
                ll f = a[i][c];
                if (i == row || f == 0) continue;
                for (int j = c; j < w; j++) {
                    a[i][j] = (a[i][j] - f * a[row][j]) % mod;
                    if (a[i][j] < 0) a[i][j] += mod;
                }
            }
            piv.push_back(c), row++;
        }
        return piv;
    }

    int rank(vector<vector<ll>> a) const {
        ll d;
        return rref(a, a.empty() ? 0 : a[0].size(), d).size();
    }

    ll det(vector<vector<ll>> a) const { // a must be square
        ll d;
        rref(a, a.size(), d);
        return d;
    }

    bool inverse(vector<vector<ll>> a, vector<vector<ll>> &inv) const {
        int n = a.size();
        for (int i = 0; i < n; i++) a[i].resize(2 * n, 0), a[i][n + i] = 1;
        ll d;
        if ((int)rref(a, n, d).size() < n) return false;
        inv.assign(n, vector<ll>(n));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) inv[i][j] = a[i][n + j];
        return true;
    }

    bool solve(vector<vector<ll>> A, vector<ll> b, vector<ll> &x, vector<vector<ll>> &basis) const {
        int n = A.size(), m = n ? A[0].size() : 0;
        for (int i = 0; i < n; i++) A[i].push_back(b[i]);
        ll d;
        vector<int> piv = rref(A, m, d);
        int r = piv.size();
        for (int i = r; i < n; i++)
            if (A[i][m]) return false;
        x.assign(m, 0);
        for (int i = 0; i < r; i++) x[piv[i]] = A[i][m];
        basis.clear();
        vector<bool> is_piv(m, false);
        for (int c : piv) is_piv[c] = true;
        for (int f = 0; f < m; f++) {
            if (is_piv[f]) continue;
            vector<ll> v(m, 0);
            v[f] = 1;
            for (int i = 0; i < r; i++) v[piv[i]] = (mod - A[i][f]) % mod;
            basis.push_back(v);
        }
        return true;
    }
};
