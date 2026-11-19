// Test for 02_Range_Query/Segment_Tree_Lazy_Generic.cpp against a plain array with naive loops.
// Three instantiations: affine + sum (mod), add + min, assign + affine composite (non-commutative).
// Every public method is checked: both constructors, set, get, prod, all_prod, apply (point and
// range), max_right, min_left.
#include "../../02_Range_Query/Segment_Tree_Lazy_Generic.cpp"
using ll = long long;

map<string, pair<long long, long long>> cov;
mt19937_64 rng(7);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }

const ll MD = 998244353;
struct AffSum {
    struct S { ll sum, len; };
    struct F { ll a, b; };
    static S op(S x, S y) { return {(x.sum + y.sum) % MD, x.len + y.len}; }
    static S e() { return {0, 0}; }
    static S mapping(F f, S x) { return {(f.a * x.sum + f.b * x.len) % MD, x.len}; }
    static F composition(F f, F g) { return {f.a * g.a % MD, (f.a * g.b + f.b) % MD}; }
    static F id() { return {1, 0}; }
};
struct AddMin {
    using S = ll;
    using F = ll;
    static S op(S x, S y) { return min(x, y); }
    static S e() { return LLONG_MAX; }
    static S mapping(F f, S x) { return x == LLONG_MAX ? x : x + f; }
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};
// S = linear function x -> a*x+b over a segment, op = composition (left part applied first);
// F = "assign every element to (a, b)": the composite of len copies is (a^len, b*(a^len-1)/(a-1)).
ll pw(ll b, ll e) {
    ll r = 1;
    for (b %= MD; e; e >>= 1, b = b * b % MD)
        if (e & 1) r = r * b % MD;
    return r;
}
struct SetComp {
    struct S { ll a, b, len; };       // composite of len functions
    struct F { bool on; ll a, b; };   // on = assign (a, b)
    static S op(S x, S y) { return {x.a * y.a % MD, (y.a * x.b + y.b) % MD, x.len + y.len}; }
    static S e() { return {1, 0, 0}; }
    static S mapping(F f, S x) {
        if (!f.on || x.len == 0) return x;
        ll A = pw(f.a, x.len), B; // b * (1 + a + ... + a^(len-1))
        if (f.a == 1) B = f.b * (x.len % MD) % MD;
        else B = f.b * ((A - 1 + MD) % MD) % MD * pw((f.a - 1 + MD) % MD, MD - 2) % MD;
        return {A, B, x.len};
    }
    static F composition(F f, F g) { return f.on ? f : g; }
    static F id() { return {false, 0, 0}; }
};

