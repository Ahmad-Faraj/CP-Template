// Test for 02_Range_Query/Merge_Sort_Tree.cpp against counting a[l..r] > k with a plain loop.
// Every public method is checked: both constructors, build, update, query (incl. l > r, extreme k).
#include "../../02_Range_Query/Merge_Sort_Tree.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(23);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int brute(const vector<int> &a, int l, int r, int k) {
    int c = 0;
    for (int i = l; i <= r; i++) c += a[i - 1] > k;
    return c;
}

int rv(int range) {
    if (rng() % 20 == 0) return rng() % 2 ? INT_MIN : INT_MAX;
    return (int)(rng() % (2 * range + 1)) - range;
}

int main() {
    { // SPOJ KQUERY sample: 5 / 5 1 2 3 4 / 3 / 2 4 1 / 4 4 4 / 1 5 2 -> 2 0 3
        Merge_Sort_Tree st(5, {5, 1, 2, 3, 4});
        chk("samples", st.query(2, 4, 1) == 2 && st.query(4, 4, 4) == 0 && st.query(1, 5, 2) == 3);
    }
    { // n = 0: everything empty
        Merge_Sort_Tree st(0, {});
        chk("n=0", st.query(1, 0, 5) == 0);
    }
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 12 + 1, range = vector<int>{2, 5, 1000}[rng() % 3];
        vector<int> a(n);
        for (auto &x : a) x = rv(range);
        Merge_Sort_Tree st;
        bool two = rng() % 2;
        if (two)
            st = Merge_Sort_Tree(n, a);
        else
            st = Merge_Sort_Tree(n), st.build(a);
        string ctor = two ? "Merge_Sort_Tree(n, a)" : "Merge_Sort_Tree(n)+build";
        int q = rng() % 25;
        for (int i = 0; i < q; i++) {
            if (rng() % 4 == 0) {
                int p = rng() % n + 1, v = rv(range);
                a[p - 1] = v;
                st.update(p, v);
                int k = rv(range);
                chk("update", st.query(1, n, k) == brute(a, 1, n, k));
                continue;
            }
            int l = rng() % n + 1, r = rng() % n + 1;
            if (rng() % 8 == 0) swap(l, r), l += (l == r); // sometimes l > r: empty
            if (l <= r) {
                int k = rng() % 5 ? rv(range) : (rng() % 2 ? INT_MIN : INT_MAX);
                chk("query", st.query(l, r, k) == brute(a, l, r, k));
                chk(ctor, st.query(l, r, k) == brute(a, l, r, k));
            } else {
                chk("query(l>r)", st.query(l, r, 0) == 0);
            }
        }
    }
    { // timing: n = 1e5 build + 1e5 queries
        int n = 100000;
        vector<int> a(n);
        for (auto &x : a) x = rng() % 1000000000;
        auto t0 = clock();
        Merge_Sort_Tree st(n, a);
        long long s = 0;
        for (int i = 0; i < 100000; i++) {
            int l = rng() % n + 1, r = rng() % n + 1;
            if (l > r) swap(l, r);
            s += st.query(l, r, rng() % 1000000000);
        }
        printf("n=1e5 build + 1e5 queries: %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
