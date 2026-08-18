// Brute-force test for:
//   DP/Aliens_Trick.cpp
// The reference is the exact O(n^2 k) DP that carries the piece count as a real dimension -
// the thing the trick exists to avoid. If the trick is right, the two agree for every k.
// Run with: py tools/test.py

#include "harness.h"
using namespace std;
using ll = long long;

namespace AT {
#include "../DP/Aliens_Trick.cpp"
}

// exact: minimum sum of (segment sum)^2 over splits of a into exactly k contiguous parts
static vector<ll> exact_costs(const vector<ll> &a) {
    int n = (int)a.size();
    const ll INF = LLONG_MAX / 4;
    vector<ll> prefix(n + 1, 0);
    for (int i = 0; i < n; i++) prefix[i + 1] = prefix[i] + a[i];
    vector<vector<ll>> dp(n + 1, vector<ll>(n + 1, INF));
    dp[0][0] = 0;
    for (int i = 1; i <= n; i++)
        for (int k = 1; k <= i; k++)
            for (int j = 0; j < i; j++)
                if (dp[j][k - 1] < INF) {
                    ll seg = prefix[i] - prefix[j];
                    dp[i][k] = min(dp[i][k], dp[j][k - 1] + seg * seg);
                }
    vector<ll> out(n + 1, INF);
    for (int k = 0; k <= n; k++) out[k] = dp[n][k];
    return out;
}

int main() {
    name("Aliens_Trick");
    mt19937_64 gen(20260819);
    ll minFail = 0, maxFail = 0, convexTrue = 0, convexFalse = 0, penaltyFail = 0, cases = 0;

    for (int it = 0; it < 4000; it++) {
        int n = 1 + (int)(gen() % 8);
        vector<ll> a(n);
        for (ll &x : a) x = (ll)(gen() % 20) + 1; // positive, so the cost really is convex in k
        vector<ll> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) prefix[i + 1] = prefix[i] + a[i];
        vector<ll> want = exact_costs(a);
        cases++;

        auto solver = [&](ll pen) -> AT::Penalised {
            const ll INF = LLONG_MAX / 4;
            vector<ll> dp(n + 1, INF);
            vector<int> parts(n + 1, 0);
            dp[0] = 0;
            for (int i = 1; i <= n; i++)
                for (int j = 0; j < i; j++) {
                    if (dp[j] >= INF) continue;
                    ll seg = prefix[i] - prefix[j];
                    ll val = dp[j] + seg * seg + pen;
                    if (val < dp[i] || (val == dp[i] && parts[j] + 1 < parts[i]))
                        dp[i] = val, parts[i] = parts[j] + 1;
                }
            return {dp[n], parts[n]};
        };

        ll hi = prefix[n] * prefix[n] + 1;
        for (int k = 1; k <= n; k++) {
            auto got = AT::aliens_min(k, 0LL, hi, solver);
            if (got.first != want[k]) minFail++;
            // the penalty it reports must actually make k a best answer, not merely produce the number
            AT::Penalised at = solver(got.second);
            if (at.first - got.second * (ll)k != want[k]) penaltyFail++;
        }

        // the convexity check must agree with the real curve
        vector<ll> curve(want.begin() + 1, want.begin() + n + 1);
        bool reallyConvex = true;
        for (int i = 2; i < (int)curve.size(); i++)
            if (curve[i] - curve[i - 1] < curve[i - 1] - curve[i - 2]) reallyConvex = false;
        if (AT::is_convex(curve) != reallyConvex) (reallyConvex ? convexTrue : convexFalse)++;
    }

    // ---- maximisation, on a problem whose exact answer is easy: pick exactly k of n items,
    //      maximising their total. The best-k curve is concave, which is what aliens_max needs.
    ll pickFail = 0;
    for (int it = 0; it < 4000; it++) {
        int n = 1 + (int)(gen() % 10);
        vector<ll> v(n);
        for (ll &x : v) x = (ll)(gen() % 200) - 100;
        vector<ll> sorted = v;
        sort(sorted.rbegin(), sorted.rend());
        // with a reward of `pen` per item taken, take every item whose value + pen is positive
        auto solver = [&](ll pen) -> AT::Penalised {
            ll total = 0;
            int taken = 0;
            for (ll x : v)
                if (x + pen > 0) total += x + pen, taken++;
            return {total, taken};
        };
        for (int k = 1; k <= n; k++) {
            ll want = 0;
            for (int i = 0; i < k; i++) want += sorted[i];
            if (AT::aliens_max(k, -200LL, 200LL, solver).first != want) pickFail++;
        }
    }

    check("min: exactly k parts vs O(n^2 k) DP", minFail, cases);
    check("min: reported penalty really works", penaltyFail, cases);
    check("max: exactly k picks vs sorting", pickFail);
    check("is_convex agrees on convex curves", convexTrue, cases);
    check("is_convex agrees on non-convex", convexFalse, cases);
    return report();
}
