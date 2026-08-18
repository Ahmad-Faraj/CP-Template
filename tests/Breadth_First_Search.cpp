// Brute-force test for:
//   Graph/Breadth_First_Search.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace BF {
#include "../Graph/Breadth_First_Search.cpp"
}
int main() {
    mt19937_64 gen(4242);
    int plainFail = 0, zoFail = 0, multiFail = 0, pathFail = 0, gridFail = 0, checked = 0;

    for (int it = 0; it < 20000; it++) {
        int n = 1 + (int)(gen() % 7), m = (int)(gen() % 12);
        bool zeroOne = (it % 2 == 0);
        BF::Breadth_First_Search g(n);
        vector<array<int, 3>> es;
        for (int i = 0; i < m; i++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
            int w = zeroOne ? (int)(gen() % 2) : 1;
            g.add_edge(u, v, w);
            es.push_back({u, v, w});
        }
        int src = 1 + (int)(gen() % n);
        vector<int> got = g.run(src);
        checked++;
        // reference: Bellman-Ford style relaxation to a fixed point
        const int INF = BF::Breadth_First_Search::INF;
        vector<int> want(n + 1, INF);
        want[src] = 0;
        for (int r = 0; r <= n + 2; r++)
            for (auto &e : es)
                if (want[e[0]] < INF) want[e[1]] = min(want[e[1]], want[e[0]] + e[2]);
        for (int v = 1; v <= n; v++)
            if (got[v] != want[v]) (zeroOne ? zoFail : plainFail)++;

        // path must be a real walk of the claimed length
        for (int t = 1; t <= n; t++) {
            vector<int> p = g.path(src, t);
            if (want[t] >= INF) { if (!p.empty()) pathFail++; continue; }
            if (p.empty() || p.front() != src || p.back() != t) { pathFail++; continue; }
            int total = 0;
            bool ok = true;
            for (size_t i = 0; i + 1 < p.size(); i++) {
                int best = INF;
                for (auto &e : es) if (e[0] == p[i] && e[1] == p[i + 1]) best = min(best, e[2]);
                if (best >= INF) { ok = false; break; }
                total += best;
            }
            if (!ok || total != want[t]) pathFail++;
        }

        // multi-source == min over single-source runs
        vector<int> srcs;
        for (int v = 1; v <= n; v++) if (gen() % 2) srcs.push_back(v);
        if (!srcs.empty()) {
            vector<int> gotM = g.run(srcs);
            for (int t = 1; t <= n; t++) {
                int wantM = INF;
                for (int s : srcs) wantM = min(wantM, g.run(s)[t]);
                if (gotM[t] != wantM) multiFail++;
            }
        }
    }

    // grid BFS against a plain queue flood fill
    for (int it = 0; it < 3000; it++) {
        int R = 1 + (int)(gen() % 6), C = 1 + (int)(gen() % 6);
        vector<string> grid(R, string(C, '.'));
        for (int r = 0; r < R; r++) for (int c = 0; c < C; c++) if (gen() % 4 == 0) grid[r][c] = '#';
        int sr = (int)(gen() % R), sc = (int)(gen() % C);
        if (grid[sr][sc] == '#') continue;
        auto got = BF::grid_bfs(grid, {{sr, sc}});
        const int INF = numeric_limits<int>::max() / 4;
        vector<vector<int>> want(R, vector<int>(C, INF));
        queue<pair<int,int>> q; want[sr][sc] = 0; q.push({sr, sc});
        int dr[] = {-1,1,0,0}, dc[] = {0,0,-1,1};
        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();
            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr < 0 || nr >= R || nc < 0 || nc >= C || grid[nr][nc] == '#') continue;
                if (want[nr][nc] > want[r][c] + 1) { want[nr][nc] = want[r][c] + 1; q.push({nr, nc}); }
            }
        }
        for (int r = 0; r < R; r++) for (int c = 0; c < C; c++) if (got[r][c] != want[r][c]) gridFail++;
    }

    printf("Breadth_First_Search: %d random graphs\n", checked);
    printf("  unweighted vs relaxation      : %d failures\n", plainFail);
    printf("  0/1 weights vs relaxation     : %d failures\n", zoFail);
    printf("  multi-source == min of singles: %d failures\n", multiFail);
    printf("  path is a real walk           : %d failures\n", pathFail);
    printf("  grid BFS vs flood fill        : %d failures\n", gridFail);
}
