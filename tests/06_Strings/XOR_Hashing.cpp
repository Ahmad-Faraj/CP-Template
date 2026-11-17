// Test for 06_Strings/XOR_Hashing.cpp against comparing std::set of prefix values directly.
// Every public method is checked: tag_of (stable, distinct), prefix (vector<int>, vector<long long>),
// and prefix equality <=> set equality across two arrays.
#include "../../06_Strings/XOR_Hashing.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(5);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 12 + 1, m = rng() % 12 + 1, k = rng() % 6 + 1;
        vector<long long> vals = {0, -1, LLONG_MIN, LLONG_MAX, 5, -7};
        vector<long long> a(n);
        vector<int> b(m);
        for (auto &x : a) x = vals[rng() % k];
        for (auto &x : b) x = (int)max<long long>(INT_MIN, min<long long>(INT_MAX, vals[rng() % k]));
        XorHashing h;
        auto pa = h.prefix(a);
        auto pb = h.prefix(b);
        chk("prefix(size)", (int)pa.size() == n && (int)pb.size() == m);
        set<long long> sa, sb;
        for (int x = 0; x < n; x++) {
            sa.insert(a[x]);
            sb.clear();
            for (int y = 0; y < m; y++) {
                sb.insert(b[y]);
                chk("prefix(equal<=>same set)", (pa[x] == pb[y]) == (sa == sb));
            }
        }
        // same array against itself: prefix i == prefix j  <=>  same distinct set
        bool ok = true;
        set<long long> si;
        for (int i = 0; i < n; i++) {
            si.insert(a[i]);
            set<long long> sj;
            for (int j = 0; j < n; j++) sj.insert(a[j]), ok &= (pa[i] == pa[j]) == (si == sj);
        }
        chk("prefix(self)", ok);
        long long v = vals[rng() % 6];
        auto t1 = h.tag_of(v), t2 = h.tag_of(v), t3 = h.tag_of(v ^ 1);
        chk("tag_of", t1 == t2 && t1 != t3);
    }
    chk("prefix(empty)", XorHashing().prefix(vector<int>()).empty());
    {
        int n = 1000000;
        vector<long long> a(n), b(n);
        for (int i = 0; i < n; i++) a[i] = rng() % 200000, b[i] = rng() % 200000;
        XorHashing h;
        auto t0 = clock();
        auto pa = h.prefix(a), pb = h.prefix(b);
        long long eq = 0;
        for (int i = 0; i < n; i++) eq += pa[i] == pb[n - 1 - i];
        printf("two n=1e6 prefix builds + 1e6 compares: %.3fs (%lld equal)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, eq);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
