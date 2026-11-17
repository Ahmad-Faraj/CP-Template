#include <bits/stdc++.h>
using namespace std;
#define ll long long

// 0-based, half-open interval [l, r)
// 1 l r x y   change all elements in range from l to R has value x to y
// 2 idx       query the value at index idx
struct LazyBinTreeUniversal {
    using T = int;
    using TMod = array<uint8_t, 101>;
    size_t size;
    vector<T> t;
    vector<TMod> mod;
    size_t h;

    LazyBinTreeUniversal(size_t N) : size(N), t(2 * N), mod(N, modInit()), h(32 - __builtin_clz(N)) {}

    LazyBinTreeUniversal(const vector<T> &other)
        : size(other.size()), t(2 * other.size()), mod(other.size(), modInit()), h(32 - __builtin_clz(other.size())) {
        copy(other.begin(), other.end(), t.begin() + size);
    }

    void apply(size_t p, const TMod &up_mod) {
        if (p < size) {
            if (mod[p][0] == 0) {
                mod[p] = up_mod;
            } else {
                mod[p] = combineMod(mod[p], up_mod);
            }
        } else {
            t[p] = combineValue(t[p], up_mod);
        }
    }

    void push(size_t p) {
        for (size_t s = h; s > 0; s--) {
            size_t pos = p >> s;
            if (mod[pos][0] != 0) {
                apply(2 * pos, mod[pos]);
                apply(2 * pos + 1, mod[pos]);
                mod[pos] = modInit();
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

    T init() { return 0; }

    TMod modInit() {
        TMod result = {};
        iota(result.begin(), result.end(), 0);
        return result;
    }
};