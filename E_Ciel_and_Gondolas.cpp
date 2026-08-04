#include <bits/stdc++.h>
using namespace std;

const int MAXN = 4010;
const int INF = 1e9 + 7;

/*
 * Topic: DP - Divide and Conquer Optimization
 * Description: Divide and Conquer Optimization reduces the time complexity of DP transitions of
 *   the form dp[i][j] = min(dp[i-1][k] + cost(k, j)) from O(K N^2) to O(K N log N)
 *   when the cost function satisfies the quadrangle inequality.
 */

int n, k;
int c[MAXN][MAXN];
int dp_before[MAXN], dp_curr[MAXN];


// O(1) - Evaluates cost of interval [left, right] using 2D prefix sums
inline int get_cost(int left, int right) {
    if (left > right) return 0;
    return c[right][right] - c[left - 1][right] - c[right][left - 1] + c[left - 1][left - 1];
}

// O(N log N) per layer
void compute(int left, int right, int opt_left, int opt_right) {
    if (left > right) return;
    int mid = left + (right - left) / 2;

    int best_cost = INF;
    int opt_k = -1;

    // k represents the start of the current gondola segment [k, mid].
    // dp_before[k - 1] represents the optimal cost of grouping people 1 to k-1 in previous gondolas.
    for (int k = opt_left; k <= min(mid, opt_right); k++) {
        int current_cost = (k > 1 ? dp_before[k - 1] : 0) + get_cost(k, mid);
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

// O(K * N log N)
int solve(int total_layers) {
    // 1-based indexing for DP layers and elements
    for (int i = 1; i <= n; i++) dp_before[i] = get_cost(1, i);
    
    // i represents the current gondola we are filling
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

    cin >> n >> k ;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            int val ; cin >> val ;
            c[i][j] = val + c[i - 1][j] + c[i][j - 1] - c[i - 1][j - 1];
        }
    }

    int min_unfamiliarity = solve(k);

    cout << min_unfamiliarity / 2 << "\n";

    return 0;
}