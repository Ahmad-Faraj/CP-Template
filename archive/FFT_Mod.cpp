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
#define PI atan2l(0, -1) // acos(-1)
using namespace __gnu_pbds;
using namespace std;
// FFT
// Fast Fourier Transform for polynomial multiplication without mod
// O(n log n) complexity

#define complex complex<long double>
vector<vector<complex>> w;

void precompute_w(int lg)
{
    w.resize(lg + 1);
    for (int lvl = 1; lvl <= lg; lvl++)
    {
        int len = 1 << lvl;
        w[lvl].resize(len / 2);
        long double ang = 2 * PI / len;
        for (int j = 0; j < len / 2; j++)
        {
            w[lvl][j] = complex(cosl(ang * j), sinl(ang * j));
        }
    }
}
void fft(vector<complex> &a, bool invert)
{
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++)
    {
        int bit = n >> 1;
        for (; j >= bit; bit >>= 1)
            j -= bit;
        j += bit;
        if (i < j)
            swap(a[i], a[j]);
    }
    for (int len = 2, lvl = 1; len <= n; len <<= 1, lvl++)
    {
        for (int i = 0; i < n; i += len)
        {
            for (int j = 0; j < len / 2; j++)
            {
                complex wj = w[lvl][j];
                complex u = a[i + j];
                complex v = a[i + j + len / 2] * wj;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
            }
        }
    }
    if (invert)
    {
        reverse(a.begin() + 1, a.end());
        for (auto &x : a)
            x /= n;
    }
}

// =====================================================================================

const int MOD = 1e9 + 7, C = sqrt(MOD); // C is the base for splitting numbers = sqrt(max value)
vector<int> multiply(const vector<int> &a, const vector<int> &b)
{
    int n = 1, lg = 0;
    while (n < sz(a) + sz(b) - 1)
        n <<= 1, lg++;
    precompute_w(lg);
    vector<complex> A1(n), A2(n), B1(n), B2(n);
    for (int i = 0; i < sz(a); i++)
        A1[i] = a[i] % C, A2[i] = a[i] / C;
    for (int i = 0; i < sz(b); i++)
        B1[i] = b[i] % C, B2[i] = b[i] / C;
    fft(A1, false);
    fft(A2, false);
    fft(B1, false);
    fft(B2, false);
    vector<complex> R1(n), R2(n), R3(n);
    for (int i = 0; i < n; i++)
    {
        R1[i] = A1[i] * B1[i];
        R2[i] = A2[i] * B2[i];
        R3[i] = (A1[i] * B2[i] + A2[i] * B1[i]);
    }
    fft(R1, true);
    fft(R2, true);
    fft(R3, true);
    vector<int> result(n);
    for (int i = 0; i < n; i++)
    {
        int val = llround(R1[i].real());
        result[i] = add_mod(result[i], val, MOD);
        val = llround(R2[i].real());
        result[i] = add_mod(result[i], mult_mod(val, mult_mod(C, C, MOD), MOD), MOD);
        val = llround(R3[i].real());
        result[i] = add_mod(result[i], mult_mod(val, C, MOD), MOD);
    }
    return result;
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
    vector<int> res = multiply(a, b);

    for (int i = 0; i < n + m - 1; i++)
    {
        cout << res[i] << sp;
    }
    cout << nl;
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    // cin >> tc;
    while (tc--)
    {
        solve();
    }
    return 0;
}

// =====================================================================================
const int MOD = 1e9 + 7, C = sqrt(MOD); // C is the base for splitting numbers = sqrt(max value)
vector<int> multiply(const vector<int> &a, const vector<int> &b)
{
    int n = 1, lg = 0;
    while (n < sz(a) + sz(b) - 1)
        n <<= 1, lg++;
    precompute_w(lg);
    vector<complex> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    fa.resize(n);
    fb.resize(n);
    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];
    fft(fa, true);
    vector<int> result(n);
    for (int i = 0; i < n; i++)
        result[i] = llround(fa[i].real());

    return result;
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
    vector<int> A1(n), A2(n), B1(m), B2(m);
    for (int i = 0; i < n; i++)
    {
        A1[i] = a[i] % C;
        A2[i] = a[i] / C;
    }
    for (int i = 0; i < m; i++)
    {
        B1[i] = b[i] % C;
        B2[i] = b[i] / C;
    }
    // res = A1 * B1 + A2 * B2 * mxv^2 + (A1 * B2 + A2 * B1) * mxv
    vector<int> res1 = multiply(A1, B1);
    vector<int> res2 = multiply(A2, B2);
    vector<int> res3 = multiply(A1, B2);
    vector<int> res4 = multiply(A2, B1);
    int sz = n + m;
    vector<int> res(sz);
    for (int i = 0; i < sz; i++)
        res[i] = add_mod(res[i], res1[i], MOD);
    for (int i = 0; i < sz; i++)
        res[i] = add_mod(res[i], mult_mod(res2[i], mult_mod(C, C, MOD), MOD), MOD);
    for (int i = 0; i < sz; i++)
        res[i] = add_mod(res[i], mult_mod(res3[i], C, MOD), MOD);
    for (int i = 0; i < sz; i++)
        res[i] = add_mod(res[i], mult_mod(res4[i], C, MOD), MOD);

    for (int i = 0; i < n + m - 1; i++)
    {
        cout << res[i] << sp;
    }
    cout << nl;
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    // cin >> tc;
    while (tc--)
    {
        solve();
    }
    return 0;
}