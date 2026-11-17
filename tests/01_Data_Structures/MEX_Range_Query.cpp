// Test for 01_Data_Structures/MEX_Range_Query.cpp against a std::set mex of every queried range.
// Every public piece is checked: mex_queries, and MexSegTree (constructor, set, get) used directly
// in an online sweep, with duplicates, negatives, huge values, n = 0 and repeated queries.
#include "../../01_Data_Structures/MEX_Range_Query.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(321);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int brute(const vector<int> &a, int l, int r) {
    set<int> s(a.begin() + l, a.begin() + r + 1);
    int m = 0;
    while (s.count(m)) m++;
    return m;
}

int main() {
    for (int it = 0; it < 60000; it++) {
        int n = rng() % 16;
        int mode = rng() % 3;
        vector<int> a(n);
        for (auto &x : a) {
            if (mode == 0) x = rng() % (n + 1);
            else if (mode == 1) x = (int)(rng() % 6) - 2;
            else x = rng() % 2 ? (int)(rng() % 4) : (int)rng(); // includes INT_MIN..INT_MAX range
        }
        if (n && rng() % 4 == 0) iota(a.begin(), a.end(), 0), shuffle(a.begin(), a.end(), rng);
        vector<pair<int, int>> qs;
        int q = n ? rng() % 25 : 0;
        for (int i = 0; i < q; i++) {
            int l = rng() % n, r = rng() % n;
            if (l > r) swap(l, r);
            qs.push_back({l, r});
            if (rng() % 5 == 0) qs.push_back({l, r}); // repeated query
        }
        vector<int> got = mex_queries(a, qs);
        bool ok = got.size() == qs.size();
        for (int i = 0; ok && i < (int)qs.size(); i++)
            ok &= got[i] == brute(a, qs[i].first, qs[i].second);
        chk("mex_queries", ok);
        // MexSegTree online: after adding a[0..r], every l is answerable
        MexSegTree s(n + 1);
        for (int r = 0; r < n; r++) {
            if (0 <= a[r] && a[r] < n) s.set(a[r], r + 1);
            for (int l = 0; l <= r; l++) chk("MexSegTree:set+get", s.get(l + 1) == brute(a, l, r));
        }
    }
    chk("empty", mex_queries({}, {}).empty());
    {
        int n = 200000, q = 200000;
        vector<int> a(n);
        for (auto &x : a) x = rng() % 2 ? rng() % 1000 : rng() % n;
        vector<pair<int, int>> qs(q);
        for (auto &[l, r] : qs) {
            l = rng() % n, r = rng() % n;
            if (l > r) swap(l, r);
        }
        auto t0 = clock();
        vector<int> got = mex_queries(a, qs);
        printf("n=2e5, q=2e5: %.3fs\n", double(clock() - t0) / CLOCKS_PER_SEC);
        bool ok = true; // spot-check a few against the brute force
        for (int i = 0; i < 20; i++) ok &= got[i] == brute(a, qs[i].first, qs[i].second);
        chk("large:spot", ok);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
