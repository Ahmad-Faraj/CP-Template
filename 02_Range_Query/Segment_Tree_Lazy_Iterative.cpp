// Lazy Segment Tree Iterative: applies a value map f: {0..100} -> {0..100} to a range; point query.
// Use when: "in a[l..r) turn every x into y" many times (CF 911G), values small, maps compose.
// Usage: LazyBinTreeUniversal t(a); auto m = t.modInit(); m[x] = y; t.modify(l, r, m); t.query(i);
// Time: modify O(log n * 101) | query O(log n * 101)
// Indexing: 0-based, half-open [l, r)
// Not: a general lazy segment tree (no range sum); for that use Segment_Tree_Lazy_Generic.cpp.
// Note: n >= 1 and values in [0, 100]. Any map is allowed; later maps apply after earlier ones.

#include <bits/stdc++.h>
using namespace std;
struct LazyBinTreeUniversal {
    using T = int;
    using TMod = array<uint8_t, 101>;
    size_t size;
    vector<T> t;
    vector<TMod> mod;
    size_t h;
    TMod ident = modInit();

    LazyBinTreeUniversal(size_t N)
        : size(N), t(2 * N), mod(N, modInit()), h(32 - __builtin_clz(N)) {}

    LazyBinTreeUniversal(const vector<T> &other)
        : size(other.size()), t(2 * other.size()), mod(other.size(), modInit()),
          h(32 - __builtin_clz(other.size())) {
        copy(other.begin(), other.end(), t.begin() + size);
    }

    void apply(size_t p, const TMod &up_mod) {
        if (p < size) {
            mod[p] = combineMod(mod[p], up_mod);
        } else {
            t[p] = combineValue(t[p], up_mod);
        }
    }

    void push(size_t p) {
        for (size_t s = h; s > 0; s--) {
            size_t pos = p >> s;
            if (mod[pos] != ident) {
                apply(2 * pos, mod[pos]);
                apply(2 * pos + 1, mod[pos]);
                mod[pos] = ident;
            }
        }
    }

    void modify(size_t l, size_t r, const TMod &value) {
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

    T query(size_t p) {
        p += size;
        push(p);
        return t[p];
    }

  private:
    TMod combineMod(const TMod &mod, const TMod &up_mod) {
        TMod result = {};
        for (size_t i = 0; i < result.size(); i++) {
            result[i] = up_mod[mod[i]];
        }
        return result;
    }

    T combineValue(T value, const TMod &up_mod) { return up_mod[value]; }

  public:
    TMod modInit() { // identity map; set m[x] = y to build "x -> y"
        TMod result = {};
        iota(result.begin(), result.end(), 0);
        return result;
    }
};