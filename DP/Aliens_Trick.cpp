// Aliens Trick: drops the "exactly k pieces" constraint by charging a penalty per piece, then binary searches it.
// Use when: "exactly k segments / groups / transactions" and the plain DP with a k dimension is too slow.
// Handles: minimisation and maximisation, any inner solver, and reporting the penalty that was found.
// Time: O(log(hi - lo)) calls to your solver
// Indexing: k is a count, not an index; lo and hi bound the penalty, not the answer
// Note: only correct when the best cost is CONVEX in k. Verify that first - it is the whole premise, not a detail.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Your solver ALWAYS adds the penalty once per piece - for maximisation too, where a negative penalty
// is what makes pieces attractive. It returns {best value counting the penalty, fewest pieces among
// the best}. Returning the FEWEST pieces is what makes the search land on the right side of a plateau.
using Penalised = pair<ll, int>;

// Minimum cost using exactly k pieces. Charging more per piece pushes the solver to use fewer,
// so the smallest penalty whose optimum already fits in k pieces is the one to undo.
template <typename Solver> pair<ll, ll> aliens_min(int k, ll lo, ll hi, Solver solve) {
    ll best_penalty = hi;
    while (lo <= hi) {
        ll mid = lo + (hi - lo) / 2;
        if (solve(mid).second <= k)
            best_penalty = mid, hi = mid - 1;
        else
            lo = mid + 1;
    }
    return {solve(best_penalty).first - best_penalty * k, best_penalty};
}

// Maximum value using exactly k pieces. A bigger penalty buys MORE pieces here, so the piece count
// runs the other way and the search takes the largest penalty that still fits in k - the mirror of
// aliens_min, not a copy of it. The solver still adds the penalty, so undoing it is still a subtraction.
template <typename Solver> pair<ll, ll> aliens_max(int k, ll lo, ll hi, Solver solve) {
    ll best_penalty = lo;
    while (lo <= hi) {
        ll mid = lo + (hi - lo) / 2;
        if (solve(mid).second <= k)
            best_penalty = mid, lo = mid + 1;
        else
            hi = mid - 1;
    }
    return {solve(best_penalty).first - best_penalty * k, best_penalty};
}

// True when the costs are convex in k, which is exactly what the trick requires. Run this on small
// inputs while writing the solution; if it is false, the trick does not apply and no penalty helps.
bool is_convex(const vector<ll> &cost_by_k) {
    for (size_t i = 2; i < cost_by_k.size(); i++)
        if (cost_by_k[i] - cost_by_k[i - 1] < cost_by_k[i - 1] - cost_by_k[i - 2]) return false;
    return true;
}

// Standard problem: split a into exactly k contiguous non-empty parts, minimising the sum over parts
// of (sum of that part)^2. The inner solver here is a plain O(n^2) DP; swap in a faster one (CHT,
// divide and conquer, Knuth) and the wrapper above does not change.
void solve() {
    int n, k;
    cin >> n >> k;
    vector<ll> a(n);
    for (ll &x : a) cin >> x;
    vector<ll> prefix(n + 1, 0);
    for (int i = 0; i < n; i++) prefix[i + 1] = prefix[i] + a[i];

    // with a penalty of `pen` per part, minimise cost + pen * parts, and among the ties take fewest parts
    auto solver = [&](ll pen) -> Penalised {
        const ll INF = LLONG_MAX / 4;
        vector<ll> dp(n + 1, INF);
        vector<int> parts(n + 1, 0);
        dp[0] = 0;
        for (int i = 1; i <= n; i++)
            for (int j = 0; j < i; j++) {
                if (dp[j] >= INF) continue;
                ll seg = prefix[i] - prefix[j];
                ll val = dp[j] + seg * seg + pen;
                if (val < dp[i] || (val == dp[i] && parts[j] + 1 < parts[i])) dp[i] = val, parts[i] = parts[j] + 1;
            }
        return {dp[n], parts[n]};
    };

    ll total = prefix[n] < 0 ? -prefix[n] : prefix[n];
    auto [answer, penalty] = aliens_min(k, 0, total * total + 1, solver);
    cout << answer << '\n';
}
