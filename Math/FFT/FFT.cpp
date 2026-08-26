/*
 * Topic: FFT
 * Description: Standard Fast Fourier Transform (Complex) [Compact]
 */
#include "../../core.h"

struct FFT {
    using cd = complex<long double>;

    static void fft(vector<cd> &a, bool inv) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            double ang = 2 * PI / len * (inv ? -1 : 1);
            cd wlen(cos(ang), sin(ang));
            for (int i = 0; i < n; i += len) {
                cd w(1);
                for (int j = 0; j < len / 2; j++, w *= wlen) {
                    cd u = a[i + j], v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                }
            }
        }
        if (inv) {
            for (cd &x : a) x /= n;
        }
    }

    static vector<int> multiply(const vector<int> &a, const vector<int> &b) {
        vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < sz(a) + sz(b)) n <<= 1;
        fa.resize(n);
        fb.resize(n);
        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; i++) fa[i] *= fb[i];
        fft(fa, true);
        vector<int> res(n);
        for (int i = 0; i < n; i++) res[i] = round(fa[i].real());
        return res;
    }

    // Function to compute the power of a polynomial using FFT
    //! use multiply with limit argument
    //! and clip the result to the limit before return in multiply
    static vector<int> multiply(const vector<int> &a, const vector<int> &b, int limit) {
        int n = 1;
        while (n < sz(a) + sz(b) - 1) n <<= 1;
        vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        fa.resize(n);
        fb.resize(n);
        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; i++) fa[i] *= fb[i];
        fft(fa, true);
        vector<int> result(min(n, limit));
        for (int i = 0; i < min(n, limit); i++) result[i] = round(fa[i].real());
        return result;
    }

    static vector<int> poly_pow(const vector<int> &a, int exp, int limit) {
        vector<int> result(1, 1);
        vector<int> base = a;

        while (exp) {
            if (exp & 1) result = multiply(result, base, limit);
            base = multiply(base, base, limit);
            exp >>= 1;
        }
        return result;
    }
};
