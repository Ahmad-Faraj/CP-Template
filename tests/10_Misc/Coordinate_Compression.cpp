// Test for 10_Misc/Coordinate_Compression.cpp - every public method against a std::set oracle.
//
// Run (from the repo root):
//   g++ -std=c++17 -O1 -g -fsanitize=address,undefined tests/10_Misc/Coordinate_Compression.cpp -o t && ./t
// Options:
//   -DWRAP_INT_LL            put "#define int long long" before the template (how it is used in contests)
//   -DTEMPLATE_PATH='"f.cpp"' test another copy of the template instead of the installed one

#include <bits/stdc++.h>
using namespace std;
#ifdef WRAP_INT_LL
#define int long long
#endif

#ifdef TEMPLATE_PATH
#include TEMPLATE_PATH
#else
#include "../../10_Misc/Coordinate_Compression.cpp"
#endif

static map<string, pair<long long, long long>> cov; // method -> {checks, mismatches}
static void check(const string &method, bool ok) {
    cov[method].first++;
    if (!ok) cov[method].second++;
}

static mt19937_64 rng(20261117);
static long long rnd(long long lo, long long hi) { return uniform_int_distribution<long long>(lo, hi)(rng); }

// Oracle: std::set of every value added; ranks counted by walking the set.
template <typename T> static void compare_all(Coordinate_Compression<T> &cc, const set<T> &s, const vector<T> &probes,
                                              const string &tag) {
    auto name = [&](const char *m) { return tag + " " + m; };
    vector<T> sorted(s.begin(), s.end());
    check(name("size"), cc.size() == (long long)sorted.size());
    for (size_t r = 0; r < sorted.size(); r++) {
        check(name("value_at"), cc.value_at(r) == sorted[r]);
        check(name("rank"), cc.rank(sorted[r]) == (long long)r);
    }
    for (const T &x : probes) {
        long long below = 0, at_most = 0;
        for (const T &v : sorted) below += v < x, at_most += !(x < v);
        check(name("rank"), cc.rank(x) == below);
        check(name("count_below"), cc.count_below(x) == below);
        check(name("count_at_most"), cc.count_at_most(x) == at_most);
        check(name("contains"), cc.contains(x) == (s.count(x) > 0));
    }
    vector<T> q = probes;
    for (const T &v : sorted) q.push_back(v);
    auto got = cc.compress(q);
    bool ok = got.size() == q.size();
    for (size_t i = 0; ok && i < q.size(); i++) {
        long long below = 0;
        for (const T &v : sorted) below += v < q[i];
        ok = got[i] == below;
    }
    check(name("compress"), ok);
}

template <typename T, typename Gen> static void run(const string &tag, Gen gen) {
    auto name = [&](const char *m) { return tag + " " + m; };
    for (int trial = 0; trial < 3000; trial++) {
        int m = rnd(0, trial < 300 ? 5 : 40);
        vector<T> init(m);
        for (auto &x : init) x = gen();
        set<T> s(init.begin(), init.end());
        vector<T> probes(rnd(0, 15));
        for (auto &x : probes) x = gen();
        for (int i = 0; i < 3 && m; i++) probes.push_back(init[rnd(0, m - 1)]);

        bool use_vector_ctor = rnd(0, 1);
        Coordinate_Compression<T> cc = use_vector_ctor ? Coordinate_Compression<T>(init) : Coordinate_Compression<T>();
        auto all_in = [&] {
            bool ok = cc.size() == (long long)s.size();
            for (const T &x : init) ok = ok && cc.contains(x);
            return ok;
        };
        if (use_vector_ctor) check(name("Coordinate_Compression(vector)"), all_in());
        else {
            check(name("Coordinate_Compression()"), cc.size() == 0);
            if (rnd(0, 1)) {
                cc.add(init);
                check(name("add(vector)"), all_in());
            } else {
                for (const T &x : init) cc.add(x);
                check(name("add(x)"), all_in());
            }
        }
        compare_all(cc, s, probes, tag);

        // values added after the first build must show up (lazy rebuild), with and without an explicit build()
        int rounds = rnd(1, 4);
        for (int rd = 0; rd < rounds; rd++) {
            int t = rnd(0, 2);
            if (t == 0) {
                T x = gen();
                cc.add(x), s.insert(x);
                check(name("add(x)"), cc.contains(x));
            } else if (t == 1) {
                vector<T> more(rnd(0, 6));
                for (auto &x : more) x = gen();
                cc.add(more), s.insert(more.begin(), more.end());
                bool ok = true;
                for (auto &x : more) ok = ok && cc.contains(x);
                check(name("add(vector)"), ok);
            } else {
                cc.build();
                cc.build(); // safe to call more than once
                check(name("build"), cc.size() == (long long)s.size());
            }
            if (rnd(0, 1)) cc.build();
            compare_all(cc, s, probes, tag);
        }
    }
}

signed main() {
    run<int>("int", [] { return (int)rnd(-20, 20); });
    run<long long>("ll", [] { return rnd(0, 2) ? rnd(-30, 30) : rnd(LLONG_MIN, LLONG_MAX); });
    run<string>("string", [] {
        string s(rnd(0, 3), 'a');
        for (auto &c : s) c = 'a' + rnd(0, 2);
        return s;
    });

    // Large n timing (2e5): build from 2e5 values in [0, 1e9], then 2e5 rank + contains + count + value_at.
    {
        const int N = 200000;
        auto t0 = chrono::steady_clock::now();
        vector<long long> a(N);
        for (auto &x : a) x = rnd(0, 1000000000);
        Coordinate_Compression<long long> cc(a);
        vector<long long> sorted = a;
        sort(sorted.begin(), sorted.end());
        sorted.erase(unique(sorted.begin(), sorted.end()), sorted.end());
        long long sink = 0;
        for (int it = 0; it < N; it++) {
            long long x = rnd(0, 1000000000);
            sink ^= cc.rank(x) ^ cc.contains(x) ^ cc.count_at_most(x) ^ cc.count_below(x) ^ cc.value_at(it % cc.size());
        }
        auto ranks = cc.compress(a);
        bool ok = cc.size() == (long long)sorted.size();
        for (int i = 0; ok && i < N; i++) ok = sorted[ranks[i]] == a[i];
        check("ll compress_large", ok);
        double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
        printf("timing n=%lld: build + 5 x %lld queries + compress in %.1f ms (sink %lld)\n", (long long)N, (long long)N, ms, sink & 1);
    }

    bool pass = true;
    for (auto &[method, cm] : cov) {
        printf("coverage %-38s checks=%-9lld mismatches=%lld\n", method.c_str(), cm.first, cm.second);
        if (cm.first == 0 || cm.second != 0) pass = false;
    }
    printf("RESULT: %s\n", pass ? "PASS" : "FAIL");
    return pass ? 0 : 1;
}
