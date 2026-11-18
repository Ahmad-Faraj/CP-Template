// Test for 04_Graphs/Min_Cost_Max_Flow.cpp against exhaustive enumeration of every integer flow
// on tiny graphs (each edge carries 0..cap, keep the conserved ones): min cost for each flow value.
// Every public method is checked: add_edge (directed, undirected, with/without id, negative costs),
// solve(s, t) and solve(s, t, goal) (flow and cost), flow_through (feasible, conserved, costs add up).
#include "../../04_Graphs/Min_Cost_Max_Flow.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(29);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

struct E { int u, v; long long cap, cost; };
const long long NONE = LLONG_MAX;

// best[k] = min cost of a feasible s-t flow of value k (NONE if impossible)
vector<long long> brute(int n, const vector<E> &es, int s, int t) {
    int m = es.size();
    vector<long long> best;
    vector<long long> f(m, 0);
    while (true) {
        vector<long long> net(n, 0);
        long long c = 0;
        for (int i = 0; i < m; i++) net[es[i].u] -= f[i], net[es[i].v] += f[i], c += f[i] * es[i].cost;
        bool ok = true;
        for (int v = 0; v < n; v++)
            if (v != s && v != t) ok &= net[v] == 0;
        if (ok && net[t] >= 0) {
            long long k = net[t];
            if ((long long)best.size() <= k) best.resize(k + 1, NONE);
            best[k] = min(best[k], c);
        }
        int i = 0;
        while (i < m && f[i] == es[i].cap) f[i++] = 0;
        if (i == m) break;
        f[i]++;
    }
    return best;
}

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 5 + 2, m = rng() % 7;
        // 0: cost >= 0 any graph, 1: negative costs on a DAG, 2: undirected,
        // 3: negative costs on any graph without a negative cycle (others skipped)
        int mode = rng() % 4;
        if (mode == 2) m = rng() % 5;
        long long capmax = m <= 5 && mode != 2 ? 3 : 2;
        MCMF F(n);
        vector<E> es;    // arcs for the brute force
        vector<int> dir; // index in es of each directed id
        for (int i = 0; i < m; i++) {
            int u = rng() % n, v = rng() % n;
            if (mode == 1) {
                while (u == v) v = rng() % n;
                if (u > v) swap(u, v);
            }
            long long cap = rng() % (capmax + 1);
            long long cost = mode % 2 ? (long long)(rng() % 21) - 10 : (long long)(rng() % 11);
            if (mode == 2) {
                F.add_edge(u, v, cap, cost, -1, false);
                es.push_back({u, v, cap, cost}), es.push_back({v, u, cap, cost});
            } else {
                F.add_edge(u, v, cap, cost, i);
                es.push_back({u, v, cap, cost});
            }
        }
        if (mode == 3) { // Floyd-Warshall over arcs with cap > 0: skip negative cycles
            vector<vector<long long>> D(n, vector<long long>(n, NONE / 4));
            for (int v = 0; v < n; v++) D[v][v] = 0;
            for (auto &e : es)
                if (e.cap > 0) D[e.u][e.v] = min(D[e.u][e.v], e.cost);
            for (int k = 0; k < n; k++)
                for (int i = 0; i < n; i++)
                    for (int j = 0; j < n; j++) D[i][j] = min(D[i][j], D[i][k] + D[k][j]);
            bool negcyc = false;
            for (int v = 0; v < n; v++) negcyc |= D[v][v] < 0;
            if (negcyc) continue;
        }
        int s = rng() % n, t = (s + 1 + rng() % (n - 1)) % n;
        if (mode == 1 && s > t) swap(s, t);
        auto best = brute(n, es, s, t);
        long long maxf = 0;
        for (long long k = 0; k < (long long)best.size(); k++)
            if (best[k] != NONE) maxf = k;
        string tag = vector<string>{"cost>=0", "neg cost DAG", "undirected", "neg cost cyclic"}[mode];
        bool capped = rng() % 2;
        long long goal = capped ? rng() % (maxf + 2) : MCMF::inf;
        auto [fl, co] = capped ? F.solve(s, t, goal) : F.solve(s, t);
        long long wantf = min(goal, maxf);
        chk(string(capped ? "solve(s,t,goal) " : "solve(s,t) ") + tag, fl == wantf && co == best[wantf]);
        if (mode != 2) {
            bool ok = true;
            vector<long long> net(n, 0);
            long long c = 0;
            for (int i = 0; i < m; i++) {
                long long f = F.flow_through[i];
                ok &= 0 <= f && f <= es[i].cap;
                net[es[i].u] -= f, net[es[i].v] += f, c += f * es[i].cost;
            }
            for (int v = 0; v < n; v++)
                if (v != s && v != t) ok &= net[v] == 0;
            ok &= net[t] == fl && c == co;
            chk("flow_through " + tag, ok);
        }
    }
    // sample: Library Checker assignment example_00 as MCMF (answer 9)
    {
        vector<vector<long long>> a = {{4, 3, 5}, {3, 5, 9}, {4, 1, 4}};
        int n = 3;
        MCMF F(2 * n + 2);
        int S = 2 * n, T = S + 1;
        for (int i = 0; i < n; i++) {
            F.add_edge(S, i, 1, 0), F.add_edge(n + i, T, 1, 0);
            for (int j = 0; j < n; j++) F.add_edge(i, n + j, 1, a[i][j], i * n + j);
        }
        auto [fl, co] = F.solve(S, T);
        long long sum = 0;
        for (int i = 0; i < n * n; i++) sum += F.flow_through[i] * a[i / n][i % n];
        chk("samples", fl == 3 && co == 9 && sum == 9);
    }
    {
        int n = 300;
        MCMF F(2 * n + 2);
        int S = 2 * n, T = S + 1;
        for (int i = 0; i < n; i++) {
            F.add_edge(S, i, 1, 0), F.add_edge(n + i, T, 1, 0);
            for (int j = 0; j < n; j++)
                F.add_edge(i, n + j, 1, (long long)(rng() % 2000000001) - 1000000000);
        }
        auto t0 = clock();
        auto [fl, co] = F.solve(S, T);
        printf("assignment n=300 (9e4 edges, negative costs): %.3fs (flow=%lld cost=%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, fl, co);
        int N = 5000, M = 50000;
        MCMF G(N);
        for (int i = 0; i < M; i++) G.add_edge(rng() % N, rng() % N, rng() % 100 + 1, rng() % 1000);
        t0 = clock();
        tie(fl, co) = G.solve(0, N - 1);
        printf("random n=5000 m=5e4: %.3fs (flow=%lld cost=%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, fl, co);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-34s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
