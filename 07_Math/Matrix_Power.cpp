// Matrix Power: matrices mod p with + - * and fast power, plus fast v * A^k jumps via A^(2^i).
// Use when: linear recurrence with huge n ("f(n) = sum of last M terms, n <= 1e18"), counting walks
// of length k, many jumps of one matrix ("Q overrides / queries, each jump M^2 log n, not M^3").
// Usage: Matrix A(m, m); A.mat[i][j] = ..; Matrix B = A ^ k; (A * B), (A + B), (A - B);
//        Matrix_Jumper J(A, 60); vector<ll> w = J.jump(v, k); // row vector v * A^k
// Handles: negative entries (normalised on load), k = 0 (identity), non-square products.
// Time: * O(n m p) | ^ O(m^3 log k) | Jumper build O(m^3 LOG), each jump O(m^2 log k)
// Indexing: 0-based mat[row][col]; v is a row vector (state as a row, transition A[from][to])
// Note: Matrix::mod is shared by all matrices (default 1e9+7), set it before use; mod < 2^31.
// Entries must stay in [0, mod) if you write mat[i][j] directly (the vector ctor normalises).
// Source: other/mazen/Matrix_Power.cpp (macros removed); jumps: ACPC 2024 F precomputed powers.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Matrix {
    static inline ll mod = 1000000007;
    vector<vector<ll>> mat;
    int n, m;
    Matrix(int n, int m) : mat(n, vector<ll>(m, 0)), n(n), m(m) {}
    Matrix(const vector<vector<ll>> &a) : Matrix(a.size(), a[0].size()) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) mat[i][j] = ((a[i][j] % mod) + mod) % mod;
    }
    static Matrix identity(int n) {
        Matrix c(n, n);
        for (int i = 0; i < n; i++) c.mat[i][i] = 1;
        return c;
    }
    Matrix operator+(const Matrix &b) const {
        Matrix c(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) c.mat[i][j] = (mat[i][j] + b.mat[i][j]) % mod;
        return c;
    }
    Matrix operator-(const Matrix &b) const {
        Matrix c(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) c.mat[i][j] = (mat[i][j] - b.mat[i][j] + mod) % mod;
        return c;
    }
    Matrix operator*(const Matrix &b) const { // (n x m) * (m x b.m)
        Matrix c(n, b.m);
        for (int i = 0; i < n; i++)
            for (int k = 0; k < m; k++) {
                if (!mat[i][k]) continue;
                for (int j = 0; j < b.m; j++)
                    c.mat[i][j] = (c.mat[i][j] + mat[i][k] * b.mat[k][j]) % mod;
            }
        return c;
    }
    Matrix operator^(ll p) const { // square matrix, p >= 0
        Matrix c = identity(n), a = *this;
        for (; p > 0; p >>= 1, a = a * a)
            if (p & 1) c = c * a;
        return c;
    }
};

// row vector times matrix: O(n m)
vector<ll> vec_mul(const vector<ll> &v, const Matrix &A) {
    vector<ll> r(A.m, 0);
    for (int k = 0; k < A.n; k++) {
        if (!v[k]) continue;
        for (int j = 0; j < A.m; j++) r[j] = (r[j] + v[k] * A.mat[k][j]) % Matrix::mod;
    }
    return r;
}

struct Matrix_Jumper { // pw[i] = A^(2^i); jump never multiplies two matrices
    vector<Matrix> pw;
    Matrix_Jumper(const Matrix &A, int LOG = 60) : pw(1, A) {
        for (int i = 1; i < LOG; i++) pw.push_back(pw.back() * pw.back());
    }
    vector<ll> jump(vector<ll> v, ll k) const { // v * A^k, needs k < 2^LOG
        for (auto &x : v) x = (x % Matrix::mod + Matrix::mod) % Matrix::mod;
        for (int i = 0; k > 0; i++, k >>= 1)
            if (k & 1) v = vec_mul(v, pw[i]);
        return v;
    }
};
