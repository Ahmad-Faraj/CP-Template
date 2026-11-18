// Test for 02_Range_Query/Segment_Tree_Lazy_Iterative.cpp against a plain array.
// Every public method is checked: both constructors, modInit, modify (any value map), query.
// Maps do not commute, so this also checks that updates are applied in the right order.
#include "../../02_Range_Query/Segment_Tree_Lazy_Iterative.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(37);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 20 + 1, V = vector<int>{3, 10, 101}[rng() % 3];
        bool from_vec = rng() % 2;
        vector<int> a(n, 0);
        if (from_vec)
            for (auto &x : a) x = rng() % V;
        LazyBinTreeUniversal t = from_vec ? LazyBinTreeUniversal(a) : LazyBinTreeUniversal(n);
        string tag = from_vec ? "vector:" : "size:";
        for (int o = 0; o < 40; o++) {
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r) swap(l, r);
            if (rng() % 2) {
                auto m = t.modInit();
                int kind = rng() % 3; // single x -> y (CF 911G), random map, or a shift
                if (kind == 0) m[rng() % V] = rng() % V;
                else if (kind == 1)
                    for (int x = 0; x < V; x++) m[x] = rng() % V;
                else
                    for (int x = 0; x < V; x++) m[x] = (x + 1) % V;
                t.modify(l, r, m);
                for (int i = l; i < r; i++) a[i] = m[a[i]];
            } else if (n) {
                int i = rng() % n;
                chk(tag + "query", t.query(i) == a[i]);
            }
        }
        bool ok = true;
        for (int i = 0; i < n; i++) ok &= t.query(i) == a[i];
        chk(tag + "modify(all points)", ok);
    }
    { // the bug the old version had: whole-range 3->5 then 5->7
        LazyBinTreeUniversal t(vector<int>{1, 2, 3, 4, 5, 6, 7, 8});
        auto m1 = t.modInit(), m2 = t.modInit();
        m1[3] = 5, m2[5] = 7;
        t.modify(0, 8, m1), t.modify(0, 8, m2);
        vector<int> got;
        for (int i = 0; i < 8; i++) got.push_back(t.query(i));
        chk("compose(3->5, 5->7)", got == vector<int>{1, 2, 7, 4, 7, 6, 7, 8});
    }
    chk("modInit is identity", [] {
        LazyBinTreeUniversal t(1);
        auto m = t.modInit();
        for (int x = 0; x < 101; x++)
            if (m[x] != x) return false;
        return true;
    }());
    { // CF 911G scale
        int n = 200000;
        vector<int> a(n);
        for (auto &x : a) x = rng() % 100 + 1;
        LazyBinTreeUniversal t(a);
        auto t0 = clock();
        long long s = 0;
        for (int i = 0; i < 200000; i++) {
            int l = rng() % n, r = rng() % n;
            if (l > r) swap(l, r);
            auto m = t.modInit();
            m[rng() % 100 + 1] = rng() % 100 + 1;
            t.modify(l, r + 1, m);
        }
        for (int i = 0; i < n; i++) s += t.query(i);
        printf("n=q=2e5 (CF 911G): %.3fs (checksum %lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
