// Brute-force test for:
//   Game_Theory/Grundy.cpp
//   Game_Theory/Games_On_Graph.cpp
//   Game_Theory/Wythoff_Game.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace GR {
#include "../Game_Theory/Grundy.cpp"
}
namespace GG {
#include "../Game_Theory/Games_On_Graph.cpp"
}
namespace WY {
#include "../Game_Theory/Wythoff_Game.cpp"
}

int main() {
    mt19937_64 gen(31415);
    int mexFail = 0, tabFail = 0, nimFail = 0, misFail = 0, moveFail = 0, sumFail = 0, ggraphFail = 0;
    int wyFail = 0, wyPairFail = 0, wyBigFail = 0, drawFail = 0;

    // ---------- mex ----------
    for (int it = 0; it < 20000; it++) {
        int n = (int)(gen() % 8);
        vector<int> v(n);
        for (int &x : v) x = (int)(gen() % 10);
        set<int> s(v.begin(), v.end());
        int want = 0;
        while (s.count(want)) want++;
        if (GR::mex(v) != want) mexFail++;
    }

    // ---------- Grundy table for subtraction games, against a direct win/lose DP ----------
    for (int it = 0; it < 800; it++) {
        int k = 1 + (int)(gen() % 4);
        set<int> takeSet;
        while ((int)takeSet.size() < k) takeSet.insert(1 + (int)(gen() % 6));
        vector<int> take(takeSet.begin(), takeSet.end());
        int LIMIT = 60;
        auto moves = [&](int s) {
            vector<int> out;
            for (int t : take)
                if (s - t >= 0) out.push_back(s - t);
            return out;
        };
        vector<int> g = GR::grundy_table(LIMIT, moves);
        // a single-pile position loses exactly when its Grundy value is 0
        vector<char> lose(LIMIT + 1, 0);
        for (int s = 0; s <= LIMIT; s++) {
            bool any = false;
            for (int t : take)
                if (s - t >= 0 && lose[s - t]) any = true;
            lose[s] = !any;
        }
        for (int s = 0; s <= LIMIT; s++)
            if ((g[s] == 0) != (bool)lose[s]) tabFail++;

        // a sum of two such piles: XOR of Grundy values must match a brute-force product DP
        int SM = 18;
        vector<vector<char>> loseSum(SM + 1, vector<char>(SM + 1, 0));
        for (int a = 0; a <= SM; a++)
            for (int b = 0; b <= SM; b++) {
                bool any = false;
                for (int t : take) {
                    if (a - t >= 0 && loseSum[a - t][b]) any = true;
                    if (b - t >= 0 && loseSum[a][b - t]) any = true;
                }
                loseSum[a][b] = !any;
            }
        for (int a = 0; a <= SM; a++)
            for (int b = 0; b <= SM; b++) {
                bool wantWin = !loseSum[a][b];
                if (GR::first_player_wins(vector<int>{g[a], g[b]}) != wantWin) sumFail++;
            }
    }

    // ---------- Nim, normal and misere, against a brute-force DP ----------
    for (int it = 0; it < 4000; it++) {
        int n = 1 + (int)(gen() % 4);
        vector<ll> piles(n);
        for (ll &p : piles) p = (ll)(gen() % 7);
        // brute force over the full product state space
        map<vector<ll>, char> memoN, memoM;
        function<bool(vector<ll>, bool)> win = [&](vector<ll> st, bool misere) -> bool {
            sort(st.begin(), st.end());
            auto &memo = misere ? memoM : memoN;
            auto it2 = memo.find(st);
            if (it2 != memo.end()) return it2->second;
            bool anyMove = false;
            for (ll v : st)
                if (v > 0) anyMove = true;
            if (!anyMove) return memo[st] = (misere ? true : false); // no move: normal loses, misere wins
            bool res = false;
            for (size_t i = 0; i < st.size() && !res; i++)
                for (ll take = 1; take <= st[i] && !res; take++) {
                    vector<ll> nxt = st;
                    nxt[i] -= take;
                    if (!win(nxt, misere)) res = true;
                }
            return memo[st] = res;
        };
        if (GR::nim_first_player_wins(piles) != win(piles, false)) nimFail++;
        if (GR::misere_nim_first_player_wins(piles) != win(piles, true)) misFail++;

        // the reported winning move must actually reach a losing position
        auto [idx, take] = GR::nim_winning_move(piles);
        if (GR::nim_first_player_wins(piles)) {
            if (idx < 0 || take <= 0 || take > piles[idx]) moveFail++;
            else {
                vector<ll> nxt = piles;
                nxt[idx] -= take;
                if (GR::nim_first_player_wins(nxt)) moveFail++;
            }
        } else if (idx != -1)
            moveFail++;
    }

    // ---------- games on a graph, including cycles ----------
    for (int it = 0; it < 3000; it++) {
        int n = 1 + (int)(gen() % 7);
        GG::Games_On_Graph g(n);
        vector<vector<int>> adj(n);
        int m = (int)(gen() % 12);
        for (int i = 0; i < m; i++) {
            int u = (int)(gen() % n), v = (int)(gen() % n);
            g.add_move(u, v);
            adj[u].push_back(v);
        }
        g.solve_all();
        // reference: iterate the classification to a fixed point
        vector<int> state(n, 0); // 0 unknown, 1 win, 2 lose
        for (int round = 0; round <= 2 * n + 5; round++) {
            for (int u = 0; u < n; u++) {
                if (state[u]) continue;
                if (adj[u].empty()) { state[u] = 2; continue; }
                bool anyLose = false, allWin = true;
                for (int v : adj[u]) {
                    if (state[v] == 2) anyLose = true;
                    if (state[v] != 1) allWin = false;
                }
                if (anyLose) state[u] = 1;
                else if (allWin) state[u] = 2;
            }
        }
        for (int u = 0; u < n; u++) {
            if (state[u] == 1 && !g.is_win(u)) ggraphFail++;
            if (state[u] == 2 && !g.is_lose(u)) ggraphFail++;
            if (state[u] == 0 && !g.is_draw(u)) drawFail++;
            if (g.is_win(u) && state[u] != 1) ggraphFail++;
            if (g.is_lose(u) && state[u] != 2) ggraphFail++;
        }
    }

    // ---------- Wythoff against the O(n^2) table ----------
    {
        int LIM = 120;
        auto table = WY::losing_table(LIM);
        for (int a = 0; a <= LIM; a++)
            for (int b = 0; b <= LIM; b++)
                if (WY::is_losing(a, b) != (bool)table[a][b]) wyFail++;
        // the listed losing pairs must all be losing, and cover every k
        auto pairs = WY::first_losing_pairs(40);
        for (int k = 0; k < 40; k++) {
            auto [a, b] = pairs[k];
            if (b - a != k) wyPairFail++;
            if (!WY::is_losing(a, b)) wyPairFail++;
            if (a <= LIM && b <= LIM && !table[a][b]) wyPairFail++;
        }
        // known first pairs
        vector<pair<ll, ll>> known{{0, 0}, {1, 2}, {3, 5}, {4, 7}, {6, 10}, {8, 13}, {9, 15}, {11, 18}};
        for (size_t k = 0; k < known.size(); k++)
            if (pairs[k] != known[k]) wyPairFail++;
    }
    // large k: floor(k*phi) must satisfy its defining inequality exactly
    for (ll k : {(ll)1000000, (ll)1000000007, (ll)3037000499, (ll)1234567890123}) {
        ll a = WY::floor_k_phi(k);
        // a <= k*phi < a+1  <=>  (2a-k)^2 < 5k^2 <= (2a+2-k)^2
        __int128 lo = (__int128)(2 * a - k) * (2 * a - k), hi = (__int128)(2 * a + 2 - k) * (2 * a + 2 - k);
        if (!(lo < (__int128)5 * k * k && (__int128)5 * k * k <= hi)) wyBigFail++;
        if (!WY::is_losing(a, a + k)) wyBigFail++;
    }

    cout << "Game_Theory\n";
    cout << "  mex                            : " << mexFail << " failures\n";
    cout << "  grundy_table vs win/lose DP    : " << tabFail << " failures\n";
    cout << "  game sum XOR vs product DP     : " << sumFail << " failures\n";
    cout << "  Nim verdict vs brute force     : " << nimFail << " failures\n";
    cout << "  misere Nim vs brute force      : " << misFail << " failures\n";
    cout << "  nim_winning_move really wins   : " << moveFail << " failures\n";
    cout << "  graph game win/lose            : " << ggraphFail << " failures\n";
    cout << "  graph game draws (cycles)      : " << drawFail << " failures\n";
    cout << "  Wythoff vs the O(n^2) table    : " << wyFail << " failures\n";
    cout << "  Wythoff losing pairs           : " << wyPairFail << " failures\n";
    cout << "  floor(k*phi) at large k        : " << wyBigFail << " failures\n";
}
