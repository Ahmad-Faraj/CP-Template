// Test for 04_Graphs/Euler_Path.cpp against exhaustive backtracking over edge orders.
// Every public member is checked: add_edge (return id), run() and run(src), path, edge_path,
// directed and undirected, 0-/1-based nodes, self loops, multi-edges, m = 0, disconnected edges.
#include "../../04_Graphs/Euler_Path.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(23);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

struct Ed { int u, v; };
int M;
vector<Ed> es;
bool dir_;
bool bt(int cur, int mask, int need_end) { // walk the unused edges from cur (end at need_end)?
    if (mask == (1 << M) - 1) return need_end == -1 || cur == need_end;
    for (int e = 0; e < M; e++)
        if (!(mask >> e & 1)) {
            if (es[e].u == cur && bt(es[e].v, mask | 1 << e, need_end)) return true;
            if (!dir_ && es[e].v == cur && bt(es[e].u, mask | 1 << e, need_end)) return true;
        }
    return false;
}

bool valid(const Euler_Path &E, int src) { // a genuine trail: each edge once, consecutive, from src
    if ((int)E.path.size() != M + 1 || (int)E.edge_path.size() != M) return false;
    if (src != -1 && E.path[0] != src) return false;
    vector<int> seen(M, 0);
    for (int i = 0; i < M; i++) {
        int e = E.edge_path[i], a = E.path[i], b = E.path[i + 1];
        if (e < 0 || e >= M || seen[e]++) return false;
        bool fw = es[e].u == a && es[e].v == b, bw = es[e].v == a && es[e].u == b;
        if (!(fw || (!dir_ && bw))) return false;
    }
    return true;
}

int main() {
    for (int it = 0; it < 40000; it++) {
        int n = rng() % 5 + 1;
        dir_ = rng() % 2;
        bool one = rng() % 2; // nodes 1..n or 0..n-1
        M = rng() % 8;
        if (it % 10 == 0) M = 0;
        es.clear();
        Euler_Path E(n, dir_);
        bool idok = true;
        for (int i = 0; i < M; i++) {
            int u = rng() % n + one, v = rng() % n + one;
            if (rng() % 6 == 0) v = u; // self loop
            if (i && rng() % 5 == 0) u = es[i - 1].u, v = es[i - 1].v; // multi-edge
            es.push_back({u, v});
            idok &= E.add_edge(u, v) == i;
        }
        chk("add_edge(id)", idok);
        string t = string(dir_ ? "directed:" : "undirected:") + (M ? "" : "m=0:");
        int lo = one, hi = n - 1 + one;
        bool any = false, closed = false;
        for (int s = lo; s <= hi; s++) any |= bt(s, 0, -1), closed |= bt(s, 0, s);
        if (M == 0) any = closed = true;
        bool r = E.run();
        chk(t + "run() exists", r == any);
        if (r && M) {
            chk(t + "run() valid trail", valid(E, -1));
            if (closed)
                chk(t + "run() closes when a circuit exists", E.path.front() == E.path.back());
        }
        if (r && !M) chk(t + "run() m=0 empty", E.path.empty() && E.edge_path.empty());
        for (int s = lo; s <= hi; s++) {
            bool want = M == 0 || bt(s, 0, -1);
            bool rs = E.run(s);
            chk(t + "run(src) exists", rs == want);
            if (rs) chk(t + "run(src) valid trail", M ? valid(E, s) : E.path == vector<int>{s});
            if (!rs) chk(t + "run(src) fail clears", E.path.empty() && E.edge_path.empty());
        }
    }
    for (int d = 0; d < 2; d++) { // large: random Euler circuit of m = 1e6 edges on n = 2e5 nodes
        int n = 200000, m = 1000000;
#ifdef __SANITIZE_ADDRESS__
        m = 200000;
#endif
        Euler_Path E(n, d);
        dir_ = d, M = m, es.clear();
        int cur = 1;
        for (int i = 0; i < m; i++) {
            int nx = i == m - 1 ? 1 : rng() % n + 1;
            E.add_edge(cur, nx), es.push_back({cur, nx}), cur = nx;
        }
        auto t0 = clock();
        bool r = E.run(1);
        printf("%s n=2e5 m=%d run: %.3fs\n", d ? "directed" : "undirected", m,
               double(clock() - t0) / CLOCKS_PER_SEC);
        chk("large circuit", r && valid(E, 1) && E.path.back() == 1);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-46s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
