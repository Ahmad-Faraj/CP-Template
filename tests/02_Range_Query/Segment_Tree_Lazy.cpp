// Test for 02_Range_Query/Segment_Tree_Lazy.cpp against a plain array.
// Every public method is checked: init, build, update (range add), query (range sum).
#include "../../02_Range_Query/Segment_Tree_Lazy.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(31);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 20 + 1;
        ll big = vector<ll>{5, 1000000000, 1000000000000LL}[rng() % 3];
        vector<ll> a(n);
        bool use_build = rng() % 4;
        for (auto &x : a) x = use_build ? (ll)(rng() % (2 * big + 1)) - big : 0;
        Lazy_Segment_Tree st;
        st.init(n);
        if (use_build) st.build(a);
        for (int o = 0; o < 40; o++) {
            int l = rng() % n + 1, r = rng() % n + 1;
            if (l > r) swap(l, r);
            if (rng() % 2) {
                ll v = (ll)(rng() % (2 * big + 1)) - big;
                st.update(l, r, v);
                for (int i = l; i <= r; i++) a[i - 1] += v;
            } else {
                ll s = 0;
                for (int i = l; i <= r; i++) s += a[i - 1];
                chk(use_build ? "build+query" : "init+query", st.query(l, r) == s);
            }
        }
        bool ok = true; // every single point and the full range at the end
        for (int i = 1; i <= n; i++) ok &= st.query(i, i) == a[i - 1];
        chk("update(points)", ok);
        chk("query(all)", st.query(1, n) == accumulate(a.begin(), a.end(), 0LL));
        chk("query(empty l>r)", st.query(3, 2) == 0);
    }
    { // CSES 1651 sample: 8 3; 3 2 4 5 1 1 5 3; ops "2 4", "1 2 5 1", "2 4" -> 5 then 6
        Lazy_Segment_Tree st;
        st.init(8);
        st.build({3, 2, 4, 5, 1, 1, 5, 3});
        bool ok = st.query(4, 4) == 5;
        st.update(2, 5, 1);
        ok &= st.query(4, 4) == 6;
        chk("samples", ok);
    }
    {
        int n = 200000;
        Lazy_Segment_Tree st;
        st.init(n);
        vector<ll> a(n);
        for (auto &x : a) x = rng() % 1000000000;
        st.build(a);
        auto t0 = clock();
        ll s = 0;
        for (int i = 0; i < 200000; i++) {
            int l = rng() % n + 1, r = rng() % n + 1;
            if (l > r) swap(l, r);
            if (i % 2) st.update(l, r, rng() % 1000000000);
            else s ^= st.query(l, r);
        }
        printf("n=q=2e5: %.3fs (checksum %lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
