// Segment Tree Lazy Generic: lazy segment tree where you plug in the node, the tag and the ops.
// Use when: range update + range query and the ops are not plain add/sum (affine, assign,
//           composite, max subarray, ...); op may be non-commutative (left-to-right is kept).
// Usage: write one struct M { S, F, op, e, mapping, composition, id } then LazySeg<M> t(v); e.g.
//        struct M { // range affine x -> a*x+b, range sum (mod 998244353)
//            struct S { ll sum, len; };  struct F { ll a, b; };
//            static S op(S x, S y) { return {(x.sum + y.sum) % MOD, x.len + y.len}; }
//            static S e() { return {0, 0}; }
//            static S mapping(F f, S x) { return {(f.a * x.sum + f.b * x.len) % MOD, x.len}; }
//            static F composition(F f, F g) { return {f.a * g.a % MOD, (f.a * g.b + f.b) % MOD}; }
//            static F id() { return {1, 0}; } };   // leaves start as S{a[i], 1}
//        t.apply(l, r, F{a, b}); t.prod(l, r).sum; t.set(p, x); t.get(p); t.all_prod();
//        t.apply(p, f); t.max_right(l, g) / t.min_left(r, g) with g(e()) == true and g monotone.
// Handles: n = 0, empty ranges l == r, non-commutative op, point and range updates.
// Time: build O(n) | set, get, prod, apply, max_right, min_left O(log n) | all_prod O(1)
// Indexing: 0-based, ranges HALF-OPEN [l, r)
// Note: mapping(f, x) = f applied to x; composition(f, g) = "g first, then f". mapping must work
//       on a whole segment, so keep len (or whatever it needs) inside S.

#include <bits/stdc++.h>
using namespace std;

template <class M> struct LazySeg {
    using S = typename M::S;
    using F = typename M::F;
    int n, sz, lg;
    vector<S> d;
    vector<F> lz;

    LazySeg(int n_ = 0) : LazySeg(vector<S>(n_, M::e())) {}
    LazySeg(const vector<S> &v) : n(v.size()), lg(0) {
        while ((1 << lg) < n) lg++;
        sz = 1 << lg;
        d.assign(2 * sz, M::e()), lz.assign(sz, M::id());
        for (int i = 0; i < n; i++) d[sz + i] = v[i];
        for (int i = sz - 1; i >= 1; i--) pull(i);
    }
    void pull(int k) { d[k] = M::op(d[2 * k], d[2 * k + 1]); }
    void all_apply(int k, const F &f) {
        d[k] = M::mapping(f, d[k]);
        if (k < sz) lz[k] = M::composition(f, lz[k]);
    }
    void push(int k) { all_apply(2 * k, lz[k]), all_apply(2 * k + 1, lz[k]), lz[k] = M::id(); }
    void push_to(int p) { for (int i = lg; i >= 1; i--) push(p >> i); }

    void set(int p, const S &x) {
        push_to(p += sz), d[p] = x;
        for (int i = 1; i <= lg; i++) pull(p >> i);
    }
    S get(int p) { return push_to(p += sz), d[p]; }
    void apply(int p, const F &f) {
        push_to(p += sz), d[p] = M::mapping(f, d[p]);
        for (int i = 1; i <= lg; i++) pull(p >> i);
    }
    void push_range(int l, int r) {
        for (int i = lg; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }
    }
    S prod(int l, int r) {
        if (l == r) return M::e();
        push_range(l += sz, r += sz);
        S sl = M::e(), sr = M::e();
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) sl = M::op(sl, d[l++]);
            if (r & 1) sr = M::op(d[--r], sr);
        }
        return M::op(sl, sr);
    }
    S all_prod() { return d[1]; }
    void apply(int l, int r, const F &f) {
        if (l == r) return;
        push_range(l += sz, r += sz);
        for (int a = l, b = r; a < b; a >>= 1, b >>= 1) {
            if (a & 1) all_apply(a++, f);
            if (b & 1) all_apply(--b, f);
        }
        for (int i = 1; i <= lg; i++) {
            if (((l >> i) << i) != l) pull(l >> i);
            if (((r >> i) << i) != r) pull((r - 1) >> i);
        }
    }
    // largest r with g(prod(l, r)) true
    template <class G> int max_right(int l, G g) {
        if (l == n) return n;
        push_to(l += sz);
        S sm = M::e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(M::op(sm, d[l]))) {
                while (l < sz) {
                    push(l), l = 2 * l;
                    if (g(M::op(sm, d[l]))) sm = M::op(sm, d[l++]);
                }
                return l - sz;
            }
            sm = M::op(sm, d[l++]);
        } while ((l & -l) != l);
        return n;
    }
    // smallest l with g(prod(l, r)) true
    template <class G> int min_left(int r, G g) {
        if (r == 0) return 0;
        r += sz;
        for (int i = lg; i >= 1; i--) push((r - 1) >> i);
        S sm = M::e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(M::op(d[r], sm))) {
                while (r < sz) {
                    push(r), r = 2 * r + 1;
                    if (g(M::op(d[r], sm))) sm = M::op(d[r--], sm);
                }
                return r + 1 - sz;
            }
            sm = M::op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
};
