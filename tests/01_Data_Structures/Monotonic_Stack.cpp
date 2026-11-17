// Test for 01_Data_Structures/Monotonic_Stack.cpp against an O(n^2) scan for the nearest index.
// Every public function is checked: prev_smaller, next_smaller, prev_greater, next_greater, for
// T = int and T = long long with duplicates, negatives, n = 0, plus the histogram-rectangle use.
#include "../../01_Data_Structures/Monotonic_Stack.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(55);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

template <typename T> void one(const vector<T> &a, const string &tag) {
    int n = a.size();
    vector<int> ps(n, -1), ns(n, n), pg(n, -1), ng(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = i - 1; j >= 0; j--)
            if (a[j] < a[i]) { ps[i] = j; break; }
        for (int j = i - 1; j >= 0; j--)
            if (a[j] > a[i]) { pg[i] = j; break; }
        for (int j = i + 1; j < n; j++)
            if (a[j] < a[i]) { ns[i] = j; break; }
        for (int j = i + 1; j < n; j++)
            if (a[j] > a[i]) { ng[i] = j; break; }
    }
    chk(tag + "prev_smaller", prev_smaller(a) == ps);
    chk(tag + "next_smaller", next_smaller(a) == ns);
    chk(tag + "prev_greater", prev_greater(a) == pg);
    chk(tag + "next_greater", next_greater(a) == ng);
}

int main() {
    for (int it = 0; it < 100000; it++) {
        int n = rng() % 15;
        int range = vector<int>{1, 2, 4, 1000000000}[rng() % 4];
        vector<int> a(n);
        for (auto &x : a) x = (int)(rng() % range) - (rng() % 2 ? range / 2 : 0);
        one(a, "int:");
        vector<long long> b(n);
        for (auto &x : b) x = (long long)(rng() % 5 - 2) * 4000000000000000000LL;
        one(b, "ll:");
        // usage: largest rectangle in a histogram vs O(n^2)
        vector<int> h(n);
        for (auto &x : h) x = rng() % 6;
        vector<int> L = prev_smaller(h), R = next_smaller(h);
        long long best = 0, want = 0;
        for (int i = 0; i < n; i++) best = max(best, (long long)h[i] * (R[i] - L[i] - 1));
        for (int i = 0; i < n; i++)
            for (int j = i, mn = INT_MAX; j < n; j++)
                mn = min(mn, h[j]), want = max(want, 1LL * mn * (j - i + 1));
        chk("usage:histogram", best == want);
    }
    one(vector<int>{}, "empty:");
    one(vector<int>{1, 5, 0, 3, 4, 5}, "original_demo:");
    {
        int n = 3000000;
        vector<int> a(n);
        for (auto &x : a) x = rng() % 1000;
        auto t0 = clock();
        long long s = 0;
        s += prev_smaller(a)[n / 2] + next_smaller(a)[n / 3];
        s += prev_greater(a)[n / 4] + next_greater(a)[5];
        double secs = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("n=3e6, all four directions: %.3fs (%lld)\n", secs, s % 7);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
