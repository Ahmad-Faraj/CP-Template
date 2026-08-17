// FFT: multiplies two polynomials (or convolves two sequences) with no modulus, using complex roots of unity.
// Use when: "how many pairs sum to k", polynomial or big-number multiplication, string matching with wildcards.
// Handles: any lengths, empty inputs, negative coefficients, and real coefficients as well as integer ones.
// Time: O(n log n)
// Indexing: 0-based; result[k] is the sum of a[i] * b[j] over i + j == k, of length a.size() + b.size() - 1
// Note: doubles carry ~53 bits, so integer answers stay exact only up to about 1e15. Past that use NTT.cpp.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

using cd = complex<double>;
const double PI = acos(-1.0);

void fft(vector<cd> &a, bool invert) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; i++) { // bit-reversal permutation, done in place
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        double angle = 2 * PI / len * (invert ? -1 : 1);
        cd step(cos(angle), sin(angle));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= step;
            }
        }
    }
    if (invert)
        for (cd &x : a) x /= n;
}

vector<ll> multiply(const vector<ll> &a, const vector<ll> &b) { // exact while |result| stays under ~1e15
    if (a.empty() || b.empty()) return {};
    int need = (int)(a.size() + b.size() - 1), n = 1;
    while (n < need) n <<= 1;
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    fa.resize(n), fb.resize(n);
    fft(fa, false), fft(fb, false);
    for (int i = 0; i < n; i++) fa[i] *= fb[i];
    fft(fa, true);
    vector<ll> res(need);
    for (int i = 0; i < need; i++) res[i] = llround(fa[i].real());
    return res;
}

vector<double> multiply_real(const vector<double> &a, const vector<double> &b) { // no rounding
    if (a.empty() || b.empty()) return {};
    int need = (int)(a.size() + b.size() - 1), n = 1;
    while (n < need) n <<= 1;
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    fa.resize(n), fb.resize(n);
    fft(fa, false), fft(fb, false);
    for (int i = 0; i < n; i++) fa[i] *= fb[i];
    fft(fa, true);
    vector<double> res(need);
    for (int i = 0; i < need; i++) res[i] = fa[i].real();
    return res;
}

// the largest error seen against the rounded value; check it is well below 0.5 to trust the result
double rounding_margin(const vector<ll> &a, const vector<ll> &b) {
    if (a.empty() || b.empty()) return 0;
    int need = (int)(a.size() + b.size() - 1), n = 1;
    while (n < need) n <<= 1;
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    fa.resize(n), fb.resize(n);
    fft(fa, false), fft(fb, false);
    for (int i = 0; i < n; i++) fa[i] *= fb[i];
    fft(fa, true);
    double worst = 0;
    for (int i = 0; i < need; i++) worst = max(worst, fabs(fa[i].real() - llround(fa[i].real())));
    return worst;
}

// Standard problem: two polynomials by their coefficient lists - print the coefficients of their product
void solve() {
    int n, m;
    cin >> n >> m;
    vector<ll> a(n), b(m);
    for (ll &x : a) cin >> x;
    for (ll &x : b) cin >> x;
    vector<ll> c = multiply(a, b);
    for (size_t i = 0; i < c.size(); i++) cout << c[i] << " \n"[i + 1 == c.size()];
}
