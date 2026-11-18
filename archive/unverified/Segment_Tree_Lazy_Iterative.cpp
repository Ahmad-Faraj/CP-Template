// Lazy Segment Tree (iterative, value map): apply a map f: value -> value to a range, read a point.
// Use when: "on [l, r] change every x into y" (CF 911G): small value range, range map, point query.
// Usage: LazyBinTreeUniversal st(a); auto f = st.modInit(); f[x] = y; st.modify(l, r + 1, f);
//        st.query(i);
// Handles: any maps (non-commutative: later modify() calls apply after earlier ones), n = 1.
// Not: range aggregate queries - only point reads.
// Time: modify O(101 log n) | query O(101 log n)
// Indexing: 0-based positions, modify() takes a half-open range [l, r)
// Note: values must stay in [0, 100] (TMod size); enlarge TMod for a bigger value range. n >= 1.

#include <bits/stdc++.h>
using namespace std;

struct LazyBinTreeUniversal {
    using T = int;
    using TMod = array<uint8_t, 101>; // f[v] = new value of v
    size_t size;
    vector<T> t;
    vector<TMod> mod;
    size_t h;

    LazyBinTreeUniversal(size_t N)
        : size(N), t(2 * N), mod(N, modInit()), h(32 - __builtin_clz(N)) {}

    LazyBinTreeUniversal(const vector<T> &other)
        : size(other.size()), t(2 * other.size()), mod(other.size(), modInit()),
          h(32 - __builtin_clz(other.size())) {
        copy(other.begin(), other.end(), t.begin() + size);
    }

    void apply(size_t p, const TMod &up_mod) {
        if (p < size)
            mod[p] = combineMod(mod[p], up_mod);
        else
            t[p] = combineValue(t[p], up_mod);
    }

    void push(size_t p) {
        for (size_t s = h; s > 0; s--) {
            size_t pos = p >> s;
            if (mod[pos] != modInit()) {
                apply(2 * pos, mod[pos]);
                apply(2 * pos + 1, mod[pos]);
                mod[pos] = modInit();
            }
        }
    }

    void modify(size_t l, size_t r, const TMod &value) { // a[i] = value[a[i]] for i in [l, r)
        if (l == r) return;
        l += size;
        r += size;
        push(l);
        push(r - 1);
        while (l < r) {
            if (l & 1) {
                apply(l, value);
                l++;
            }
            if (r & 1) {
                r--;
                apply(r, value);
            }
            l /= 2;
            r /= 2;
        }
    }

    T query(size_t p) { // a[p]
        p += size;
        push(p);
        return t[p];
    }

    TMod modInit() { // the identity map
        TMod result = {};
        iota(result.begin(), result.end(), 0);
        return result;
    }

  private:
    TMod combineMod(const TMod &mod, const TMod &up_mod) { // mod first, then up_mod
        TMod result = {};
        for (size_t i = 0; i < result.size(); i++) result[i] = up_mod[mod[i]];
        return result;
    }

    T combineValue(T value, const TMod &up_mod) { return up_mod[value]; }
};
