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


// NTT for any MOD
// use any mod here
const int MOD = 1e9 + 7;

// Three moduli for CRT
const int mod1 = 167772161; // 2^25 * 5 + 1
const int mod2 = 469762049; // 2^26 * 7 + 1
const int mod3 = 754974721; // 2^24 * 45 + 1

const int root1 = 3;
const int root2 = 3;
const int root3 = 11;

// Modular inverse using Fermat
int power(int a, int b, int mod) {
    int res = 1;
    while(b) {
        if(b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

void ntt(vector<int> &a, int mod, int root, bool invert) {
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

vector<int> multiply_mod(vector<int> a, vector<int> b, int mod, int root) {
    int n = 1;
    while (n < sz(a) + sz(b) - 1) n <<= 1;
    a.resize(n); b.resize(n);

    ntt(a, mod, root, false);
    ntt(b, mod, root, false);
    for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % mod;
    ntt(a, mod, root, true);

    return a;
}

int mod_inv(int a, int m) {
    return power(a, m - 2, m);
}

int crt(int r1, int r2, int r3) {
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

vector<int> multiply(vector<int> a, vector<int> b) {
    vector<int> x = multiply_mod(a, b, mod1, root1);
    vector<int> y = multiply_mod(a, b, mod2, root2);
    vector<int> z = multiply_mod(a, b, mod3, root3);

    vector<int> res(sz(x));
    for (int i = 0; i < sz(x); i++)
        res[i] = crt(x[i], y[i], z[i]);
    return res;
}

void solve()
{
    int n, m;
    cin >> n >> m;
    vector<int> a(n), b(m);
    for (int i = 0; i < n; i++)
        cin >> a[i];
    for (int i = 0; i < m; i++)
        cin >> b[i];
    vector<int> result = multiply(a, b);
    for (int i = 0; i < n + m - 1; i++)
    {
        if (i > 0) cout << sp;
        cout << result[i];
    }
    cout << nl;
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