/*
 * Topic: FFT_With_MOD
 * Description: FFT combined with modulo arithmetic
 */
#include "../../core.h"

struct FFT_With_MOD {
    // FFT
    // Fast Fourier Transform for polynomial multiplication without mod
    // O(n log n) complexity

    using cd = complex<long double>;
    inline static vector<vector<cd>> w;

    static void precompute_w(int lg) {
        w.resize(lg + 1);
        for (int lvl = 1; lvl <= lg; lvl++) {
            int len = 1 << lvl;
            w[lvl].resize(len / 2);
            long double ang = 2 * PI / len;
            for (int j = 0; j < len / 2; j++) {
                w[lvl][j] = complex(cosl(ang * j), sinl(ang * j));
            }
        }
    }

    static void fft(vector<cd> &a, bool invert) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j >= bit; bit >>= 1) j -= bit;
            j += bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2, lvl = 1; len <= n; len <<= 1, lvl++) {
            for (int i = 0; i < n; i += len) {
                for (int j = 0; j < len / 2; j++) {
                    cd wj = w[lvl][j];
                    cd u = a[i + j];
                    cd v = a[i + j + len / 2] * wj;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                }
            }
        }
        if (invert) {
            reverse(a.begin() + 1, a.end());
            for (auto &x : a) x /= n;
        }
    }

    // =====================================================================================

    static const int MOD = 1e9 + 7, C = 31622; // C is the base for splitting numbers = sqrt(max value)

    static vector<int> multiply(const vector<int> &a, const vector<int> &b) {
        int n = 1, lg = 0;
        while (n < sz(a) + sz(b) - 1) n <<= 1, lg++;
        precompute_w(lg);
        vector<cd> A1(n), A2(n), B1(n), B2(n);
        for (int i = 0; i < sz(a); i++) A1[i] = a[i] % C, A2[i] = a[i] / C;
        for (int i = 0; i < sz(b); i++) B1[i] = b[i] % C, B2[i] = b[i] / C;
        fft(A1, false);
        fft(A2, false);
        fft(B1, false);
        fft(B2, false);
        vector<cd> R1(n), R2(n), R3(n);
        for (int i = 0; i < n; i++) {
            R1[i] = A1[i] * B1[i];
            R2[i] = A2[i] * B2[i];
            R3[i] = (A1[i] * B2[i] + A2[i] * B1[i]);
        }
        fft(R1, true);
        fft(R2, true);
        fft(R3, true);
        vector<int> result(n);
        for (int i = 0; i < n; i++) {
            int val = llround(R1[i].real());
            result[i] = add_mod(result[i], val, MOD);
            val = llround(R2[i].real());
            result[i] = add_mod(result[i], mult_mod(val, mult_mod(C, C, MOD), MOD), MOD);
            val = llround(R3[i].real());
            result[i] = add_mod(result[i], mult_mod(val, C, MOD), MOD);
        }
        return result;
    }

    static vector<int> multiply(const vector<int> &a, const vector<int> &b, int limit) {
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
