// Test for 10_Misc/Misc_Helpers.cpp.
// Oracle: naive loops (prefix sums, O(nm) substring scan, repeated multiplication) for small inputs
// and __int128 arithmetic near the overflow limits stated in the header.
// Checked: issubstring, sumall, sumodd, sumeven, get_log, log_floor. Sample: CSES 1083.
#include "../../10_Misc/Misc_Helpers.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(1083);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (int it = 0; it < 200000; it++) {
        string a(rng() % 4, 'a'), b(rng() % 8, 'a');
        for (auto &c : a) c = 'a' + rng() % 2;
        for (auto &c : b) c = 'a' + rng() % 2;
        bool want = false;
        for (size_t i = 0; i + a.size() <= b.size(); i++) want |= b.compare(i, a.size(), a) == 0;
        chk("issubstring", issubstring(a, b) == want);
    }
    { // naive prefix sums for x in [0, 1e5]
        ll all = 0, odd = 0, even = 0;
        bool o1 = true, o2 = true, o3 = true;
        for (ll x = 0; x <= 100000; x++) {
            all += x, (x % 2 ? odd : even) += x;
            o1 &= sumall(x) == all, o2 &= sumodd(x) == odd, o3 &= sumeven(x) == even;
        }
        chk("sumall(naive)", o1), chk("sumodd(naive)", o2), chk("sumeven(naive)", o3);
    }
    for (int it = 0; it < 200000; it++) { // big x up to the stated limits, vs __int128
        ll x = it % 3 == 0 ? 4294967295LL - (ll)(rng() % 100) : (ll)(rng() % 4294967296ULL);
        __int128 X = x;
        chk("sumall(big)", (__int128)sumall(x) == X * (X + 1) / 2);
        ll y = it % 3 == 0 ? 6074000997LL - (ll)(rng() % 100) : (ll)(rng() % 6074000998ULL);
        __int128 c = (y + 1) / 2, m = y / 2;
        chk("sumodd(big)", (__int128)sumodd(y) == c * c);
        chk("sumeven(big)", (__int128)sumeven(y) == m * (m + 1));
    }
    for (int it = 0; it < 300000; it++) {
        ll b = rng() % 3 ? rng() % 9 + 2 : rng() % 1000000 + 2;
        ll a = it % 4 == 0 ? LLONG_MAX - (ll)(rng() % 3) : (ll)(rng() % ((1ULL << (rng() % 62 + 1))) + 1);
        if (it % 5 == 0) { // exact powers
            __int128 p = 1;
            int e = rng() % 40;
            for (int i = 0; i < e && p * b <= LLONG_MAX; i++) p *= b;
            a = (ll)p;
        }
        int k = 0;
        __int128 p = b;
        while (p <= a) p *= b, k++;
        chk("log_floor", log_floor(a, b) == k);
        chk("get_log(within 1e-9 rel)", fabs(get_log(a, b) - log((double)a) / log((double)b)) < 1e-9);
    }
    chk("get_log(trap documented)", get_log(1000, 10) < 3.0 && log_floor(1000, 10) == 3);
    chk("edge:zero", sumall(0) == 0 && sumodd(0) == 0 && sumeven(0) == 0 && log_floor(1, 2) == 0);
    chk("edge:empty strings", issubstring("", "") && issubstring("", "x") && !issubstring("x", ""));
    // CSES 1083 Missing Number sample: n = 5, numbers 2 3 1 5 -> 4
    chk("samples(CSES 1083)", sumall(5) - (2 + 3 + 1 + 5) == 4);
    {
        auto t0 = clock();
        unsigned long long s = 0;
        for (ll x = 0; x < 10000000; x++) s += sumall(x) ^ sumodd(x) ^ sumeven(x);
        for (int i = 0; i < 1000000; i++) s += log_floor((ll)(rng() >> 1) + 1, i % 30 + 2);
        printf("1e7 sums + 1e6 log_floor: %.3fs (%llu)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
