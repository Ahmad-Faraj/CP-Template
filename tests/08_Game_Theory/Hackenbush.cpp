// Test for 08_Game_Theory/Hackenbush.cpp against an exhaustive game search.
// Oracle: a position is the set of remaining edges (bitmask); a move deletes one edge and then
// every edge no longer connected to ground; Grundy = mex over moves, memoised over all 2^m edge sets.
// Every public method is checked: set_ground (0, 1 or several ground nodes), add_edge (self loops,
// multi-edges, edges between ground nodes), grundy (also called twice), first_player_wins.
#include "../../08_Game_Theory/Hackenbush.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(31);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int brute(int n, const vector<pair<int, int>> &es, const vector<int> &ground) {
    int m = es.size();
    auto prune = [&](int mask) { // keep only edges reachable from ground through mask
        vector<char> seen(n + 1, 0);
        vector<int> st;
        for (int g : ground) seen[g] = 1, st.push_back(g);
        while (!st.empty()) {
            int u = st.back();
            st.pop_back();
            for (int i = 0; i < m; i++)
                if (mask >> i & 1) {
                    auto [a, b] = es[i];
                    int o = a == u ? b : b == u ? a : -1;
                    if (o != -1 && !seen[o]) seen[o] = 1, st.push_back(o);
                }
        }
        int res = 0;
        for (int i = 0; i < m; i++)
            if ((mask >> i & 1) && seen[es[i].first]) res |= 1 << i;
        return res;
    };
    vector<int> g(1 << m, -1);
    for (int mask = 0; mask < (1 << m); mask++) { // submasks are smaller, so already done
        if (prune(mask) != mask) continue;
        set<int> opts;
        for (int i = 0; i < m; i++)
            if (mask >> i & 1) opts.insert(g[prune(mask ^ (1 << i))]);
        int x = 0;
        while (opts.count(x)) x++;
        g[mask] = x;
    }
    return g[prune((1 << m) - 1)];
}

int main() {
    for (int it = 0; it < 12000; it++) {
        int n = rng() % 6 + 1, m = rng() % 12;
        int shape = rng() % 3; // 0 any graph, 1 tree-ish (edges to smaller index), 2 dense loops
        vector<pair<int, int>> es;
        for (int i = 0; i < m; i++) {
            int u = rng() % n + 1, v = rng() % n + 1;
            if (shape == 1 && u > 1) v = rng() % (u - 1) + 1;
            if (shape == 2 && rng() % 2) v = u;
            es.push_back({u, v});
        }
        vector<int> ground;
        int gmode = rng() % 3; // 0 default (node 1), 1 one random node, 2 several
        Hackenbush h(n);
        if (gmode == 0) ground = {1};
        else {
            int k = gmode == 1 ? 1 : rng() % n + 1;
            for (int i = 0; i < k; i++) ground.push_back(rng() % n + 1);
            for (int g : ground) h.set_ground(g);
        }
        for (auto [u, v] : es) h.add_edge(u, v);
        int want = brute(n, es, ground);
        string t = gmode == 0 ? "default ground:" : gmode == 1 ? "one ground:" : "many ground:";
        int got = h.grundy();
        chk(t + "grundy", got == want);
        chk("grundy (second call)", h.grundy() == want);
        chk("first_player_wins", h.first_player_wins() == (want != 0));
        if (it < 5 && got != want) printf("n=%d m=%d want %d got %d\n", n, m, want, got);
    }
    { // hand cases: a loop above a bridge, and a known stalk
        Hackenbush h(2);
        h.set_ground(1), h.add_edge(1, 2), h.add_edge(2, 2);
        chk("hand: loop at deep node", h.grundy() == 2);
        Hackenbush p(4);
        p.add_edge(1, 2), p.add_edge(2, 3), p.add_edge(3, 4);
        chk("hand: stalk of 3", p.grundy() == 3);
        Hackenbush e(1);
        chk("hand: empty", e.grundy() == 0 && !e.first_player_wins());
    }
    {
        int n = 300000;
        Hackenbush h(n);
        h.set_ground(1);
        for (int i = 2; i <= n; i++) h.add_edge(i - 1, i); // long path: deep recursion
        for (int i = 0; i < 300000; i++) h.add_edge(rng() % n + 1, rng() % n + 1);
        auto t0 = clock();
        int g = h.grundy();
        double sec = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("path + random n=3e5 m=6e5: %.3fs (grundy=%d)\n", sec, g);
        Hackenbush p(n);
        for (int i = 2; i <= n; i++) p.add_edge(i - 1, i);
        chk("long path n=3e5", p.grundy() == n - 1);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