int main() {
    for (int it = 0; it < 3000; it++) {
        int n = rng() % 18;
        // ---- affine + sum ----
        {
            vector<ll> a(n);
            for (auto &x : a) x = rnd(0, MD - 1);
            vector<AffSum::S> v(n);
            for (int i = 0; i < n; i++) v[i] = {a[i], 1};
            LazySeg<AffSum> t(v);
            for (int q = 0; q < 60; q++) {
                int l = rnd(0, n), r = rnd(0, n);
                if (l > r) swap(l, r);
                int op = rng() % 6;
                if (op == 0) {
                    ll A = rnd(0, MD - 1), B = rnd(0, MD - 1);
                    t.apply(l, r, {A, B});
                    for (int i = l; i < r; i++) a[i] = (A * a[i] + B) % MD;
                    chk("aff:apply(l,r)", true);
                } else if (op == 1 && n) {
                    int p = rnd(0, n - 1);
                    ll x = rnd(0, MD - 1);
                    t.set(p, {x, 1}), a[p] = x;
                    chk("aff:set", true);
                } else if (op == 2 && n) {
                    int p = rnd(0, n - 1);
                    ll A = rnd(0, MD - 1), B = rnd(0, MD - 1);
                    t.apply(p, {A, B}), a[p] = (A * a[p] + B) % MD;
                    chk("aff:apply(p)", true);
                } else if (op == 3 && n) {
                    int p = rnd(0, n - 1);
                    auto s = t.get(p);
                    chk("aff:get", s.sum == a[p] && s.len == 1);
                } else {
                    ll s = 0;
                    for (int i = l; i < r; i++) s = (s + a[i]) % MD;
                    auto got = t.prod(l, r);
                    chk("aff:prod", got.sum == s && got.len == r - l);
                }
                ll all = 0;
                for (ll x : a) all = (all + x) % MD;
                chk("aff:all_prod", t.all_prod().sum == all && t.all_prod().len == n);
            }
        }
        // ---- add + min, with max_right / min_left ----
        {
            vector<ll> a(n, 0);
            LazySeg<AddMin> t(n); // n-constructor: all e()
            chk("min:ctor(n)", n == 0 || t.prod(0, n) == LLONG_MAX);
            for (int i = 0; i < n; i++) a[i] = rnd(-20, 20), t.set(i, a[i]);
            for (int q = 0; q < 60; q++) {
                int l = rnd(0, n), r = rnd(0, n);
                if (l > r) swap(l, r);
                int op = rng() % 6;
                if (op == 0) {
                    ll x = rnd(-10, 10);
                    t.apply(l, r, x);
                    for (int i = l; i < r; i++) a[i] += x;
                    chk("min:apply(l,r)", true);
                } else if (op == 1) {
                    ll s = LLONG_MAX;
                    for (int i = l; i < r; i++) s = min(s, a[i]);
                    chk("min:prod", t.prod(l, r) == s);
                } else if (op == 2 && n) {
                    int p = rnd(0, n - 1);
                    chk("min:get", t.get(p) == a[p]);
                } else {
                    ll k = rnd(-25, 25);
                    auto g = [&](ll s) { return s >= k; };
                    int want = l;
                    while (want < n && a[want] >= k) want++;
                    chk("min:max_right", t.max_right(l, g) == want);
                    int want2 = r;
                    while (want2 > 0 && a[want2 - 1] >= k) want2--;
                    chk("min:min_left", t.min_left(r, g) == want2);
                }
            }
            ll s = LLONG_MAX;
            for (ll x : a) s = min(s, x);
            chk("min:all_prod", t.all_prod() == s);
        }
        // ---- assign + composite (non-commutative op) ----
        {
            vector<ll> A(n), B(n);
            vector<SetComp::S> v(n);
            for (int i = 0; i < n; i++) A[i] = rnd(0, MD - 1), B[i] = rnd(0, MD - 1);
            if (it % 3 == 0) // small values so a == 1 shows up
                for (int i = 0; i < n; i++) A[i] = rnd(0, 2), B[i] = rnd(0, 3);
            for (int i = 0; i < n; i++) v[i] = {A[i], B[i], 1};
            LazySeg<SetComp> t(v);
            for (int q = 0; q < 60; q++) {
                int l = rnd(0, n), r = rnd(0, n);
                if (l > r) swap(l, r);
                int op = rng() % 5;
                if (op == 0) {
                    ll a = it % 3 == 0 ? rnd(0, 2) : rnd(0, MD - 1), b = rnd(0, MD - 1);
                    t.apply(l, r, {true, a, b});
                    for (int i = l; i < r; i++) A[i] = a, B[i] = b;
                    chk("comp:apply(l,r)", true);
                } else if (op == 1 && n) {
                    int p = rnd(0, n - 1);
                    ll a = rnd(0, MD - 1), b = rnd(0, MD - 1);
                    t.set(p, {a, b, 1}), A[p] = a, B[p] = b;
                    chk("comp:set", true);
                } else {
                    ll x = rnd(0, MD - 1), y = x;
                    for (int i = l; i < r; i++) y = (A[i] * y + B[i]) % MD;
                    auto s = t.prod(l, r);
                    chk("comp:prod", (s.a * x + s.b) % MD == y && s.len == r - l);
                }
            }
            ll x = 5, y = 5;
            for (int i = 0; i < n; i++) y = (A[i] * y + B[i]) % MD;
            auto s = t.all_prod();
            chk("comp:all_prod", (s.a * x + s.b) % MD == y);
        }
    }
    {
        int n = 500000;
        vector<AffSum::S> v(n);
        for (int i = 0; i < n; i++) v[i] = {rnd(0, MD - 1), 1};
        auto t0 = clock();
        LazySeg<AffSum> t(v);
        ll h = 0;
        for (int q = 0; q < 500000; q++) {
            int l = rnd(0, n), r = rnd(0, n);
            if (l > r) swap(l, r);
            if (q & 1) t.apply(l, r, {rnd(0, MD - 1), rnd(0, MD - 1)});
            else h ^= t.prod(l, r).sum;
        }
        printf("n=5e5, 5e5 affine/sum ops: %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC,
               h & 1);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
