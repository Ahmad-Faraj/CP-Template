// Test for 06_Strings/Suffix_Array_Integers.cpp against sorting all suffixes with std::sort on vectors.
// Every public member is checked: constructor (build, count_sort, build_lcp), p, c (rank), lcp, n, s.
#include "../../06_Strings/Suffix_Array_Integers.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(3);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (int it = 0; it < 40000; it++) {
        int n = rng() % 18, k = vector<int>{1, 2, 3, 1000}[rng() % 4];
        vector<int> v(n);
        for (auto &x : v) x = rng() % k + 1;
        if (n && rng() % 5 == 0) v[rng() % n] = INT_MAX;
        SuffixArray sa(v);
        vector<int> ord(n);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [&](int a, int b) {
            return lexicographical_compare(v.begin() + a, v.end(), v.begin() + b, v.end());
        });
        vector<int> want_p = {n};
        want_p.insert(want_p.end(), ord.begin(), ord.end());
        chk("n/s", sa.n == n + 1 && (int)sa.s.size() == n + 1 && sa.s.back() == 0);
        chk("p", sa.p == want_p);
        bool okc = true, okl = sa.lcp[0] == 0;
        for (int r = 0; r <= n; r++) okc &= sa.c[want_p[r]] == r;
        for (int r = 1; r <= n; r++) {
            int a = want_p[r - 1], b = want_p[r], L = 0;
            while (a + L < n && b + L < n && v[a + L] == v[b + L]) L++;
            okl &= sa.lcp[r] == L;
        }
        chk("c(rank)", okc);
        chk("lcp", okl);
    }
    {
        int n = 1000000;
        vector<int> v(n);
        for (auto &x : v) x = rng() % 3 + 1;
        auto t0 = clock();
        SuffixArray sa(v);
        printf("n=1e6 build + lcp: %.3fs\n", double(clock() - t0) / CLOCKS_PER_SEC);
        vector<int> w(200000, 7); // periodic worst case
        SuffixArray sw(w);
        bool ok = true;
        for (int r = 1; r <= 200000; r++) ok &= sw.p[r] == 200000 - r && sw.lcp[r] == r - 1;
        chk("large:all-equal", ok);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
