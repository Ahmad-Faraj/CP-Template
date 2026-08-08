#include "../../../core.h"
/*
 * Algorithm: System of Difference Constraints
 * Purpose: Solves x_i - x_j <= C using Bellman-Ford on a constraint graph.
 * Complexity: Time O(V * E) | Space O(V + E)
 * Verified: ..........
 */
template <typename T = int>
struct DifferenceConstraints {
    struct Constraint {
        int u, v;
        T c;
    };

    int n;
    vector<Constraint> constraints;

    DifferenceConstraints(int _n) : n(_n) {}

    // X_u - X_v >= c
    void add_constraint(int u, int v, T c) { constraints.push_back({u, v, c}); }

    bool is_solvable() const {
        vector<T> dist(n, 0);
        int m = (int)(constraints).size();
        for (int i = 0; i < n; ++i) {
            bool updated = false;
            for (int j = 0; j < m; ++j) {
                int u = constraints[j].u;
                int v = constraints[j].v;
                T c = constraints[j].c;
                if (dist[u] < dist[v] + c) {
                    dist[u] = dist[v] + c;
                    updated = true;
                }
            }
            if (!updated) return true;
            if (i == n - 1) return false;
        }
        return true;
    }

    vector<T> get_any_values() const {
        vector<T> dist(n, 0);
        int m = (int)(constraints).size();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < m; ++j) {
                int u = constraints[j].u;
                int v = constraints[j].v;
                T c = constraints[j].c;
                dist[u] = max(dist[u], dist[v] + c);
            }
        }
        return dist;
    }

    vector<T> get_min_values() const { return get_any_values(); }

    vector<T> get_max_values() const {
        vector<T> dist(n, 0);
        int m = (int)(constraints).size();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < m; ++j) {
                int u = constraints[j].u;
                int v = constraints[j].v;
                T c = constraints[j].c;
                dist[v] = max(dist[v], dist[u] + c);
            }
        }
        vector<T> res(n);
        for (int i = 0; i < n; ++i) res[i] = -dist[i];
        return res;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        DifferenceConstraints<int> dc(n);
        for (int i = 0; i < m; i++) {
            int u, v, c;
            cin >> u >> v >> c;
            dc.add_constraint(u - 1, v - 1, c);
        }
        if (dc.is_solvable()) {
            cout << "YES\n";
            vector<int> res = dc.get_any_values();
            for (int i = 0; i < n; i++) {
                cout << res[i] << (i == n - 1 ? "" : " ");
            }
            cout << "\n";
        } else {
            cout << "NO\n";
        }
    }
    return 0;
}
