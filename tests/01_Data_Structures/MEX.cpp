// Test for 01_Data_Structures/MEX.cpp against a std::set brute force.
// Every public function is checked: mex.
#include "../../01_Data_Structures/MEX.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(7);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (int it = 0; it < 200000; it++) {
        int n = rng() % 12;
        int range = vector<int>{1, 3, 8, 15, 1000000000}[rng() % 5];
        vector<int> a(n);
        for (auto &x : a) x = (int)(rng() % (2 * range + 1)) - (rng() % 4 == 0 ? range : 0);
        set<int> s(a.begin(), a.end());
        int want = 0;
        while (s.count(want)) want++;
        chk("mex", mex(a) == want);
    }
    chk("mex(empty)", mex({}) == 0);
    chk("mex(INT_MAX, INT_MIN)", mex({INT_MAX, INT_MIN, 0}) == 1);
    {
        vector<int> a(2000000);
        iota(a.begin(), a.end(), 0);
        shuffle(a.begin(), a.end(), rng);
        auto t0 = clock();
        int r = mex(a);
        printf("n=2e6: %.3fs\n", double(clock() - t0) / CLOCKS_PER_SEC);
        chk("mex(permutation)", r == 2000000);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
