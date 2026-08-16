#include "../../core.h"

struct KMP {
    string pattern;
    vector<int> lp; // lp[i] stores the length of the longest proper prefix of pattern[0..i] which is also a suffix of pattern[0..i]

    KMP(const string &str = "") : pattern(str) { build(); }

    void build() {
        int n = pattern.size();
        lp.assign(n, 0);
        for (int i = 1; i < n; i++) {
            lp[i] = failure(lp[i - 1], pattern[i]);
        }
    }

    int failure(int idx, char nxt) {
        while (idx > 0 && pattern[idx] != nxt) {
            idx = lp[idx - 1];
        }
        return idx + (pattern[idx] == nxt);
    }
};