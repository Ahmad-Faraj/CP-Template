// Test for 10_Misc/Inversions.cpp against the O(n^2) pair count.
// count_inversions is checked on int, long long and string vectors: empty, n=1, duplicates, negatives,
// extremes, sorted / reversed inputs, and the original bug case [2, 1, 1] -> 2.
#include "../../10_Misc/Inversions.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(23);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

template <class T> long long brute(const vector<T> &a) {
    long long r = 0;
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = i + 1; j < a.size(); j++) r += a[j] < a[i];
    return r;
}

int main() {
    chk("count_inversions(fixed)", count_inversions(vector<int>{2, 1, 1}) == 2);
    chk("count_inversions(fixed)", count_inversions(vector<int>{}) == 0);
    chk("count_inversions(fixed)", count_inversions(vector<int>{5}) == 0);
    chk("count_inversions(fixed)", count_inversions(vector<int>{3, 3, 3}) == 0);
    chk("count_inversions(fixed)", count_inversions(vector<int>{INT_MAX, INT_MIN, 0}) == 2);
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 40;
        int R = vector<int>{1, 2, 5, 100, 1000000000}[rng() % 5];
        vector<int> a(n);
        for (auto &x : a) x = (int)((long long)(rng() % (2ULL * R + 1)) - R);
        int mode = rng() % 6;
        if (mode == 0) sort(a.begin(), a.end());
        if (mode == 1) sort(a.rbegin(), a.rend());
        chk("count_inversions<int>", count_inversions(a) == brute(a));
        vector<long long> b(n);
        for (auto &x : b) x = (long long)(rng() % 7) * (1LL << 60) / 7 - (rng() % 2 ? LLONG_MAX / 2 : 0);
        chk("count_inversions<ll>", count_inversions(b) == brute(b));
        vector<string> c(n % 12);
        for (auto &s : c) s = string(rng() % 3, char('a' + rng() % 3));
        chk("count_inversions<string>", count_inversions(c) == brute(c));
    }
    {
        int n = 200000;
        vector<int> a(n);
        for (int i = 0; i < n; i++) a[i] = n - i;
        chk("count_inversions(reversed 2e5)", count_inversions(a) == (long long)n * (n - 1) / 2);
        for (auto &x : a) x = rng() % 1000;
        auto t0 = clock();
        long long r = count_inversions(a);
        printf("n=2e5 random (many duplicates): %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, r);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
