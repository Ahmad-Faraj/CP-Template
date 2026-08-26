#include "../../../core.h"

/*
 * Topic: Hungarian Algorithm
 * Description: Finds minimum or maximum weight perfect matching in a bipartite graph.
 *
 * Important Facts:
 * - 1-based indexing for nodes by default.
 * - Time Complexity: O(V^3)
 * - Space Complexity: O(V^2)
 */

struct Hungarian {
    static const int N = 505;
    long long c[N][N], fx[N], fy[N], d[N];
    int l[N], r[N], arg[N], trace[N];
    queue<int> q;
    int start, finish, n;

    // Default constructor.
    Hungarian() {}

    // Initializes the Hungarian structure with maximum dimension.
    Hungarian(int n1, int n2) : n(max(n1, n2)) {
        for (int i = 1; i <= n; ++i) {
            fy[i] = l[i] = r[i] = 0;
            for (int j = 1; j <= n; ++j) c[i][j] = inf;
        }
    }

    // Updates the edge cost between u and v with minimum.
    void add_edge(int u, int v, long long cost) { c[u][v] = min(c[u][v], cost); }

    inline long long getC(int u, int v) { return c[u][v] - fx[u] - fy[v]; }

    void initBFS() {
        while (!q.empty()) q.pop();
        q.push(start);
        for (int i = 0; i <= n; ++i) trace[i] = 0;
        for (int v = 1; v <= n; ++v) {
            d[v] = getC(start, v);
            arg[v] = start;
        }
        finish = 0;
    }

    void findAugPath() {
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v = 1; v <= n; ++v)
                if (!trace[v]) {
                    long long w = getC(u, v);
                    if (!w) {
                        trace[v] = u;
                        if (!r[v]) {
                            finish = v;
                            return;
                        }
                        q.push(r[v]);
                    }
                    if (d[v] > w) {
                        d[v] = w;
                        arg[v] = u;
                    }
                }
        }
    }

    void subX_addY() {
        long long delta = inf;
        for (int v = 1; v <= n; ++v)
            if (trace[v] == 0 && d[v] < delta) {
                delta = d[v];
            }
        fx[start] += delta;
        for (int v = 1; v <= n; ++v)
            if (trace[v]) {
                int u = r[v];
                fy[v] -= delta;
                fx[u] += delta;
            } else
                d[v] -= delta;
        for (int v = 1; v <= n; ++v)
            if (!trace[v] && !d[v]) {
                trace[v] = arg[v];
                if (!r[v]) {
                    finish = v;
                    return;
                }
                q.push(r[v]);
            }
    }

    void Enlarge() {
        do {
            int u = trace[finish];
            int nxt = l[u];
            l[u] = finish;
            r[finish] = u;
            finish = nxt;
        } while (finish);
    }

    long long maximum_matching() {
        for (int u = 1; u <= n; ++u) {
            fx[u] = c[u][1];
            for (int v = 1; v <= n; ++v) {
                fx[u] = min(fx[u], c[u][v]);
            }
        }
        for (int v = 1; v <= n; ++v) {
            fy[v] = c[1][v] - fx[1];
            for (int u = 1; u <= n; ++u) {
                fy[v] = min(fy[v], c[u][v] - fx[u]);
            }
        }
        for (int u = 1; u <= n; ++u) {
            start = u;
            initBFS();
            while (!finish) {
                findAugPath();
                if (!finish) subX_addY();
            }
            Enlarge();
        }
        long long ans = 0;
        for (int i = 1; i <= n; ++i) {
            if (c[i][l[i]] != inf)
                ans += c[i][l[i]];
            else
                l[i] = 0;
        }
        return ans;
    }

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Retrieves the final assignment pairs (1-indexed)
    vector<pair<int, int>> get_assignment() {
        vector<pair<int, int>> assignment;
        for (int i = 1; i <= n; ++i) {
            if (l[i] != 0 && c[i][l[i]] != inf) {
                assignment.push_back({i, l[i]});
            }
        }
        return assignment;
    }
};

/*
 * Takes n (workers and tasks) and an n x n cost matrix.
 * Gives minimum total cost and the optimal worker-task assignments.
 */
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    if (cin >> n) {
        Hungarian M(n, n);
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                long long cost;
                cin >> cost;
                M.add_edge(i, j, cost);
            }
        }
        long long total = M.maximum_matching();
        cout << total << "\n";
        auto assignment = M.get_assignment();
        for (auto [u, v] : assignment) {
            cout << u << " " << v << "\n";
        }
    }
    return 0;
}