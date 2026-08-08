#include "../../core.h"

/*
 * Topic: DP - Slope Trick
 * Description: Slope Trick optimization for DP transitions involving convex piecewise linear functions.
 * Input: Convex/concave functions added iteratively.
 * Output: The minimum/maximum evaluated value across states.
 */

// Problem: Make array non-decreasing with minimum sum of |a[i] - b[i]|
// To make it strictly increasing, first apply: a[i] -= i for all i.
long long slope_trick_non_decreasing(const vector<long long>& a) {
    long long cost = 0;
    priority_queue<long long> pq; // max heap to store the slope change points

    for (int i = 0; i < a.size(); i++) {
        pq.push(a[i]);
        // If the max slope change point is greater than the current element,
        // it means our optimal function shifts, and we incur a cost.
        if (pq.top() > a[i]) {
            cost += pq.top() - a[i];
            pq.pop();
            pq.push(a[i]); // Push twice because slope decreases by 1 then increases by 1
        }
    }
    return cost;
}

int32_t main() {
    // Fast I/O
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n;
    if (cin >> n) {
        vector<long long> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
            // If strictly increasing is required, uncomment the following line:
            // a[i] -= i; 
        }
        cout << slope_trick_non_decreasing(a) << '\n';
    }
    return 0;
}