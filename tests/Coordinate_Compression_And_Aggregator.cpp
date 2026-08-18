// Brute-force test for:
//   Misc/Coordinate_Compression.cpp
//   Misc/Logarithmic_Subarray_Aggregator.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace CC {
#include "../Misc/Coordinate_Compression.cpp"
}
namespace LA {
#include "../Misc/Logarithmic_Subarray_Aggregator.cpp"
}

int main() {
    mt19937_64 gen(24680);
    int rankFail = 0, invFail = 0, containsFail = 0, compFail = 0, cntFail = 0, sizeFail = 0, lateFail = 0;
    int gcdFail = 0, andFail = 0, orFail = 0, endFail = 0, distFail = 0;
    int ccChecked = 0, laChecked = 0;

    // ---------- coordinate compression against a sorted-unique reference ----------
    for (int it = 0; it < 20000; it++) {
        int n = (int)(gen() % 12);
        vector<int> a(n);
        for (int &x : a) x = (int)(gen() % 21) - 10;
        CC::Coordinate_Compression<int> cc(a);
        ccChecked++;

        vector<int> want = a;
        sort(want.begin(), want.end());
        want.erase(unique(want.begin(), want.end()), want.end());

        if (cc.size() != (int)want.size()) sizeFail++;
        // rank is the index in the sorted-unique list, and value_at inverts it
        for (size_t i = 0; i < want.size(); i++) {
            if (cc.rank(want[i]) != (int)i) rankFail++;
            if (cc.value_at((int)i) != want[i]) invFail++;
        }
        // ranks are dense and 0-based, so they index an array of exactly size() directly
        vector<int> got = cc.compress(a);
        for (int i = 0; i < n; i++) {
            if (got[i] < 0 || got[i] >= cc.size()) compFail++;
            if (cc.value_at(got[i]) != a[i]) compFail++;
        }
        // contains, and the counting helpers
        for (int x = -12; x <= 12; x++) {
            bool wantHas = binary_search(want.begin(), want.end(), x);
            if (cc.contains(x) != wantHas) containsFail++;
            int below = (int)(lower_bound(want.begin(), want.end(), x) - want.begin());
            int atMost = (int)(upper_bound(want.begin(), want.end(), x) - want.begin());
            if (cc.count_below(x) != below) cntFail++;
            if (cc.count_at_most(x) != atMost) cntFail++;
        }
        // adding after the build must invalidate and rebuild transparently
        CC::Coordinate_Compression<int> late;
        for (int x : a) late.add(x);
        late.add(999);
        if (late.size() != (int)want.size() + 1) lateFail++;
        if (!late.contains(999)) lateFail++;
        if (late.rank(999) != (int)want.size()) lateFail++; // 999 is larger than everything
        for (size_t i = 0; i < want.size(); i++)
            if (late.rank(want[i]) != (int)i) lateFail++;
    }

    // ---------- subarray aggregates against the O(n^2) definition ----------
    for (int it = 0; it < 3000; it++) {
        int n = 1 + (int)(gen() % 10);
        vector<ll> a(n);
        for (ll &x : a) x = 1 + (ll)(gen() % 40);
        laChecked++;

        auto brute = [&](function<ll(ll, ll)> op) {
            map<ll, ll> m;
            for (int i = 0; i < n; i++) {
                ll cur = a[i];
                for (int j = i; j < n; j++) {
                    if (j > i) cur = op(cur, a[j]);
                    m[cur]++;
                }
            }
            return m;
        };
        if (LA::subarray_gcd_counts(a) != brute([](ll p, ll q) { return __gcd(p, q); })) gcdFail++;
        if (LA::subarray_and_counts(a) != brute([](ll p, ll q) { return p & q; })) andFail++;
        if (LA::subarray_or_counts(a) != brute([](ll p, ll q) { return p | q; })) orFail++;

        // the total count over all values must be exactly n*(n+1)/2
        ll total = 0;
        for (auto &kv : LA::subarray_gcd_counts(a)) total += kv.second;
        if (total != (ll)n * (n + 1) / 2) gcdFail++;

        // distinct values
        auto dist = LA::distinct_subarray_values(a, [](ll p, ll q) { return __gcd(p, q); });
        auto wantMap = brute([](ll p, ll q) { return __gcd(p, q); });
        vector<ll> wantDist;
        for (auto &kv : wantMap) wantDist.push_back(kv.first);
        if (dist != wantDist) distFail++;

        // per_endpoint: for each i, the pairs must describe exactly the subarrays ending at i
        auto ends = LA::per_endpoint(a, [](ll p, ll q) { return __gcd(p, q); });
        if ((int)ends.size() != n) endFail++;
        for (int i = 0; i < n; i++) {
            map<ll, ll> want2;
            ll cur = 0;
            for (int j = i; j >= 0; j--) {
                cur = (j == i) ? a[i] : __gcd(cur, a[j]);
                want2[cur]++;
            }
            map<ll, ll> got2;
            for (auto [v, c] : ends[i]) got2[v] += c;
            if (got2 != want2) endFail++;
            // and the O(log) claim: never more than about 2*log2(max) distinct values
            if ((int)ends[i].size() > 40) endFail++;
        }
    }

    cout << "Misc\n";
    cout << "  rank matches sorted-unique index : " << rankFail << " failures (" << ccChecked << " arrays)\n";
    cout << "  value_at inverts rank            : " << invFail << " failures\n";
    cout << "  contains                         : " << containsFail << " failures\n";
    cout << "  compress gives dense 0-based ids : " << compFail << " failures\n";
    cout << "  count_below / count_at_most      : " << cntFail << " failures\n";
    cout << "  size                             : " << sizeFail << " failures\n";
    cout << "  add() after build rebuilds       : " << lateFail << " failures\n";
    cout << "  subarray gcd counts vs O(n^2)    : " << gcdFail << " failures (" << laChecked << " arrays)\n";
    cout << "  subarray AND counts              : " << andFail << " failures\n";
    cout << "  subarray OR counts               : " << orFail << " failures\n";
    cout << "  distinct_subarray_values         : " << distFail << " failures\n";
    cout << "  per_endpoint structure           : " << endFail << " failures\n";
}
