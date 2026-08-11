/*
 * Topic: NTT
 * Description: Number Theoretic Transform (mod 998244353)
 */
#include "../../core.h"
// NTT
// NTT is FFT but for modular calculations
// O(n log n) complexity
struct NTT
{
    static const int MOD = 998244353;
    static const int ROOT = 3; // Primitive root of unity for MOD

    static int mod_pow(int base, int exp, int mod)
    {
        int result = 1;
        while (exp > 0)
        {
            if (exp % 2 == 1)
                result = (result * base) % mod;
            base = (base * base) % mod;
            exp /= 2;
        }
        return result;
    }

    static void ntt(vector<int> &a, bool invert)
    {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++)
        {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1)
                j ^= bit;
            j ^= bit;
            if (i < j)
                swap(a[i], a[j]);
        }

        for (int len = 2; len <= n; len <<= 1)
        {
            int wlen = mod_pow(ROOT, (MOD - 1) / len, MOD);
            if (invert)
                wlen = mod_pow(wlen, MOD - 2, MOD);
            for (int i = 0; i < n; i += len)
            {
                int w = 1;
                for (int j = 0; j < len / 2; j++)
                {
                    int u = a[i + j];
                    int v = (int)(a[i + j + len / 2] * w % MOD);
                    a[i + j] = u + v < MOD ? u + v : u + v - MOD;
                    a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + MOD;
                    w = (int)(w * wlen % MOD);
                }
            }
        }
        if (invert)
        {
            int n_inv = mod_pow(n, MOD - 2, MOD);
            for (int &x : a)
                x = (int)(x * n_inv % MOD);
        }
    }

    static vector<int> multiply(const vector<int> &a, const vector<int> &b)
    {
        vector<int> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < (int)fa.size() + (int)fb.size())
            n <<= 1;
        fa.resize(n);
        fb.resize(n);
        ntt(fa, false);
        ntt(fb, false);
        for (int i = 0; i < n; i++)
            fa[i] = (int)(fa[i] * fb[i] % MOD);
        ntt(fa, true);
        fa.resize(sz(a) + sz(b) - 1);
        return fa;
    }
    static vector<int> multiply(const vector<int> &a, const vector<int> &b, int limit)
    {
        vector<int> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < (int)fa.size() + (int)fb.size())
            n <<= 1;
        fa.resize(n);
        fb.resize(n);
        ntt(fa, false);
        ntt(fb, false);
        for (int i = 0; i < n; i++)
            fa[i] = (int)((1LL * fa[i] * fb[i]) % MOD);
        ntt(fa, true);
        
        int final_sz = min((int)(a.size() + b.size() - 1), limit);
        fa.resize(final_sz);
        return fa;
    }


    static vector<int> poly_pow_mod(const vector<int> &a, int exp, int limit) {
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

    static int powmod (int a, int b, int p) {
        int res = 1;
        while (b)
            if (b & 1)
                res = (res * 1ll * a % p),  --b;
            else
                a = (a * 1ll * a % p),  b >>= 1;
        return res;
    }

    static int generator (int p) {
        vector<int> fact;
        int phi = p-1,  n = phi;
        for (int i=2; i*i<=n; ++i)
            if (n % i == 0) {
                fact.push_back (i);
                while (n % i == 0)
                    n /= i;
            }
        if (n > 1)
            fact.push_back (n);

        for (int res=2; res<=p; ++res) {
            bool ok = true;
            for (size_t i=0; i<fact.size() && ok; ++i)
                ok &= powmod (res, phi / fact[i], p) != 1;
            if (ok)  return res;
        }
        return -1;
    }
};
