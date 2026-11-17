// Test for 03_Trees/Diameter.cpp against all-pairs BFS distances.
// Every public method is checked: add_edge, dfs (through dist / parent), farthest, diameter, path.
#include "../../03_Trees/Diameter.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(11);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

vector<int> bfs(const vector<vector<int>> &g, int s) {
    vector<int> d(g.size(), -1);
    queue<int> q;
    d[s] = 0, q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u])
            if (d[v] < 0) d[v] = d[u] + 1, q.push(v);
    }
    return d;
}

int main() {
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 12 + 1, shape = rng() % 4;
        Tree_Diameter T(n);
        vector<vector<int>> g(n + 1);
        vector<int> perm(n);
        iota(perm.begin(), perm.end(), 1);
        shuffle(perm.begin(), perm.end(), rng);
        for (int i = 1; i < n; i++) {
            int p = shape == 0 ? i - 1 : shape == 1 ? 0 : (int)(rng() % i); // path, star, random
            int u = perm[p], v = perm[i];
            if (rng() % 2) swap(u, v);
            T.add_edge(u, v), g[u].push_back(v), g[v].push_back(u);
        }
        vector<vector<int>> D(n + 1);
        int best = 0;
        for (int s = 1; s <= n; s++) {
            D[s] = bfs(g, s);
            for (int v = 1; v <= n; v++) best = max(best, D[s][v]);
        }
        int src = rng() % n + 1;
        int f = T.farthest(src);
        chk("farthest", D[src][f] == *max_element(D[src].begin() + 1, D[src].end()));
        bool ok = true;
        for (int v = 1; v <= n; v++) ok &= T.dist[v] == D[src][v];
        chk("dfs dist", ok);
        auto [len, a, b] = T.diameter();
        chk("diameter", len == best && D[a][b] == best);
        ok = T.parent[a] == -1;
        for (int v = 1; v <= n; v++) ok &= T.dist[v] == D[a][v];
        for (int v = 1; v <= n; v++)
            if (v != a) ok &= T.parent[v] >= 1 && D[a][T.parent[v]] == D[a][v] - 1;
        chk("dfs parent", ok);
        auto p = T.path();
        ok = (int)p.size() == best + 1 && set<int>(p.begin(), p.end()).size() == p.size();
        ok &= D[p.front()][p.back()] == best;
        for (int i = 0; ok && i + 1 < (int)p.size(); i++) ok &= D[p[i]][p[i + 1]] == 1;
        chk("path", ok);
    }
    for (int shape = 0; shape < 2; shape++) { // timing, random tree and a path (deep recursion)
        int n = 200000;
        Tree_Diameter T(n);
        for (int i = 2; i <= n; i++) T.add_edge(shape ? i - 1 : (int)(rng() % (i - 1)) + 1, i);
        auto t0 = clock();
        auto d = T.diameter();
        auto p = T.path();
        double sec = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("n=2e5 %s: %.3fs (len=%d)\n", shape ? "path" : "random", sec, d[0]);
        if (shape) chk("diameter", d[0] == n - 1 && (int)p.size() == n);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-12s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
