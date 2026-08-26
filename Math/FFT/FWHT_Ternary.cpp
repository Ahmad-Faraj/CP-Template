/*
 * Topic: FWHT Ternary
 * Description: Fast Walsh-Hadamard Transform for base 3 / ternary operations
 * Reference: https://www.codechef.com/problems/MDSWIN
 */
#include "../../core.h"

struct FWHT_Ternary {
    static const int BIT = 11;
    static const int M = 177147; // 3^BIT

    static inline void add(int &a, int b) {
        a += b;
        if (a >= mod) a -= mod;
    }

    static inline void sub(int &a, int b) {
        a -= b;
        if (a < 0) a += mod;
    }

    static inline int mul(int a, int b) { return (1LL * a * b) % mod; }

    struct cmplx {
        int a, b;

        cmplx(int _a = 0, int _b = 0) : a(_a), b(_b) {
            if (a >= mod) a %= mod;
            if (b >= mod) b %= mod;
            if (a < 0) a = (a % mod + mod) % mod;
            if (b < 0) b = (b % mod + mod) % mod;
        }

        cmplx operator+(const cmplx &x) const {
            cmplx res = *this;
            add(res.a, x.a);
            add(res.b, x.b);
            return res;
        }

        cmplx operator*(int k) const { return cmplx(mul(a, k), mul(b, k)); }

        cmplx operator*(const cmplx &x) const {
            int na = mul(a, x.a);
            sub(na, mul(b, x.b));

            int nb = mul(a, x.b);
            add(nb, mul(b, x.a));
            sub(nb, mul(b, x.b));

            return cmplx(na, nb);
        }
    };

    static cmplx w() { return cmplx(0, 1); }

    static cmplx w2() { return cmplx(mod - 1, mod - 1); }

    static void fwht(vector<cmplx> &vals, bool invert = false) {
        int n = vals.size();
        for (int len = 1; len < n; len *= 3) {
            int pitch = len * 3;
            int len2 = len * 2;
            for (int i = 0; i < n; i += pitch) {
                for (int j = 0; j < len; j++) {
                    cmplx a = vals[i + j];
                    cmplx b = vals[i + j + len];
                    cmplx c = vals[i + j + len2];

                    vals[i + j] = a + b + c;
                    vals[i + j + len] = a + (b * w()) + (c * w2());
                    vals[i + j + len2] = a + (b * w2()) + (c * w());
                    if (invert) swap(vals[i + j + len], vals[i + j + len2]);
                }
            }
        }

        if (invert) {
            int inv3 = (mod + 1) / 3;
            int inv = 1;
            for (int i = 1; i < n; i *= 3) inv = mul(inv, inv3);
            for (int i = 0; i < n; i++) vals[i] = vals[i] * inv;
        }
    }

    static cmplx power(cmplx x, long long n) {
        cmplx ret(1);
        while (n) {
            if (n & 1) ret = ret * x;
            x = x * x;
            n >>= 1;
        }
        return ret;
    }

    static int tobase3(int x) {
        int ret = 0;
        for (int i = BIT - 1; i >= 0; i--) {
            ret *= 3;
            if (x & (1 << i)) ret++;
        }
        return ret;
    }

    static vector<cmplx> convert(int n, const vector<int> &a) {
        vector<cmplx> p(M);
        for (int i = 0; i < n; i++) {
            int x = tobase3(i);
            add(p[x].a, a[i] % mod);
        }
        return p;
    }

    static vector<int> multiply(int n, const vector<int> &a, const vector<int> &b) {
        vector<cmplx> p = convert(n, a);
        vector<cmplx> q = convert(n, b);
        fwht(p);
        fwht(q);
        for (int i = 0; i < M; i++) p[i] = p[i] * q[i];
        fwht(p, true);

        vector<int> ans(M);
        for (int i = 0; i < M; i++) ans[i] = p[i].a;
        return ans;
    }

    static vector<int> poly_pow(int n, const vector<int> &a, long long k) {
        vector<cmplx> p = convert(n, a);
        fwht(p);
        for (int i = 0; i < M; i++) p[i] = power(p[i], k);
        fwht(p, true);

        vector<int> ans(M);
        for (int i = 0; i < M; i++) ans[i] = p[i].a;
        return ans;
    }
};
