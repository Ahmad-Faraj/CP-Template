/*
 * Topic: XOR Basis
 * Description: Standard linear basis for maximum XOR subsets and GF(2) spaces
 */
#include "../../core.h"

template <int LOG = 62> struct XorBasis {
    static_assert(LOG >= 1 && LOG <= 63);
    int b[LOG];
    int rk;

    XorBasis() {
        memset(b, 0, sizeof(b));
        rk = 0;
    }

    bool insert(int x) {
        int v = x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (((v >> i) & 1) == 0) continue;
            if (!b[i]) {
                b[i] = v;
                rk++;
                return true;
            }
            v ^= b[i];
        }
        return false;
    }

    int max_xor(int start = 0) const {
        int ans = start;
        for (int i = LOG - 1; i >= 0; i--) {
            ans = max(ans, ans ^ b[i]);
        }
        return ans;
    }

    int reduce(int x) const {
        int ans = x;
        for (int i = LOG - 1; i >= 0; i--) {
            ans = min(ans, ans ^ b[i]);
        }
        return ans;
    }

    bool contains(int x) const { return reduce(x) == 0; }
};
