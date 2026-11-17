// Test for 01_Data_Structures/MEX_All_Subarrays.cpp against the mex of every subarray in O(n^2).
// Every public function is checked: subarray_mexes (the whole flag vector), mex_of_subarray_mexes,
// on values in [0, n), duplicates, negatives and values >= n, plus the CF 1436E samples.
#include "../../01_Data_Structures/MEX_All_Subarrays.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(1436);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

vector<bool> brute(const vector<int> &a) {
    int n = a.size();
    vector<bool> f(n + 2, false);
    for (int i = 0; i < n; i++) {
        set<int> s;
        int m = 0;
        for (int j = i; j < n; j++) {
            s.insert(a[j]);
            while (s.count(m)) m++;
            f[m] = true;
        }
    }
    return f;
}

int main() {
    for (int it = 0; it < 150000; it++) {
        int n = rng() % 13;
        int mode = rng() % 4;
        vector<int> a(n);
        for (auto &x : a) {
            if (mode == 0) x = rng() % max(n, 1);               // the CF range, 0-based
            else if (mode == 1) x = rng() % 3;                  // tiny alphabet
            else if (mode == 2) x = (int)(rng() % (n + 4)) - 2; // negatives and >= n
            else x = rng() % 2 ? rng() % max(n, 1) : (int)rng(); // huge values
        }
        if (mode == 0 && n && rng() % 3 == 0) { // permutations, which reach mex n
            iota(a.begin(), a.end(), 0);
            shuffle(a.begin(), a.end(), rng);
        }
        vector<bool> want = brute(a);
        chk("subarray_mexes", subarray_mexes(a) == want);
        int m = 0;
        while (want[m]) m++;
        chk("mex_of_subarray_mexes", mex_of_subarray_mexes(a) == m);
    }
    { // CF 1436E samples: values 1..n, answer = mex_of_subarray_mexes(a - 1) + 1
        vector<int> s1 = {1, 3, 2}, s2 = {1, 4, 3, 1, 2};
        for (auto &x : s1) x--;
        for (auto &x : s2) x--;
        chk("cf1436e_samples",
            mex_of_subarray_mexes(s1) + 1 == 3 && mex_of_subarray_mexes(s2) + 1 == 6);
        chk("empty", mex_of_subarray_mexes({}) == 0 && subarray_mexes({}).size() == 2);
    }
    for (int test = 0; test < 3; test++) { // large: permutation, 0 x 0 y 0 z..., small values
        int n = 200000;
        vector<int> a(n);
        if (test == 0) iota(a.begin(), a.end(), 0), shuffle(a.begin(), a.end(), rng);
        if (test == 1)
            for (int i = 0; i < n; i++) a[i] = i % 2 ? i / 2 + 1 : 0;
        if (test == 2)
            for (auto &x : a) x = rng() % 50;
        auto t0 = clock();
        int m = mex_of_subarray_mexes(a);
        double secs = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("n=2e5 case %d: answer %d in %.3fs\n", test, m, secs);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
