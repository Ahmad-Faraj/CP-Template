#include "../../core.h"

/*
 * Topic: DP - Divide and Conquer Optimization
 * Description: Divide and Conquer Optimization reduces the time complexity of DP transitions of
 *   the form dp[i][j] = min(dp[i-1][k-1] + cost(k, j)) from O(K N^2) to O(K N log N)
 *   when the cost function satisfies the quadrangle inequality.
 *
 * Basic Idea:
 *   If the optimal split point for dp[i][j] is opt(i, j), the quadrangle inequality
 *   guarantees that opt(i, j-1) <= opt(i, j) <= opt(i+1, j).
 *   Because of this monotonicity, if we compute dp[i][mid] for the middle element,
 *   we can restrict the search space for the left half to [opt_left, opt_mid] and
 *   the right half to [opt_mid, opt_right]. This divide and conquer approach reduces
 *   the transition time per layer to O(N log N).
 *
 * Basic Problem Implemented:
 *   Divide an array of N positive integers into K contiguous segments such that the sum
 *   of the squares of the sums of each segment is minimized.
 *   cost(l, r) = (pref[r] - pref[l-1])^2
 */

/*
 * Trick (Blackbox Testing): Check Quadrangle Inequality quickly:
 * bool is_qi() {
 *     // C(a,c) + C(b,d) <= C(a,d) + C(b,c) for a < b < c < d
 *     return get_cost(1, 3) + get_cost(2, 4) <= get_cost(1, 4) + get_cost(2, 3);
 * }
 */

const int MAXN = 4010;
int n, k;
ll pref[MAXN];
ll dp_before[MAXN], dp_curr[MAXN];

/*
 * Function: get_cost
 * Description: Evaluates the cost function C(left, right) in O(1) time.
 *              For Divide and Conquer to work, this cost function MUST satisfy the
 *              Quadrangle Inequality: C(a, c) + C(b, d) <= C(a, d) + C(b, c) for a <= b <= c <= d.
 * Parameters:
 *   - left: Starting index of the interval
 *   - right: Ending index of the interval
 * Returns: The cost of the interval [left, right]
 */
inline ll get_cost(int left, int right) {
    if (left > right) return 0;
    ll sum = pref[right] - pref[left - 1];
    return sum * sum;
}

/*
 * Function: compute
 * Description: Recursively computes the DP transitions for a specific layer.
 *              It finds the optimal split point (opt_k) for the midpoint `mid`
 *              by iterating only between the optimal split points of its boundaries
 *              (`opt_left` to `opt_right`). This monotonic property reduces
 *              the complexity from O(N^2) to O(N log N) per layer.
 * Parameters:
 *   - left: The left boundary of the state indices we are currently evaluating
 *   - right: The right boundary of the state indices we are currently evaluating
 *   - opt_left: The lowest possible optimal split point for the current range
 *   - opt_right: The highest possible optimal split point for the current range
 */
void compute(int left, int right, int opt_left, int opt_right) {
    if (left > right) return;
    int mid = left + (right - left) / 2;

    ll best_cost = INF;
    int opt_k = -1;

    // k represents the start of the current segment [k, mid].
    // dp_before[k - 1] represents the optimal cost of dividing elements 1 to k-1 into previous segments.
    for (int k = opt_left; k <= min(mid, opt_right); k++) {
        ll current_cost = (k > 1 ? dp_before[k - 1] : 0) + get_cost(k, mid);
        if (current_cost < best_cost) {
            best_cost = current_cost;
            opt_k = k;
        }
    }

    dp_curr[mid] = best_cost;

    // The optimal split point for the left half must be <= opt_k
    compute(left, mid - 1, opt_left, opt_k);

    // The optimal split point for the right half must be >= opt_k
    compute(mid + 1, right, opt_k, opt_right);
}

/*
 * Function: solve
 * Description: Manages the layer-by-layer computation of the DP.
 *              Since dp[layer][j] only depends on dp[layer-1][k], we use two 1D arrays
 *              (`dp_before` and `dp_curr`) to optimize memory to O(N).
 *              For each layer, it invokes `compute` to populate `dp_curr`.
 * Parameters:
 *   - total_layers: The number of partitions/layers (e.g. K segments)
 * Returns: The optimal DP value after computing all layers
 */
ll solve(int total_layers) {
    // 1-based indexing for DP layers and elements
    for (int i = 1; i <= n; i++) dp_before[i] = get_cost(1, i);

    // i represents the current segment we are forming
    for (int i = 2; i <= total_layers; i++) {
        compute(1, n, 1, n);

        // Swap dp_curr to dp_before for the next layer
        for (int j = 1; j <= n; j++) {
            dp_before[j] = dp_curr[j];
        }
    }
    return dp_before[n];
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    if (cin >> n >> k) {
        for (int i = 1; i <= n; i++) {
            ll val;
            cin >> val;
            pref[i] = pref[i - 1] + val;
        }

        cout << solve(k) << "\n";
    }

    return 0;
}