// Hungarian: the cheapest perfect assignment of n workers to m jobs, given a full cost matrix.
// Use when: "assign every worker one job at least total cost", the assignment problem, min-cost perfect matching.
// Handles: rectangular matrices (pads to a square), negative costs, and maximum-cost assignment by negating.
// Time: O(max(n, m)^3)
// Indexing: 1-based; cost(i, j) for worker i and job j, both from 1
// Note: sized by vectors, not fixed arrays - a 505x505 long long matrix as a member was 2 MB on the stack.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Hungarian {
    static constexpr ll INF = numeric_limits<ll>::max() / 4;

    int n, rows, cols;
    vector<vector<ll>> cost;
    vector<int> assign_row, assign_col; // assign_row[i] = the job given to worker i, 0 if none

    Hungarian(int rows, int cols)
        : n(max(rows, cols)), rows(rows), cols(cols), cost(n + 1, vector<ll>(n + 1, 0)), assign_row(n + 1, 0),
          assign_col(n + 1, 0) {}

    void set_cost(int i, int j, ll c) { cost[i][j] = c; }

    // minimum total cost of a perfect assignment on the padded square; also fills assign_row
    ll minimum_cost() {
        vector<ll> u(n + 1, 0), v(n + 1, 0);
        vector<int> p(n + 1, 0), way(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            vector<ll> minv(n + 1, INF);
            vector<char> used(n + 1, false);
            p[0] = i;
            int j0 = 0;
            do {
                used[j0] = true;
                int i0 = p[j0], j1 = 0;
                ll delta = INF;
                for (int j = 1; j <= n; j++)
                    if (!used[j]) {
                        ll cur = cost[i0][j] - u[i0] - v[j];
                        if (cur < minv[j]) minv[j] = cur, way[j] = j0;
                        if (minv[j] < delta) delta = minv[j], j1 = j;
                    }
                for (int j = 0; j <= n; j++)
                    if (used[j])
                        u[p[j]] += delta, v[j] -= delta;
                    else
                        minv[j] -= delta;
                j0 = j1;
            } while (p[j0] != 0);
            do { // walk the alternating path back, reassigning as we go
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0);
        }
        fill(assign_row.begin(), assign_row.end(), 0);
        fill(assign_col.begin(), assign_col.end(), 0);
        ll total = 0;
        for (int j = 1; j <= n; j++)
            if (p[j] >= 1 && p[j] <= rows && j <= cols) {
                assign_row[p[j]] = j;
                assign_col[j] = p[j];
                total += cost[p[j]][j];
            }
        return total;
    }

    ll maximum_cost() { // negate, minimise, negate back
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) cost[i][j] = -cost[i][j];
        ll got = minimum_cost();
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) cost[i][j] = -cost[i][j];
        return -got;
    }

    vector<pair<int, int>> pairs() { // the assigned (worker, job) pairs
        vector<pair<int, int>> out;
        for (int i = 1; i <= rows; i++)
            if (assign_row[i]) out.push_back({i, assign_row[i]});
        return out;
    }
};

// Standard problem: an n x n cost matrix - report the cheapest perfect assignment and who takes what
void solve() {
    int n;
    cin >> n;
    Hungarian h(n, n);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) {
            ll c;
            cin >> c;
            h.set_cost(i, j, c);
        }
    cout << h.minimum_cost() << '\n';
    for (auto [i, j] : h.pairs()) cout << i << ' ' << j << '\n';
}
