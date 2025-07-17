#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ld long double
#define int long long
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) (int)(x.size())
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;
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

    static vector<int> poly_pow_mod(const vector<int> &a, int exp) {
        int n = a.size();
        vector<int> result(n, 0);
        result[0] = 1; 
        vector<int> base = a;
        while (exp > 0) {
            if (exp % 2 == 1) {
                result = multiply(result, base);
            }
            base = multiply(base, base);
            exp /= 2;
        }
        return result;
    }
};  

int powmod (int a, int b, int p) {
    int res = 1;
    while (b)
        if (b & 1)
            res = (res * 1ll * a % p),  --b;
        else
            a = (a * 1ll * a % p),  b >>= 1;
    return res;
}

int generator (int p) {
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

void solve()
{
    // Example usage of NTT for polynomial multiplication
    vector<int> poly1 = {1, 2, 3}; // Represents 1 + 2x + 3x^2
    vector<int> poly2 = {4, 5};    // Represents 4 + 5x

    vector<int> result = NTT::multiply(poly1, poly2);

    cout << "Result of polynomial multiplication: ";
    for (int coeff : result)
        cout << coeff << " ";
    cout << nl;
    // The result should represent the polynomial 4 + 13x + 22x^2 + 15x^3
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    //cin >> tc;
    while (tc--)
    {
        solve();
    }
    return 0;
}