// Test for 08_Game_Theory/Impartial_Games.cpp against explicit minimax over the full compound game.
// Every compound game (Nim, misere Nim, selective, take-from-not-all) is solved by brute-force
// recursion over tuples of positions with a memo; Grundy values by a separate set-based loop.
// Checked: mex, compute_grundy, Grundy_Graph, nim_value, normal_disjunctive_long,
// misere_disjunctive_long, nim_winning_move, normal_selective_long, normal_selective_variant.
#include "../../08_Game_Theory/Impartial_Games.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(1730);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

// a compound game: component positions are nodes of one DAG `out`; mode picks the compound rule
// mode 0: disjunctive (move in exactly one), 1: selective (move in any non-empty set),
// 2: selective, not all (non-empty set of size <= k-1 where k = number of components)
// misere: the player who cannot move wins
vector<vector<int>> out;
map<pair<vector<int>, int>, bool> memo;
int MODE;
bool MIS;
bool wins(vector<int> s) {
    sort(s.begin(), s.end());
    auto key = make_pair(s, MODE * 2 + MIS);
    if (memo.count(key)) return memo[key];
    int k = s.size();
    bool any_move = false, res = false;
    vector<int> movable;
    for (int i = 0; i < k; i++)
        if (!out[s[i]].empty()) movable.push_back(i);
    int lim = MODE == 0 ? 1 : MODE == 1 ? k : k - 1;
    int M = movable.size();
    for (int mask = 1; mask < (1 << M) && !res; mask++) {
        if (__builtin_popcount(mask) > lim) continue;
        // enumerate all combinations of moves in the chosen components
        vector<int> idx;
        for (int j = 0; j < M; j++)
            if (mask >> j & 1) idx.push_back(movable[j]);
        vector<int> ch(idx.size(), 0);
        while (!res) {
            vector<int> t = s;
            for (size_t j = 0; j < idx.size(); j++) t[idx[j]] = out[s[idx[j]]][ch[j]];
            any_move = true;
            if (!wins(t)) res = true;
            size_t j = 0;
            while (j < idx.size() && ++ch[j] == (int)out[s[idx[j]]].size()) ch[j++] = 0;
            if (j == idx.size()) break;
        }
    }
    if (!any_move) res = MIS;
    return memo[key] = res;
}
bool solve_game(const vector<int> &s, int mode, bool mis) { return MODE = mode, MIS = mis, wins(s); }

// Grundy values of `out` (DAG, edges go to smaller ids) by the definition, with std::set
vector<int> grundy_brute() {
    vector<int> g(out.size());
    for (size_t v = 0; v < out.size(); v++) {
        set<int> s;
        for (int u : out[v]) s.insert(g[u]);
        while (s.count(g[v])) g[v]++;
    }
    return g;
}

void nim_graph(int maxp) { // node p = Nim pile of size p
    out.assign(maxp + 1, {});
    for (int p = 0; p <= maxp; p++)
        for (int q = 0; q < p; q++) out[p].push_back(q);
    memo.clear();
}

