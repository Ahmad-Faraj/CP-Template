/*
 * Topic: Prefix XOR Basis
 * Description: Linear basis supporting offline range [L, R] queries via prefix indexing
 */
#include "../../core.h"

template <int LOG = 62> struct XorBasisPrefix {
    static_assert(LOG >= 1 && LOG <= 63);
    int b[LOG];
    int pos[LOG];
    int last;

    XorBasisPrefix() {
        memset(b, 0, sizeof(b));
        memset(pos, 0, sizeof(pos));
        last = 0;
    }

    void insert(int v, int idx = 0) {
        if (idx == 0)
            idx = ++last;
        else
            last = max(last, idx);

        int p = idx;
        for (int i = LOG - 1; i >= 0; i--) {
            if (((v >> i) & 1) == 0) continue;
            if (!b[i]) {
                b[i] = v;
                pos[i] = p;
                return;
            }
            if (pos[i] < p) {
                swap(pos[i], p);
                swap(b[i], v);
            }
            v ^= b[i];
        }
    }

    int max_xor(int l, int start = 0) const {
        int ans = start;
        for (int i = LOG - 1; i >= 0; i--) {
            if (pos[i] >= l) ans = max(ans, ans ^ b[i]);
        }
        return ans;
    }

    int reduce(int x, int l) const {
        int ans = x;
        for (int i = LOG - 1; i >= 0; i--) {
            if (pos[i] >= l) ans = min(ans, ans ^ b[i]);
        }
        return ans;
    }

    bool contains(int x, int l) const { return reduce(x, l) == 0; }
};
