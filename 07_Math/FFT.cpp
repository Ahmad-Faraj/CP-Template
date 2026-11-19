// FFT: exact product of two integer polynomials (no modulus).
// Use when: "number of pairs with sum k", convolution of counts, big-number multiplication.
// Usage: vector<ll> c = multiply(a, b); | poly_pow(a, e) | mul_big_int("123", "456") == "56088"
// Handles: negative coefficients, empty input (returns {}), leading zeros in mul_big_int input.
// Time: O(n log n), n = padded size >= |a| + |b| - 1. long double is slow: 2^20 padded ~1.4 s
// Indexing: 0-based; c[k] = sum a[i] * b[j] over i + j == k, |c| = |a| + |b| - 1
// Note: exact while the TRUE |c[k]| <= ~1e17 (2^19 x 2^19 terms of 1e6 is fine).
//       Mod answers: NTT / NTT_Any_Mod. poly_pow keeps every term: (|a| - 1) * e + 1.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

using cplx = complex<long double>;

void fft(vector<cplx> &a, bool invert) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j >= bit; bit >>= 1) j -= bit;
        j += bit;
        if (i < j) swap(a[i], a[j]);
    }
    const long double PI = acosl(-1);
    vector<cplx> w(n / 2 + 1);
    for (int len = 2; len <= n; len <<= 1) {
        long double ang = 2 * PI / len;
        for (int j = 0; j < len / 2; j++) w[j] = cplx(cosl(ang * j), sinl(ang * j));
        for (int i = 0; i < n; i += len)
            for (int j = 0; j < len / 2; j++) {
                cplx u = a[i + j], v = a[i + j + len / 2] * w[j];
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
            }
    }
    if (invert) {
        reverse(a.begin() + 1, a.end());
        for (auto &x : a) x /= n;
    }
}

vector<ll> multiply(const vector<ll> &a, const vector<ll> &b) {
    if (a.empty() || b.empty()) return {};
    int need = a.size() + b.size() - 1, n = 1;
    while (n < need) n <<= 1;
    vector<cplx> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    fa.resize(n), fb.resize(n);
    fft(fa, false), fft(fb, false);
    for (int i = 0; i < n; i++) fa[i] *= fb[i];
    fft(fa, true);
    vector<ll> result(need);
    for (int i = 0; i < need; i++) result[i] = llroundl(fa[i].real());
    return result;
}

vector<ll> poly_pow(const vector<ll> &a, ll e) { // a^e, every term kept
    vector<ll> result{1}, base = a;
    while (e) {
        if (e & 1) result = multiply(result, base);
        e >>= 1;
        if (e) base = multiply(base, base);
    }
    return result;
}

string mul_big_int(const string &a, const string &b) { // non-negative decimal strings
    int n = a.size(), m = b.size();
    vector<ll> num1(n), num2(m);
    for (int i = 0; i < n; i++) num1[i] = a[n - 1 - i] - '0';
    for (int i = 0; i < m; i++) num2[i] = b[m - 1 - i] - '0';
    vector<ll> result = multiply(num1, num2);
    string res;
    ll carry = 0;
    for (ll val : result) {
        val += carry;
        carry = val / 10;
        res.push_back(val % 10 + '0');
    }
    while (carry) res.push_back(carry % 10 + '0'), carry /= 10;
    while (res.size() > 1 && res.back() == '0') res.pop_back();
    reverse(res.begin(), res.end());
    return res.empty() ? "0" : res;
}
