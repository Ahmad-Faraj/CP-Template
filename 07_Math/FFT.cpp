#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ld long double
#define int long long
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
// FFT
// Fast Fourier Transform for polynomial multiplication without mod
// O(n log n) complexity
// if wrong try use precompute_w with fft function from FFT_With_MOD.cpp
#define complex complex<long double>
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
    for (int len = 2; len <= n; len <<= 1)
    {
        double angle = 2 * PI / len * (invert ? -1 : 1);
        complex wlen(cos(angle), sin(angle));
        for (int i = 0; i < n; i += len)
        {
            complex w(1);
            for (int j = 0; j < len / 2; j++)
            {
                complex u = a[i + j];
                complex v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert)
    {
        for (auto &x : a)
            x /= n;
    }
}
vector<int> multiply(const vector<int> &a, const vector<int> &b)
{
    int n = 1;
    while (n < sz(a) + sz(b) - 1)
        n <<= 1;
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
// Function to compute the power of a polynomial using FFT
//! use multiply with limit argument
//! and clip the result to the limit before return in multiply
/*
    vector<int> multiply(const vector<int> &a, const vector<int> &b, int limit = 1e5)
    {
        int n = 1;
        while (n < sz(a) + sz(b) - 1)
            n <<= 1;
        vector<complex> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        fa.resize(n);
        fb.resize(n);
        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; i++)
            fa[i] *= fb[i];
        fft(fa, true);
        vector<int> result(min(n, limit));
        for (int i = 0; i < min(n, limit); i++)
            result[i] = round(fa[i].real());
        return result;
    }
*/
static vector<int> poly_pow(const vector<int> &a, int exp)
{
    int n = a.size();
    vector<int> result(n, 0);
    result[0] = 1;
    vector<int> base = a;

    while (exp)
    {
        if (exp & 1)
            result = multiply(result, base);
        base = multiply(base, base);
        exp >>= 1;
    }
    return result;
}
// Function to multiply two large integers represented as strings in O(n log n) time using FFT
string mul_big_int(const string &a, const string &b)
{
    int n = a.size(), m = b.size();
    vector<int> num1(n), num2(m);
    for (int i = 0; i < n; i++)
    {
        num1[i] = a[n - 1 - i] - '0';
    }
    for (int i = 0; i < m; i++)
    {
        num2[i] = b[m - 1 - i] - '0';
    }
    vector<int> result = multiply(num1, num2);
    string res;
    int carry = 0;
    for (auto &val : result)
    {
        val += carry;
        carry = val / 10;
        res.push_back((val % 10) + '0');
    }
    while (carry)
    {
        res.push_back((carry % 10) + '0');
        carry /= 10;
    }
    reverse(res.begin(), res.end());
    for (int i = 0; i < sz(res); i++){
        if(res[i] != '0'){
            return res.substr(i);
        }
    }
    return "0";
}

void solve()
{
    // Example usage of FFT for polynomial multiplication
    vector<int> poly1 = {1, 2, 3}; // Represents 1 + 2x + 3x^2
    vector<int> poly2 = {4, 5};    // Represents 4 + 5x
    vector<int> result = multiply(poly1, poly2);

    cout << "Result of polynomial multiplication: ";
    for (auto &val : result)
        cout << val << sp; // Output the real part of the complex numbers
    cout << endl;
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