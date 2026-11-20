// Test for 06_Strings/Hashing_Lazy_Segment_Tree.cpp against a std::string mirror.
// Every public method is checked: constructor (Base 0 and Base 1, T = int and long long),
// update(l, r, c), update(i, c), query(l, r), equal(...). Oracle for query: the naive little-endian
// hash sum (s[l+k]-'a'+1) * P^k mod M of the explicitly updated string, powers by repeated
// multiplication. equal() and hash equality must match substring equality on every pair of ranges
// (collisions are counted and reported).
#include "../../06_Strings/Hashing_Lazy_Segment_Tree.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(4242);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

// 1-based inclusive range of a 0-based string, with the bases the tree picked at random
template <class T, bool B> pair<ll, ll> naive(const string &s, int l, int r) {
    const ll *P = Hash_LazySegmentTree<T, B>::P, M[2] = {1000000007, 1000000009};
    ll h[2] = {0, 0};
    for (int k = 0; k < 2; k++) {
        ll p = 1;
        for (int i = l; i <= r; i++)
            h[k] = (h[k] + (s[i - 1] - 'a' + 1) * p) % M[k], p = p * P[k] % M[k];
    }
    return {h[0], h[1]};
}

long long collisions = 0;
template <class T, bool B> void run(int it) {
    string tag = string(sizeof(T) == 8 ? "ll" : "int") + (B ? "/Base1" : "");
    int n = 1 + rng() % 25, alpha = 1 + rng() % 3;
    string s(n, 'a');
    for (auto &c : s) c = 'a' + rng() % alpha;
    Hash_LazySegmentTree<T, B> st(n, B ? "#" + s : s);
    int ops = rng() % 30;
    for (int q = 0; q <= ops; q++) {
        int t = rng() % 4;
        int l = 1 + rng() % n, r = 1 + rng() % n;
        if (l > r) swap(l, r);
        char c = 'a' + rng() % alpha;
        if (t == 0) {
            st.update(l, r, c);
            for (int i = l; i <= r; i++) s[i - 1] = c;
        } else if (t == 1) {
            st.update(l, c);
            s[l - 1] = c;
        } else if (t == 2) {
            auto h = st.query(l, r);
            chk(tag + ":query", pair<ll, ll>(h.first, h.second) == naive<T, B>(s, l, r));
        } else {
            int a = 1 + rng() % (n - (r - l)), b = a + (r - l); // same length, random place
            bool eq = s.substr(l - 1, r - l + 1) == s.substr(a - 1, b - a + 1);
            chk(tag + ":equal", st.equal(l, r, a, b) == eq);
            int r2 = min(n, r + 1);
            chk(tag + ":equal(diff len)", r2 == r || !st.equal(l, r, l, r2));
        }
    }
    // after the updates: every range hash matches the rebuilt string, and hash equality matches
    // substring equality on every pair of equal-length ranges
    bool ok = true;
    for (int l = 1; l <= n; l++)
        for (int r = l; r <= n; r++) {
            auto h = st.query(l, r);
            ok &= pair<ll, ll>(h.first, h.second) == naive<T, B>(s, l, r);
            for (int a = 1; a + (r - l) <= n; a++) {
                bool eq = s.compare(l - 1, r - l + 1, s, a - 1, r - l + 1) == 0;
                bool heq = st.query(a, a + (r - l)) == h;
                if (heq && !eq) collisions++;
                ok &= eq == heq;
            }
        }
    chk(tag + ":all ranges", ok);
    // a fresh tree built on the explicitly updated string agrees with the updated tree
    Hash_LazySegmentTree<T, B> fresh(n, B ? "#" + s : s);
    bool same = true;
    for (int l = 1; l <= n; l++) same &= fresh.query(l, n) == st.query(l, n);
    chk(tag + ":rebuilt", same);
    (void)it;
}

int main() {
    for (int it = 0; it < 6000; it++) {
        run<int, 0>(it);
        run<long long, 0>(it);
        run<int, 1>(it);
    }
    {
        Hash_LazySegmentTree<> e(0, ""); // n = 0 must not crash
        chk("n=0", e.n == 0);
    }
    printf("collisions seen: %lld\n", collisions);
    {
        int n = 1000000;
        string s(n, 'a');
        for (auto &c : s) c = 'a' + rng() % 26;
        auto t0 = clock();
        Hash_LazySegmentTree<> st(n, s);
        ll x = 0;
        for (int i = 0; i < 500000; i++) {
            int l = 1 + rng() % n, r = 1 + rng() % n;
            if (l > r) swap(l, r);
            if (i % 2) st.update(l, r, char('a' + rng() % 26));
            else x += st.equal(l, (l + r) / 2, (l + r) / 2 + 1, r - ((r - l + 1) % 2));
        }
        printf("n=1e6 build + 5e5 ops (assign/equal): %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, x);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
