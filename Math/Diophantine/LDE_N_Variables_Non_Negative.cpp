// LDE N Variables Non Negative: which totals k are reachable as a[0]*x[0] + ... with every x[i] >= 0, and how many.
// Use when: "which amounts can these coin values make", the money-changing problem, counting reachable totals.
// Handles: repeated and unsorted coefficients, k = 0, huge k, and counting over a range up to 1e18.
// Time: build O(min(a) * n * log(min(a))) | can O(1) | count O(min(a))
// Indexing: 0-based coefficient list; every a[i] must be strictly positive
// Note: the build cost is driven by the SMALLEST coefficient, so it is only practical while min(a) is modest.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct LDE_Non_Negative {
    static constexpr ll INF = numeric_limits<ll>::max() / 4;

    int n, modulus;
    vector<ll> cheapest; // cheapest[r] = the smallest reachable total congruent to r mod the smallest coefficient

    LDE_Non_Negative(vector<ll> a) {
        n = (int)a.size();
        modulus = (int)*min_element(a.begin(), a.end());
        assert(modulus > 0); // every coefficient must be positive for this to terminate
        cheapest.assign(modulus, INF);
        cheapest[0] = 0;
        // Dijkstra over residues: adding a[i] moves residue r to (r + a[i]) mod modulus
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> q;
        q.push({0, 0});
        while (!q.empty()) {
            auto [val, r] = q.top();
            q.pop();
            if (cheapest[r] != val) continue;
            for (int i = 0; i < n; i++) {
                ll total = val + a[i];
                int nr = (int)(total % modulus);
                if (cheapest[nr] > total) {
                    cheapest[nr] = total;
                    q.push({total, nr});
                }
            }
        }
    }

    bool can(ll k) { return k >= 0 && cheapest[k % modulus] <= k; } // is k reachable

    ll count(ll l, ll r) { // how many k with l <= k <= r are reachable
        if (r < 0) return 0;
        l = max(l, 0LL);
        auto up_to = [&](ll limit) { // reachable totals in [0, limit]
            if (limit < 0) return (ll)0;
            ll total = 0;
            for (int i = 0; i < modulus; i++)
                if (cheapest[i] <= limit) total += (limit - cheapest[i]) / modulus + 1;
            return total;
        };
        return up_to(r) - up_to(l - 1);
    }

    ll smallest_unreachable() { // the largest gap matters for Frobenius-style questions
        for (ll k = 0;; k++)
            if (!can(k)) return k;
    }

    ll largest_unreachable() { // -1 when every total is reachable from some point on and none is missing
        ll worst = -1;
        for (int i = 0; i < modulus; i++)
            if (cheapest[i] < INF) worst = max(worst, cheapest[i] - modulus);
            else return -1; // this residue is never reachable, so infinitely many totals are missing
        return worst;
    }
};

// Standard problem: n coin values and a range - report how many totals in [l, r] can be made exactly
void solve() {
    int n;
    ll l, r;
    cin >> n >> l >> r;
    vector<ll> a(n);
    for (ll &v : a) cin >> v;
    LDE_Non_Negative lde(a);
    cout << lde.count(l, r) << '\n';
}
