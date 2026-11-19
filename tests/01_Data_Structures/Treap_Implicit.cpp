// Test for 01_Data_Structures/Treap_Implicit.cpp against a std::vector with naive insert / erase /
// reverse / loops. Every public method is checked: constructor, insert, erase, reverse, update,
// query, get, size, split, merge - with T = ll and T = ModInt<998244353>.
#include "../../01_Data_Structures/Treap_Implicit.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(7);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

const ll P = 998244353;
struct M { // minimal mod-p value type, independent of the library's ModInt
    ll v;
    M(ll x = 0) : v((x % P + P) % P) {}
    M operator+(M o) const { return M(v + o.v); }
    M operator*(M o) const { return M(v * o.v % P); }
};

ll R(ll lo, ll hi) { return lo + (ll)(rng() % (hi - lo + 1)); }

// ll: values small enough that sums never overflow (affine with b in {-1,0,1,2}, few rounds)
void run_ll(int it) {
    int n = R(0, 12), maxv = it % 2 ? 5 : 1000000;
    vector<ll> a(n);
    for (auto &x : a) x = R(-maxv, maxv);
    Treap_Implicit<ll> t(a);
    chk("ll:constructor", t.size() == n);
    for (int q = 0; q < 40; q++) {
        int op = R(0, 7), sz = a.size();
        if (op == 0) {
            int i = R(0, sz);
            ll x = R(-maxv, maxv);
            t.insert(i, x), a.insert(a.begin() + i, x);
            chk("ll:insert", t.size() == (int)a.size());
        } else if (op == 1 && sz) {
            int i = R(0, sz - 1);
            t.erase(i), a.erase(a.begin() + i);
            chk("ll:erase", t.size() == (int)a.size());
        } else if (op == 2) {
            int l = R(0, sz), r = R(l, sz);
            t.reverse(l, r), std::reverse(a.begin() + l, a.begin() + r);
            chk("ll:reverse", true);
        } else if (op == 3) {
            int l = R(0, sz), r = R(l, sz);
            ll b = R(-1, 1), c = R(-maxv, maxv);
            t.update(l, r, b, c);
            for (int i = l; i < r; i++) a[i] = b * a[i] + c;
            chk("ll:update", true);
        } else if (op == 4 || op == 5) {
            int l = R(0, sz), r = R(l, sz);
            ll s = 0;
            for (int i = l; i < r; i++) s += a[i];
            chk(l == r ? "ll:query(empty)" : "ll:query", t.query(l, r) == s);
        } else if (op == 6 && sz) {
            int i = R(0, sz - 1);
            chk("ll:get", t.get(i) == a[i]);
        } else if (op == 7) { // cut [l, r) and paste it at the front, via split / merge
            int l = R(0, sz), r = R(l, sz);
            auto [x, yz] = t.split(t.root, l);
            auto [y, z] = t.split(yz, r - l);
            chk("ll:split", t.t[x].sz == l && t.t[y].sz == r - l && t.t[z].sz == sz - r);
            t.root = t.merge(y, t.merge(x, z));
            vector<ll> b(a.begin() + l, a.begin() + r);
            b.insert(b.end(), a.begin(), a.begin() + l);
            b.insert(b.end(), a.begin() + r, a.end());
            a = b;
            chk("ll:merge", t.size() == (int)a.size());
        }
        // full content check, element by element - only sometimes: get() pushes every lazy tag,
        // and queries must also be checked on trees that still carry pending tags
        if (q % 10 != 9) continue;
        bool ok = t.size() == (int)a.size();
        for (int i = 0; ok && i < (int)a.size(); i++) ok &= t.get(i) == a[i];
        chk("ll:size+content", ok);
    }
}

void run_mod() {
    int n = R(0, 15);
    vector<ll> a(n);
    vector<M> am(n);
    for (int i = 0; i < n; i++) a[i] = R(0, P - 1), am[i] = M(a[i]);
    Treap_Implicit<M> t(am);
    for (int q = 0; q < 60; q++) {
        int op = R(0, 5), sz = a.size();
        if (op == 0) {
            int i = R(0, sz);
            ll x = R(0, P - 1);
            t.insert(i, M(x)), a.insert(a.begin() + i, x);
        } else if (op == 1 && sz) {
            int i = R(0, sz - 1);
            t.erase(i), a.erase(a.begin() + i);
        } else if (op == 2) {
            int l = R(0, sz), r = R(l, sz);
            t.reverse(l, r), std::reverse(a.begin() + l, a.begin() + r);
        } else if (op == 3) {
            int l = R(0, sz), r = R(l, sz);
            ll b = R(0, 2) ? R(0, P - 1) : R(0, 1), c = R(0, P - 1); // b = 0 (assign), 1 (add)
            t.update(l, r, M(b), M(c));
            for (int i = l; i < r; i++) a[i] = (b * a[i] + c) % P;
            chk("mod:update", true);
        } else if (op == 4) {
            int l = R(0, sz), r = R(l, sz);
            ll s = 0;
            for (int i = l; i < r; i++) s = (s + a[i]) % P;
            chk("mod:query", t.query(l, r).v == s);
        } else if (op == 5 && sz) {
            int i = R(0, sz - 1);
            chk("mod:get", t.get(i).v == a[i]);
        }
        if (q % 10 != 9) continue;
        bool ok = t.size() == (int)a.size();
        for (int i = 0; ok && i < (int)a.size(); i++) ok &= t.get(i).v == a[i];
        chk("mod:size+content", ok);
    }
}

int main() {
    for (int it = 0; it < 30000; it++) run_ll(it);
    for (int it = 0; it < 15000; it++) run_mod();
    { // edge: empty treap
        Treap_Implicit<ll> t;
        chk("edge:empty", t.size() == 0 && t.query(0, 0) == 0);
        t.reverse(0, 0), t.update(0, 0, 5, 5);
        t.insert(0, 7);
        chk("edge:single", t.size() == 1 && t.get(0) == 7 && t.query(0, 1) == 7);
        t.erase(0);
        chk("edge:emptied", t.size() == 0 && t.query(0, 0) == 0);
    }
    { // timing: 5e5 elements, 5e5 mixed ops (mod)
        int n = 500000;
        vector<M> a(n);
        for (auto &x : a) x = M(R(0, P - 1));
        auto t0 = clock();
        Treap_Implicit<M> t(a);
        ll h = 0;
        for (int q = 0; q < 500000; q++) {
            int sz = t.size(), op = R(0, 4);
            int l = R(0, sz - 1), r = R(l + 1, sz);
            if (op == 0) t.insert(l, M(R(0, P - 1)));
            else if (op == 1) t.erase(l);
            else if (op == 2) t.reverse(l, r);
            else if (op == 3) t.update(l, r, M(R(0, P - 1)), M(R(0, P - 1)));
            else h ^= t.query(l, r).v;
        }
        double sec = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("5e5 build + 5e5 mixed ops: %.3fs (h=%lld)\n", sec, h);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
