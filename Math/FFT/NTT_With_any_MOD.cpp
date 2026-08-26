/*
 * Topic: NTT_With_any_MOD
 * Description: NTT with arbitrary modulo using CRT (3 primes)
 */
#include "../../core.h"

struct NTT_With_any_MOD {
    // NTT for any MOD
    // use any mod here
    static const int MOD = 1e9 + 7;

    // Three moduli for CRT
    static const int mod1 = 167772161; // 2^25 * 5 + 1
    static const int mod2 = 469762049; // 2^26 * 7 + 1
    static const int mod3 = 754974721; // 2^24 * 45 + 1

    static const int root1 = 3;
    static const int root2 = 3;
    static const int root3 = 11;

    // Modular inverse using Fermat
    static int power(int a, int b, int mod) {
        int res = 1;
        while (b) {
            if (b & 1) res = res * a % mod;
            a = a * a % mod;
            b >>= 1;
        }
        return res;
    }

    static void ntt(vector<int> &a, int mod, int root, bool invert) {
        int n = sz(a);
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }

        for (int len = 2; len <= n; len <<= 1) {
            int wlen = power(root, (mod - 1) / len, mod);
            if (invert) wlen = power(wlen, mod - 2, mod);
            for (int i = 0; i < n; i += len) {
                int w = 1;
                for (int j = 0; j < len / 2; j++) {
                    int u = a[i + j], v = a[i + j + len / 2] * w % mod;
                    a[i + j] = (u + v) % mod;
                    a[i + j + len / 2] = (u - v + mod) % mod;
                    w = w * wlen % mod;
                }
            }
        }

        if (invert) {
            int inv_n = power(n, mod - 2, mod);
            for (int &x : a) x = x * inv_n % mod;
        }
    }

    static vector<int> multiply_mod(vector<int> a, vector<int> b, int mod, int root) {
        int n = 1;
        while (n < sz(a) + sz(b) - 1) n <<= 1;
        a.resize(n);
        b.resize(n);

        ntt(a, mod, root, false);
        ntt(b, mod, root, false);
        for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % mod;
        ntt(a, mod, root, true);

        return a;
    }

    static int mod_inv(int a, int m) { return power(a, m - 2, m); }

    static int crt(int r1, int r2, int r3) {
        int64_t m1 = mod1, m2 = mod2, m3 = mod3;

        int64_t m1_inv_m2 = mod_inv(m1, m2);
        int64_t m12 = m1 * m2;
        int64_t m12_inv_m3 = mod_inv(m12 % m3, m3);

        int64_t x1 = r1;
        int64_t x2 = ((r2 - x1 + m2) % m2) * m1_inv_m2 % m2;
        int64_t x3 = ((r3 - (x1 + m1 * x2) % m3 + m3) % m3) * m12_inv_m3 % m3;

        __int128 result = x1 + (__int128)m1 * x2 + (__int128)m12 * x3;
        return (int)(result % MOD);
    }

    static vector<int> multiply(vector<int> a, vector<int> b) {
        vector<int> x = multiply_mod(a, b, mod1, root1);
        vector<int> y = multiply_mod(a, b, mod2, root2);
        vector<int> z = multiply_mod(a, b, mod3, root3);

        vector<int> res(sz(x));
        for (int i = 0; i < sz(x); i++) res[i] = crt(x[i], y[i], z[i]);
        return res;
    }

    static vector<int> multiply(vector<int> a, vector<int> b, int limit) {
        vector<int> res = multiply(a, b);
        int final_sz = min((int)(a.size() + b.size() - 1), limit);
        res.resize(final_sz);
        return res;
    }

    static vector<int> poly_pow(const vector<int> &a, int exp, int limit) {
        vector<int> result(1, 1);
        vector<int> base = a;
        while (exp > 0) {
            if (exp % 2 == 1) {
                result = multiply(result, base, limit);
            }
            base = multiply(base, base, limit);
            exp /= 2;
        }
        return result;
    }
};
