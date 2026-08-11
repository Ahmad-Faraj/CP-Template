/*
 * Topic: NTT With Limit
 * Description: Number Theoretic Transform for modular polynomial multiplication with size limits
 */
#include "../../core.h"

struct NTT_with_limit {
    static const int MOD = 998244353;
    static const int ROOT = 3;  // Primitive root of unity for MOD

    static int mod_pow(int base, int exp, int mod) {
        int result = 1;
        while (exp > 0) {
            if (exp % 2 == 1) result = (result * base) % mod;
            base = (base * base) % mod;
            exp /= 2;
        }
        return result;
    }

    static void ntt(vector<int>& a, bool invert) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }

        for (int len = 2; len <= n; len <<= 1) {
            int wlen = mod_pow(ROOT, (MOD - 1) / len, MOD);
            if (invert) wlen = mod_pow(wlen, MOD - 2, MOD);
            for (int i = 0; i < n; i += len) {
                int w = 1;
                for (int j = 0; j < len / 2; j++) {
                    int u = a[i + j];
                    int v = (int)(a[i + j + len / 2] * w % MOD);
                    a[i + j] = u + v < MOD ? u + v : u + v - MOD;
                    a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + MOD;
                    w = (int)(w * wlen % MOD);
                }
            }
        }
        if (invert) {
            int n_inv = mod_pow(n, MOD - 2, MOD);
            for (int& x : a) x = (int)(x * n_inv % MOD);
        }
    }

    static vector<int> multiply(const vector<int>& a, const vector<int>& b, int li) {
        vector<int> fa(a.begin(), a.end()), fb(b.begin(), b.end());

        if (fa.size() > li) fa.resize(li);
        if (fb.size() > li) fb.resize(li);
        int n = 1;
        while (n < (int)fa.size() + (int)fb.size()) n <<= 1;
        fa.resize(n);
        fb.resize(n);
        ntt(fa, false);
        ntt(fb, false);
        for (int i = 0; i < n; i++) fa[i] = (int)(fa[i] * fb[i] % MOD);
        ntt(fa, true);
        fa.resize(min(li, sz(a) + sz(b) - 1));
        return fa;
    }

    static vector<int> poly_pow_mod(const vector<int>& a, int exp, int li) {
        vector<int> result = {1};
        vector<int> base = a;
        while (exp > 0) {
            if (exp % 2 == 1) { result = multiply(result, base, li); }
            if (exp > 1) { base = multiply(base, base, li); }
            exp /= 2;
        }
        return result;
    }
};
