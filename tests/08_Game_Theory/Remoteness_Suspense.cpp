// Test for 08_Game_Theory/Remoteness_Suspense.cpp against explicit minimax over the game tree.
// Oracle: plain recursion over every line of play (no memo, no win flags reused), where the mover
// ranks outcomes: any win beats any loss; remoteness prefers short wins / long losses, suspense
// the opposite. Also checks the header's sum rules against brute force on product games:
// "move in every component, ends when ANY ends" (min remoteness odd) and "... when ALL end"
// (max suspense odd). Methods checked: add_move, is_winning, remoteness, suspense.
#include "../../08_Game_Theory/Remoteness_Suspense.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(5);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

using Adj = vector<vector<int>>;

// returns {mover wins, length}; mode 0 = remoteness rule, 1 = suspense rule
pair<bool, int> minimax(const Adj &g, int u, int mode) {
    if (g[u].empty()) return {false, 0};
    pair<bool, int> best{false, -1};
    auto better = [&](pair<bool, int> a, pair<bool, int> b) { // is a better than b for the mover
        if (b.second < 0) return true;
        if (a.first != b.first) return a.first;
        bool shorter = a.second < b.second;
        return a.first == (mode == 0) ? shorter : a.second > b.second;
    };
    for (int v : g[u]) {
        auto [w, len] = minimax(g, v, mode);
        pair<bool, int> c{!w, len + 1};
        if (better(c, best)) best = c;
    }
    return best;
}

Adj rnd_dag(int n, int p) {
    Adj g(n);
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    shuffle(perm.begin(), perm.end(), rng); // edges go forward in a random order of labels
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if ((int)(rng() % 100) < p) g[perm[i]].push_back(perm[j]);
    return g;
}

// product games: all = 0 -> ends when ANY component ends, all = 1 -> ends when ALL end
bool sum_win(const Adj &A, const Adj &B, int a, int b, int all, map<pair<int, int>, bool> &memo) {
    auto key = make_pair(a, b);
    if (memo.count(key)) return memo[key];
    bool ea = A[a].empty(), eb = B[b].empty();
    bool res = false;
    if (all == 0 && (ea || eb)) return memo[key] = false;
    if (all == 1 && ea && eb) return memo[key] = false;
    vector<int> na = ea ? vector<int>{a} : A[a], nb = eb ? vector<int>{b} : B[b];
    for (int x : na)
        for (int y : nb)
            if (!sum_win(A, B, x, y, all, memo)) res = true;
    return memo[key] = res;
}

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 9 + 1, p = vector<int>{10, 30, 50, 80}[rng() % 4];
        Adj g = rnd_dag(n, p);
        Remoteness_Suspense rs(n);
        for (int u = 0; u < n; u++)
            for (int v : g[u]) rs.add_move(u, v);
        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        shuffle(order.begin(), order.end(), rng); // query in random order (memo from any start)
        for (int u : order) {
            auto [w0, r] = minimax(g, u, 0);
            auto [w1, s] = minimax(g, u, 1);
            int q = rng() % 3; // vary which method is called first
            if (q == 0) chk("is_winning", rs.is_winning(u) == w0 && w0 == w1);
            if (q <= 1) chk("remoteness", rs.remoteness(u) == r);
            chk("suspense", rs.suspense(u) == s);
            chk("remoteness", rs.remoteness(u) == r);
            chk("is_winning", rs.is_winning(u) == w0);
            chk("parity: losing iff even", (r % 2 == 0) == !w0 && (s % 2 == 0) == !w0);
        }
    }
    for (int it = 0; it < 3000; it++) { // the sum rules quoted in the header
        int na = rng() % 7 + 1, nb = rng() % 7 + 1;
        Adj A = rnd_dag(na, 40), B = rnd_dag(nb, 40);
        Remoteness_Suspense ra(na), rb(nb);
        for (int u = 0; u < na; u++)
            for (int v : A[u]) ra.add_move(u, v);
        for (int u = 0; u < nb; u++)
            for (int v : B[u]) rb.add_move(u, v);
        map<pair<int, int>, bool> m0, m1;
        for (int a = 0; a < na; a++)
            for (int b = 0; b < nb; b++) {
                bool any = min(ra.remoteness(a), rb.remoteness(b)) % 2 == 1;
                bool all = max(ra.suspense(a), rb.suspense(b)) % 2 == 1;
                chk("sum rule: ends when any ends", any == sum_win(A, B, a, b, 0, m0));
                chk("sum rule: ends when all end", all == sum_win(A, B, a, b, 1, m1));
            }
    }
    { // hand: chain 0->1->2->3: lengths 3, 2, 1, 0
        Remoteness_Suspense c(4);
        c.add_move(0, 1), c.add_move(1, 2), c.add_move(2, 3);
        chk("hand: chain", c.remoteness(0) == 3 && c.remoteness(1) == 2 && c.suspense(1) == 2);
    }
    {
        int n = 200000;
        Remoteness_Suspense rs(n);
        for (int i = 0; i + 1 < n; i++) rs.add_move(i, i + 1); // long chain: deep recursion
        for (int i = 0; i < 800000; i++) {
            int u = rng() % n, v = rng() % n;
            if (u != v) rs.add_move(min(u, v), max(u, v));
        }
        auto t0 = clock();
        long long tot = 0;
        for (int u = 0; u < n; u++) tot += rs.remoteness(u) + rs.suspense(u);
        double sec = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("DAG n=2e5 m=1e6, all positions: %.3fs (sum=%lld)\n", sec, tot);
        Remoteness_Suspense ch(n);
        for (int i = 0; i + 1 < n; i++) ch.add_move(i, i + 1);
        chk("long chain n=2e5", ch.remoteness(0) == n - 1 && ch.suspense(0) == n - 1);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
