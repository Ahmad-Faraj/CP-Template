// Test for 04_Graphs/Dynamic_Connectivity.cpp against recomputing components with a fresh DSU
// after every time step. Every public method is checked: add_edge, remove_edge (multi-edges, self
// loops), add_query with and without a pair, run(), comps[] at every time and same[] at queries,
// both with one operation per time step and with several operations sharing a time.
#include "../../04_Graphs/Dynamic_Connectivity.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(31);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

struct Brute {
    vector<int> p;
    Brute(int n) : p(n + 1) { iota(p.begin(), p.end(), 0); }
    int f(int x) { return p[x] == x ? x : p[x] = f(p[x]); }
};

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 6 + 1, T = rng() % 30 + 1;
        bool shared = rng() % 3 == 0; // several operations may share one time step
        Dynamic_Connectivity D(n, T);
        multiset<pair<int, int>> cur;
        vector<multiset<pair<int, int>>> at(T + 1);
        vector<pair<int, int>> q(T + 1, {-1, -1});
        int t = 1;
        while (t <= T) {
            int ty = rng() % 3;
            if (ty == 0) {
                int u = rng() % n + 1, v = rng() % n + 1;
                D.add_edge(t, u, v), cur.insert({min(u, v), max(u, v)});
            } else if (ty == 1 && !cur.empty()) {
                auto e = next(cur.begin(), rng() % cur.size());
                if (rng() % 2) D.remove_edge(t, e->first, e->second);
                else D.remove_edge(t, e->second, e->first);
                cur.erase(e);
            } else {
                int u = rng() % 2 ? 0 : (int)(rng() % n + 1), v = rng() % n + 1;
                D.add_query(t, u, v), q[t] = {u, v};
            }
            at[t] = cur;
            if (!shared || rng() % 2) t++;
        }
        D.run();
        string tag = shared ? " (shared times)" : "";
        bool okc = true;
        for (int s = 1; s <= T; s++) {
            Brute b(n);
            int c = n;
            for (auto [u, v] : at[s])
                if (b.f(u) != b.f(v)) b.p[b.f(u)] = b.f(v), c--;
            okc &= D.comps[s] == c;
            bool conn = q[s].first > 0 && b.f(q[s].first) == b.f(q[s].second);
            if (q[s].first > 0) chk("same" + tag, D.same[s] == conn);
            else if (q[s].first == 0) chk("comps, query u=0" + tag, D.comps[s] == c && !D.same[s]);
        }
        chk("comps every time" + tag, okc);
    }
    for (int k = 0; k < 2; k++) { // timing: n = T = 3e5 (CF gym 100551A size); k = 1 is a star
        int n = 300000, T = 300000;
        Dynamic_Connectivity D(n, T);
        vector<pair<int, int>> live;
        for (int t = 1; t <= T; t++) {
            int ty = rng() % 3;
            if (ty == 0 || live.empty()) {
                int u = k ? 1 : (int)(rng() % n + 1), v = rng() % n + 1;
                D.add_edge(t, u, v), live.push_back({u, v});
            } else if (ty == 1) {
                int i = rng() % live.size();
                swap(live[i], live.back());
                D.remove_edge(t, live.back().first, live.back().second), live.pop_back();
            } else
                D.add_query(t, rng() % n + 1, rng() % n + 1);
        }
        auto t0 = clock();
        D.run();
        double sec = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("n=T=3e5 %s: %.3fs (comps[T]=%d)\n", k ? "star" : "random", sec, D.comps[T]);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-38s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
