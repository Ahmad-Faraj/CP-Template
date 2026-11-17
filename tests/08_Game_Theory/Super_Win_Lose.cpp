// Test for 08_Game_Theory/Super_Win_Lose.cpp against explicit minimax over the whole sum game.
// Oracle: the position is the tuple of component states. Normal: if any component is terminal the
// mover has lost, else win iff some move (in one component) reaches a losing tuple. Misere: if any
// component is terminal the mover has won (the opponent finished it), else the same recursion.
// Components come from different random acyclic move tables (state = table * 100 + s).
// Methods checked: grundy_normal, grundy_misere (SL/SW classes vs definition, values via sums),
// first_player_wins (normal and misere, 0..4 components), mex.
#include "../../08_Game_Theory/Super_Win_Lose.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(23);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

vector<vector<vector<int>>> tables; // tables[t][s] = moves from s (to smaller s)
vector<int> moves_of(int st) {
    vector<int> r;
    for (int x : tables[st / 100][st % 100]) r.push_back(st / 100 * 100 + x);
    return r;
}

map<pair<vector<int>, bool>, bool> memo;
bool brute(vector<int> v, bool misere) {
    sort(v.begin(), v.end());
    auto key = make_pair(v, misere);
    if (memo.count(key)) return memo[key];
    for (int s : v)
        if (moves_of(s).empty()) return memo[key] = misere;
    bool win = false;
    for (int i = 0; i < (int)v.size() && !win; i++)
        for (int to : moves_of(v[i])) {
            auto w = v;
            w[i] = to;
            if (!brute(w, misere)) {
                win = true;
                break;
            }
        }
    return memo[key] = win;
}

int main() {
    for (int it = 0; it < 400; it++) {
        int T = rng() % 3 + 1;
        tables.assign(T, {});
        for (int t = 0; t < T; t++) {
            int K = rng() % 9 + 1, p = rng() % 70 + 15;
            tables[t].assign(K, {});
            for (int s = 1; s < K; s++) {
                for (int x = 0; x < s; x++) // moves to 0 (finishing) are rarer
                    if ((int)(rng() % 100) < (x ? p : p / 4)) tables[t][s].push_back(x);
                if (s >= 2 && rng() % 2) tables[t][s].push_back(1 + rng() % (s - 1)); // fewer ends
            }
        }
        memo.clear();
        Super_Win_Lose g(moves_of);
        vector<int> all;
        for (int t = 0; t < T; t++)
            for (int s = 0; s < (int)tables[t].size(); s++) all.push_back(t * 100 + s);
        for (int st : all) { // the special classes, straight from the definition
            bool term = moves_of(st).empty(), to_term = false;
            for (int x : moves_of(st)) to_term |= moves_of(x).empty();
            int gn = g.grundy_normal(st), gm = g.grundy_misere(st);
            chk("grundy_normal: SL iff terminal", (gn == g.SL) == term);
            chk("grundy_normal: SW iff can finish", (gn == g.SW) == (!term && to_term));
            chk("grundy_misere: SW iff terminal", (gm == g.SW) == term && gm != g.SL);
        }
        for (int a : all) // two plain components: second player wins iff equal Grundy values
            for (int b : all)
                for (int mis = 0; mis < 2; mis++) {
                    int x = mis ? g.grundy_misere(a) : g.grundy_normal(a);
                    int y = mis ? g.grundy_misere(b) : g.grundy_normal(b);
                    if (x >= g.SL || y >= g.SL) continue;
                    string t = mis ? "misere" : "normal";
                    chk("grundy_" + t + " values (all pairs)", (x != y) == brute({a, b}, mis));
                    if (x > 0) chk("grundy_" + t + " values > 0 (pairs)", true);
                }
        for (int rep = 0; rep < 60; rep++) {
            int k = rng() % 5;
            vector<int> v(k);
            for (auto &x : v) x = all[rng() % all.size()];
            for (int mis = 0; mis < 2; mis++) {
                bool want = brute(v, mis);
                string t = mis ? "misere" : "normal";
                bool got = g.first_player_wins(v, mis);
                chk("first_player_wins " + t + " k=" + to_string(k), got == want);
            }
        }
    }
    using S = Super_Win_Lose;
    chk("mex", S::mex({}) == 0 && S::mex({0, 1, 3}) == 2 && S::mex({1, 2}) == 0);
    { // the classic remove 1, 3 or 4 tokens, one big state (deep recursion)
#ifdef __SANITIZE_ADDRESS__
        const int BIG = 200000; // keeps the sanitizer run short
#else
        const int BIG = 1000000;
#endif
        Super_Win_Lose g([](int s) {
            vector<int> r;
            for (int d : {1, 3, 4})
                if (s >= d) r.push_back(s - d);
            return r;
        });
        auto t0 = clock();
        int a = g.grundy_normal(BIG), b = g.grundy_misere(BIG);
        double sec = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("subtract {1,3,4}, state %d, both variants: %.3fs (%d %d)\n", BIG, sec, a, b);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-36s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
