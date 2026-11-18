// Test for 08_Game_Theory/Games_On_Graph.cpp against explicit depth-bounded minimax.
// Oracle: W_k(v) = some move reaches L_{k-1}, L_k(v) = every move reaches W_{k-1} (no moves = L_0).
// A position is winning / losing iff W_k / L_k holds for some k <= 2n+2, otherwise it is a draw.
// Checked: add_move, run, winning, losing, draw; plus the samples of Codeforces 786A (Berzerk).
#include "../../08_Game_Theory/Games_On_Graph.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(786);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

// returns 1 win, -1 lose, 0 draw for every position
vector<int> brute(int n, const vector<pair<int, int>> &es) {
    vector<vector<int>> out(n);
    for (auto [u, v] : es) out[u].push_back(v);
    vector<char> W(n, 0), L(n, 0);
    for (int k = 0; k <= 2 * n + 2; k++) {
        vector<char> nW(n), nL(n);
        for (int v = 0; v < n; v++) {
            bool any = false, all = true;
            for (int u : out[v]) any |= (bool)L[u], all &= (bool)W[u];
            nW[v] = W[v] || any;
            nL[v] = L[v] || all;
        }
        W = nW, L = nL;
    }
    vector<int> r(n);
    for (int v = 0; v < n; v++) r[v] = W[v] ? 1 : L[v] ? -1 : 0;
    return r;
}

// Codeforces 786A: circle of n, black hole at 1; output for Rick first and Morty first
vector<string> berzerk(int n, vector<vector<int>> s) {
    Games_On_Graph g(2 * n); // node p*n + pos: player p to move, monster at pos (0-based, 0 = hole)
    for (int p = 0; p < 2; p++)
        for (int pos = 1; pos < n; pos++)
            for (int x : s[p]) g.add_move(p * n + pos, (1 - p) * n + (pos + x) % n);
    g.run();
    vector<string> res(2);
    for (int p = 0; p < 2; p++)
        for (int pos = 1; pos < n; pos++) {
            int v = p * n + pos;
            res[p] += string(g.winning[v] ? "Win" : g.losing[v] ? "Lose" : "Loop") +
                      (pos + 1 < n ? " " : "");
        }
    return res;
}

int main() {
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 9 + 1, m = rng() % (3 * n + 1);
        vector<pair<int, int>> es;
        bool dag = rng() % 3 == 0;
        for (int i = 0; i < m; i++) {
            int u = rng() % n, v = rng() % n;
            if (dag && u == v) continue;
            if (dag && u > v) swap(u, v);
            es.push_back({u, v});
            if (rng() % 8 == 0) es.push_back({u, v}); // multi-edge
        }
        Games_On_Graph g(n);
        for (auto [u, v] : es) g.add_move(u, v);
        chk("add_move", true);
        g.run();
        auto r = brute(n, es);
        bool okw = true, okl = true, okd = true, okx = true;
        for (int v = 0; v < n; v++) {
            okw &= g.winning[v] == (r[v] == 1);
            okl &= g.losing[v] == (r[v] == -1);
            okd &= g.draw(v) == (r[v] == 0);
            okx &= !(g.winning[v] && g.losing[v]);
        }
        chk("run", okx);
        chk("winning", okw);
        chk("losing", okl);
        chk("draw", okd);
        if (dag) chk("dag:no_draw", count(r.begin(), r.end(), 0) == 0 && okd);
    }
    {   // edge cases: single terminal node, single self-loop, 2-cycle
        Games_On_Graph a(1);
        a.run();
        chk("edge:n=1 terminal", a.losing[0] && !a.winning[0]);
        Games_On_Graph b(1);
        b.add_move(0, 0);
        b.run();
        chk("edge:self-loop", b.draw(0));
        Games_On_Graph c(2);
        c.add_move(0, 1), c.add_move(1, 0);
        c.run();
        chk("edge:2-cycle", c.draw(0) && c.draw(1));
    }
    {   // Codeforces 786A samples
        auto r1 = berzerk(5, {{3, 2}, {1, 2, 3}});
        chk("samples", r1[0] == "Lose Win Win Loop" && r1[1] == "Loop Win Win Win");
    }
    {   // timing: 2e5 positions, 1e6 random moves, plus a 2e5-long chain (deep recursion)
        int n = 200000;
        Games_On_Graph g(n);
        for (int v = 0; v + 1 < n; v++) g.add_move(v, v + 1);
        for (int i = 0; i < 1000000; i++) {
            int u = rng() % n, v = rng() % n;
            if (u < v) g.add_move(u, v); // keep a DAG so the whole chain is resolved
        }
        auto t0 = clock();
        g.run();
        int dr = 0;
        for (int v = 0; v < n; v++) dr += g.draw(v);
        printf("n=2e5, m=~7e5 (DAG with 2e5 chain): %.3fs, draws=%d\n",
               double(clock() - t0) / CLOCKS_PER_SEC, dr);
        chk("large:dag has no draw", dr == 0);
    }
    {   // timing: 2e5 positions, 1e6 random moves with cycles
        int n = 200000;
        Games_On_Graph g(n);
        for (int i = 0; i < 1000000; i++) g.add_move(rng() % n, rng() % n);
        auto t0 = clock();
        g.run();
        int w = 0, l = 0;
        for (int v = 0; v < n; v++) w += g.winning[v], l += g.losing[v];
        printf("n=2e5, m=1e6 (random, cyclic): %.3fs, win=%d lose=%d draw=%d\n",
               double(clock() - t0) / CLOCKS_PER_SEC, w, l, n - w - l);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
