// Test for 01_Data_Structures/MEX_With_Updates.cpp against recomputing the mex with a std::set.
// Every public method is checked: constructor, mex, update (negatives, huge values, duplicates).
#include "../../01_Data_Structures/MEX_With_Updates.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(11);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int brute(const vector<int> &a) {
    set<int> s(a.begin(), a.end());
    int r = 0;
    while (s.count(r)) r++;
    return r;
}

int rnd_val(int n) {
    int t = rng() % 10;
    if (t == 0) return -(int)(rng() % 5) - 1;          // negative
    if (t == 1) return (int)(rng() % 1000000000);     // huge
    if (t == 2) return INT_MAX;
    return (int)(rng() % (n + 2));                    // near the mex
}

int main() {
    { // empty array
        Mex m(vector<int>{});
        chk("mex(empty)", m.mex() == 0);
    }
    { // ABC330 E sample 1
        Mex m({2, 0, 2, 2, 1, 1, 2, 5});
        vector<pair<int, int>> q = {{4, 3}, {4, 4}, {6, 3}, {8, 1000000000}, {2, 1}};
        vector<int> want = {4, 3, 6, 5, 0}, got;
        for (auto [i, x] : q) m.update(i - 1, x), got.push_back(m.mex());
        chk("samples", got == want);
    }
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 9 + 1;
        vector<int> a(n);
        for (auto &x : a) x = rnd_val(n);
        Mex m(a);
        chk("constructor+mex", m.mex() == brute(a));
        int q = rng() % 30;
        for (int i = 0; i < q; i++) {
            int idx = rng() % n, x = rng() % 3 ? rnd_val(n) : a[rng() % n]; // dupes too
            a[idx] = x;
            m.update(idx, x);
            chk("update", m.A == a);
            chk("mex", m.mex() == brute(a));
        }
    }
    { // timing: 2e5 elements, 2e5 updates
        int n = 200000;
        vector<int> a(n);
        for (auto &x : a) x = rng() % n;
        auto t0 = clock();
        Mex m(a);
        long long s = 0;
        for (int i = 0; i < 200000; i++) m.update(rng() % n, rng() % (n + 1)), s += m.mex();
        printf("n=2e5 build + 2e5 updates/mex: %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC,
               s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
