/*
 * Topic: FWHT
 * Description: Fast Walsh-Hadamard Transform (Iterative, Fast Modulo)
 */
#include "../../core.h"

struct FWHT {
    static const int OR = 0;
    static const int AND = 1;
    static const int XOR = 2;

    static int mod_pow(int base, long long exp) {
        int res = 1;
        base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = (1LL * res * base) % mod;
            base = (1LL * base * base) % mod;
            exp /= 2;
        }
        return res;
    }

    static inline void add(int &a, int b) {
        a += b;
        if (a >= mod) a -= mod;
    }

    static inline void sub(int &a, int b) {
        a -= b;
        if (a < 0) a += mod;
    }

    static void fwht(vector<int> &a, bool inv, int flag) {
        int n = a.size();
        for (int len = 1; 2 * len <= n; len <<= 1) {
            for (int i = 0; i < n; i += 2 * len) {
                for (int j = 0; j < len; j++) {
                    int u = a[i + j];
                    int v = a[i + len + j];

                    if (flag == OR) {
                        if (!inv)
                            add(a[i + len + j], u);
                        else
                            sub(a[i + len + j], u);
                    } else if (flag == AND) {
                        if (!inv)
                            add(a[i + j], v);
                        else
                            sub(a[i + j], v);
                    } else if (flag == XOR) {
                        a[i + j] = u;
                        add(a[i + j], v);
                        a[i + len + j] = u;
                        sub(a[i + len + j], v);
                    }
                }
            }
        }
        if (inv && flag == XOR) {
            int inv_n = mod_pow(n, mod - 2);
            for (int i = 0; i < n; i++) {
                a[i] = (1LL * a[i] * inv_n) % mod;
            }
        }
    }

    static vector<int> multiply(vector<int> a, vector<int> b, int flag = XOR) {
        int n = 1;
        while (n < max(sz(a), sz(b))) n <<= 1;
        a.resize(n, 0);
        b.resize(n, 0);

        fwht(a, false, flag);
        fwht(b, false, flag);
        for (int i = 0; i < n; i++) {
            a[i] = (1LL * a[i] * b[i]) % mod;
        }
        fwht(a, true, flag);
        return a;
    }

    static vector<int> poly_pow(vector<int> a, long long k, int flag = XOR) {
        int n = 1;
        while (n < sz(a)) n <<= 1;
        a.resize(n, 0);

        fwht(a, false, flag);
        for (int i = 0; i < n; i++) {
            a[i] = mod_pow(a[i], k);
        }
        fwht(a, true, flag);
        return a;
    }
};
