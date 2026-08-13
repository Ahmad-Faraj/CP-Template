#include "../../../core.h"
/*
 * Topic: Functional Graph (Successor Graph)
 * Description: Handles graphs with out-degree 1. Fast cycle finding and K-th successor queries.
 * 
 * Important Facts:
 * - 0-based indexing for nodes by default.
 * - Time Complexity: O(V log K)
 * - Space Complexity: O(V log K)
 */
#include <vector>
#include <algorithm>
#include <utility>

struct FunctionalGraph {
    int n;
    int max_log;
    vector<vector<int>> up;
    vector<int> f;

    // Time Complexity: O(N log K)
    // Space Complexity: O(N log K)
    // FunctionalGraph fg(n, k_max_log);
    FunctionalGraph(int n_nodes, int k_max_log) : n(n_nodes), max_log(k_max_log), up(n_nodes, vector<int>(k_max_log, -1)) {}

    // Time Complexity: O(N log K)
    // Space Complexity: O(1)
    // fg.build(next_node);
    void build(const vector<int>& next_node) {
        f = next_node;
        for (int i = 0; i < n; i++) {
            up[i][0] = next_node[i];
        }
        for (int j = 1; j < max_log; j++) {
            for (int i = 0; i < n; i++) {
                int nxt = up[i][j - 1];
                if (nxt != -1) {
                    up[i][j] = up[nxt][j - 1];
                }
            }
        }
    }




    // int res = fg.kth_successor(u, k);
    int kth_successor(int u, long long k) {
        for (int j = max_log - 1; j >= 0; j--) {
            if (((k >> j) & 1) != 0) {
                u = up[u][j];
                if (u == -1) break;
            }
        }
        return u;
    }

    // auto cycle = fg.find_cycle_floyd(start);
    pair<int, int> find_cycle_floyd(int start) {
        if (f.empty() || f[start] == -1) return {-1, -1};
        int tortoise = f[start];
        int hare = f[start];
        if (f[hare] == -1) return {-1, -1};
        hare = f[f[hare]];

        while (tortoise != hare) {
            if (hare == -1 || f[hare] == -1) return {-1, -1};
            tortoise = f[tortoise];
            hare = f[f[hare]];
        }
        int mu = 0;
        tortoise = start;
        while (tortoise != hare) {
            tortoise = f[tortoise];
            hare = f[hare];
            mu++;
        }
        int lambda = 1;
        hare = f[tortoise];
        while (tortoise != hare) {
            hare = f[hare];
            lambda++;
        }
        return {mu, lambda};
    }

};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, q;
    if (cin >> n >> q) {
        vector<int> f(n);
        for (int i = 0; i < n; i++) {
            cin >> f[i];
            f[i]--;
        }
        FunctionalGraph fg(n, 31);
        fg.build(f);
        for (int i = 0; i < q; i++) {
            int x;
            long long k;
            cin >> x >> k;
            cout << fg.kth_successor(x - 1, k) + 1 << "\n";
        }
    }
    return 0;
}