int main() {
    // mex
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 8;
        vector<int> v(n);
        for (auto &x : v) x = (int)(rng() % 10) - 2;
        if (rng() % 5 == 0 && n) v[0] = 1000000000;
        int m = 0;
        while (find(v.begin(), v.end(), m) != v.end()) m++;
        chk("mex", mex(v) == m);
    }
    // Nim: normal, misere, winning move, selective-variant; brute over all tuples of piles
    nim_graph(6);
    for (int k = 0; k <= 4; k++) {
        int total = 1;
        for (int i = 0; i < k; i++) total *= 7;
        for (int c = 0; c < total; c++) {
            vector<int> s;
            for (int i = 0, x = c; i < k; i++, x /= 7) s.push_back(x % 7);
            vector<ll> piles(s.begin(), s.end());
            bool nw = solve_game(s, 0, false);
            chk("nim_value", (nim_value(piles) != 0) == nw);
            chk("normal_disjunctive_long", normal_disjunctive_long(piles) == nw);
            chk("misere_disjunctive_long", misere_disjunctive_long(piles) == solve_game(s, 0, true));
            auto [i, take] = nim_winning_move(piles);
            if (!nw) chk("nim_winning_move", i == -1 && take == 0);
            else {
                bool ok = i >= 0 && i < k && take >= 1 && take <= piles[i];
                if (ok) {
                    vector<int> t = s;
                    t[i] -= (int)take;
                    ok = !solve_game(t, 0, false);
                }
                chk("nim_winning_move", ok);
            }
            if (k >= 1) chk("normal_selective_variant", normal_selective_variant(piles) ==
                                                            solve_game(s, 2, false));
        }
    }
    // 64-bit Nim piles
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 6 + 1;
        vector<ll> p(n);
        for (auto &x : p) x = rng() % 3 ? (ll)(rng() >> 1) : (ll)(rng() % 3);
        ll x = 0;
        for (ll q : p) x ^= q;
        chk("nim_value(64-bit)", nim_value(p) == x);
        auto [i, take] = nim_winning_move(p);
        if (x == 0) chk("nim_winning_move(64-bit)", i == -1);
        else {
            bool ok = i >= 0 && take >= 1 && take <= p[i];
            if (ok) p[i] -= take, ok = nim_value(p) == 0;
            chk("nim_winning_move(64-bit)", ok);
        }
    }
    // random DAG games: Grundy_Graph and the selective / disjunctive rules on 1..3 components
    for (int it = 0; it < 1500; it++) {
        int n = rng() % 8 + 1;
        out.assign(n, {});
        Grundy_Graph G(n);
        for (int v = 1; v < n; v++)
            for (int u = 0; u < v; u++)
                if (rng() % 3 == 0) out[v].push_back(u), G.add_move(v, u);
        memo.clear();
        auto gb = grundy_brute();
        bool ok = true;
        for (int v = n - 1; v >= 0; v--) ok &= G.grundy(v) == gb[v];
        chk("Grundy_Graph", ok);
        for (int q = 0; q < 6; q++) {
            int k = rng() % 4 + 1;
            vector<int> s(k), gr(k);
            vector<ll> grl(k);
            for (int i = 0; i < k; i++) s[i] = rng() % n, gr[i] = gb[s[i]], grl[i] = gr[i];
            chk("normal_disjunctive_long(grundy)", normal_disjunctive_long(grl) ==
                                                       solve_game(s, 0, false));
            chk("normal_selective_long", normal_selective_long(gr) == solve_game(s, 1, false));
            chk("normal_selective_variant(grundy, empirical)",
                normal_selective_variant(grl) == solve_game(s, 2, false));
        }
    }
    // subtraction games: compute_grundy vs the definition, and the sum of two games by minimax
    for (int it = 0; it < 300; it++) {
        int H = rng() % 25 + 1;
        vector<int> moves;
        for (int m = 1; m <= 6; m++)
            if (rng() % 2) moves.push_back(m);
        if (moves.empty()) moves.push_back(rng() % 6 + 1);
        out.assign(H + 1, {});
        for (int h = 0; h <= H; h++)
            for (int m : moves)
                if (h >= m) out[h].push_back(h - m);
        memo.clear();
        auto gb = grundy_brute();
        vector<int> dp(H + 1, -1);
        bool ok = true;
        for (int h = H; h >= 0; h -= 3) ok &= compute_grundy(h, moves, dp) == gb[h];
        for (int h = 0; h <= H; h++) ok &= compute_grundy(h, moves, dp) == gb[h];
        chk("compute_grundy", ok);
        for (int q = 0; q < 10; q++) {
            int a = rng() % (H + 1), b = rng() % (H + 1);
            chk("compute_grundy(sum by minimax)",
                ((compute_grundy(a, moves, dp) ^ compute_grundy(b, moves, dp)) != 0) ==
                    solve_game({a, b}, 0, false));
        }
    }
    // edge cases
    chk("edge:empty", !normal_disjunctive_long({}) && misere_disjunctive_long({}) &&
                          !normal_selective_long({}) && nim_winning_move({}).first == -1);
    chk("edge:misere single 1", !misere_disjunctive_long({1}) && misere_disjunctive_long({2}));
    chk("edge:variant single pile", !normal_selective_variant({5}));
    // CSES 1730 Nim Game I style / CSES 1098 Nim Game II (subtract 1..3 -> Grundy p % 4)
    {
        vector<int> dp(100, -1), mv{1, 2, 3};
        bool ok = true;
        for (int h = 0; h < 100; h++) ok &= compute_grundy(h, mv, dp) == h % 4;
        chk("samples(CSES 1098 rule: g = h % 4)", ok);
    }
    {   // timing
        auto t0 = clock();
        int H = 1000000;
        vector<int> dp(H + 1, -1), mv{1, 3, 4, 7, 11};
        for (int h = 0; h <= H; h += 1000) compute_grundy(h, mv, dp);
        compute_grundy(H, mv, dp);
        int n = 200000;
        Grundy_Graph G(n);
        for (int i = 0; i < 1000000; i++) {
            int u = rng() % n, v = rng() % n;
            if (u != v) G.add_move(max(u, v), min(u, v));
        }
        ll s = 0;
        for (int v = 0; v < n; v++) s += G.grundy(v);
        vector<ll> p(1000000);
        for (auto &x : p) x = rng() >> 1;
        s += normal_disjunctive_long(p) + misere_disjunctive_long(p) + nim_winning_move(p).first;
        printf("compute_grundy H=1e6 + Grundy_Graph n=2e5 m=1e6 + Nim 1e6 piles: %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-40s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
