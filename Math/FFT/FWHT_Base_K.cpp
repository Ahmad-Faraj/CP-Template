/*
 * Topic: FWHT Arbitrary Base
 * Description: Fast Walsh-Hadamard Transform for arbitrary base k
 * Reference: https://www.codechef.com/problems/XORTREEH
 */
#include "../../core.h"

const int FWHT_MOD = 330301441;

struct FWHT_Base_K {
    static int POW(long long n, long long k) {
        int ans = 1 % FWHT_MOD;
        n %= FWHT_MOD;
        if (n < 0) n += FWHT_MOD;
        while (k) {
            if (k & 1) ans = (long long)ans * n % FWHT_MOD;
            n = (long long)n * n % FWHT_MOD;
            k >>= 1;
        }
        return ans;
    }

    static const int MXK = 10; //(FWHT_MOD - 1) should be divisible by each base

    struct Matrix {
        int a[MXK][MXK], n, m;

        Matrix(int n, int m) : n(n), m(m) {}
    };

    static Matrix mul_mat(Matrix &p, Matrix &q) {
        Matrix result(p.n, q.m);
        for (int i = 0; i < p.n; i++)
            for (int j = 0; j < q.m; j++) {
                long long val = 0;
                for (int k = 0; k < p.m; k++) val += 1LL * p.a[i][k] * q.a[k][j];
                result.a[i][j] = val % FWHT_MOD;
            }
        return result;
    }

    static Matrix constructMatrix(int k, bool invert) {
        vector<int> divs;
        int x = FWHT_MOD - 1;
        for (int i = 2; i * i <= x; i++)
            if (x % i == 0) {
                divs.push_back((FWHT_MOD - 1) / i);
                while (x % i == 0) x /= i;
            }
        if (x > 1) divs.push_back((FWHT_MOD - 1) / x);
        int g = 2;
        while (true) {
            bool ok = true;
            for (int d : divs)
                if (POW(g, d) == 1) ok = false;
            if (ok) break;
            g++;
        }
        int root = POW(g, (FWHT_MOD - 1) / k);
        Matrix result(k, k);
        for (int i = 0; i < k; i++) {
            int cur = POW(root, invert ? k - i : i);
            for (int j = 0; j < k; j++) result.a[i][j] = POW(cur, j);
        }
        return result;
    }

    static void transform(vector<int> &a, int n, int k, bool invert) {
        Matrix M = constructMatrix(k, invert);
        for (int len = 1; len < n; len *= k)
            for (int i = 0; i < n; i += k * len)
                for (int j = 0; j < len; j++) {
                    Matrix V(1, k);
                    for (int p = 0; p < k; p++) V.a[0][p] = a[i + j + p * len];
                    V = mul_mat(V, M);
                    for (int p = 0; p < k; p++) a[i + j + p * len] = V.a[0][p];
                }
        if (invert) {
            long long d = POW(n, FWHT_MOD - 2);
            for (int i = 0; i < n; i++) a[i] = a[i] * d % FWHT_MOD;
        }
    }

    // O(k^d * k * d) where d = log_k(MAX VALUE)
    static vector<int> multiply(vector<int> a, vector<int> b, int k) {
        int n = 1;
        int nw = max((int)a.size() - 1, (int)b.size() - 1);
        while (n <= nw) n *= k;
        a.resize(n);
        b.resize(n);
        transform(a, n, k, false);
        transform(b, n, k, false);
        for (int i = 0; i < n; i++) a[i] = 1LL * a[i] * b[i] % FWHT_MOD;
        transform(a, n, k, true);
        return a;
    }

    static vector<int> poly_pow(vector<int> a, long long p, int k) {
        int n = 1;
        int nw = (int)a.size() - 1;
        while (n <= nw) n *= k;
        a.resize(n);
        transform(a, n, k, false);
        for (int i = 0; i < n; i++) a[i] = POW(a[i], p);
        transform(a, n, k, true);
        return a;
    }
};
