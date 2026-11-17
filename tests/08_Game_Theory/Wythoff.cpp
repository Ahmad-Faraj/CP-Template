// Test for 08_Game_Theory/Wythoff.cpp against an exhaustive win/lose table and exact integer math.
// Every public function is checked: floor_k_phi, get_kth_wythoff_pair, is_wythoff_losing,
// wythoff_pairs, is_valid_wythoff_move, generate_wythoff_moves.
#include "../../08_Game_Theory/Wythoff.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(17);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

const int L = 200;
bool lose[L + 1][L + 1];

// every move from (a, b), written independently: take t >= 1 from pile 1, pile 2, or both
set<pair<ll, ll>> brute_moves(ll a, ll b) {
    set<pair<ll, ll>> s;
    for (ll t = 1; t <= max(a, b); t++) {
        if (t <= a) s.insert({a - t, b});
        if (t <= b) s.insert({a, b - t});
        if (t <= a && t <= b) s.insert({a - t, b - t});
    }
    return s;
}

// floor(k * phi) = floor((k + isqrt(5 k^2)) / 2), isqrt by binary search on __int128
ll brute_floor_phi(ll k) {
    __int128 n = (__int128)5 * k * k, lo = 0, hi = (__int128)3 * k + 2;
    while (lo < hi) {
        __int128 mid = (lo + hi + 1) / 2;
        if (mid * mid <= n) lo = mid;
        else hi = mid - 1;
    }
    return (ll)((k + lo) / 2);
}

int main() {
    for (int a = 0; a <= L; a++)
        for (int b = 0; b <= L; b++) {
            bool anyLose = false;
            for (auto [x, y] : brute_moves(a, b)) anyLose |= lose[x][y];
            lose[a][b] = !anyLose;
        }
    for (int a = 0; a <= L; a++)
        for (int b = 0; b <= L; b++) chk("is_wythoff_losing(small)", is_wythoff_losing(a, b) == lose[a][b]);

    // the k-th losing pair from the table: the losing (a, b) with a <= b and b - a == k
    for (int k = 0; k <= 75; k++) {
        int fa = -1, cnt = 0;
        for (int a = 0; a + k <= L; a++)
            if (lose[a][a + k]) fa = a, cnt++;
        auto p = get_kth_wythoff_pair(k);
        chk("get_kth_wythoff_pair(small)", cnt == 1 && p.first == fa && p.second == fa + k);
        chk("floor_k_phi(small)", floor_k_phi(k) == fa);
    }
    for (int N : {0, 1, 2, 7, 76}) {
        auto v = wythoff_pairs(N);
        bool ok = (int)v.size() == N;
        for (int k = 0; ok && k < N; k++)
            ok = v[k].second - v[k].first == k && lose[v[k].first][v[k].second];
        chk("wythoff_pairs", ok);
    }

    // large k: random, powers of ten, Fibonacci numbers (where the double formula breaks)
    vector<ll> ks;
    for (int i = 0; i < 300000; i++) ks.push_back(rng() % (ll)1e18);
    for (int i = 0; i < 300000; i++) ks.push_back(rng() % 2000000000);
    for (ll p = 1; p > 0 && p <= (ll)1e18; p = p <= (ll)1e17 ? p * 10 : -1)
        ks.push_back(p), ks.push_back(p - 1), ks.push_back(p + 1);
    for (ll f1 = 1, f2 = 2; f2 <= (ll)1e18; swap(f1, f2), f2 += f1) ks.push_back(f1), ks.push_back(f2);
    ks.push_back(102334155);
    for (ll k : ks) {
        ll a = brute_floor_phi(k);
        chk("floor_k_phi(large)", floor_k_phi(k) == a);
        auto p = get_kth_wythoff_pair(k);
        chk("get_kth_wythoff_pair(large)", p.first == a && p.second == a + k);
        chk("is_wythoff_losing(large)", is_wythoff_losing(a, a + k) && is_wythoff_losing(a + k, a));
        chk("is_wythoff_losing(large,not)", !is_wythoff_losing(a + 1, a + k) &&
                                                !is_wythoff_losing(a, a + k + 1) &&
                                                (a == 0 || !is_wythoff_losing(a - 1, a + k)));
    }
    chk("is_wythoff_losing(F40 case)", is_wythoff_losing(165580140, 267914295));

    for (int it = 0; it < 3000; it++) {
        ll a = rng() % 40, b = rng() % 40;
        auto s = brute_moves(a, b);
        auto g = generate_wythoff_moves(a, b);
        chk("generate_wythoff_moves", set<pair<ll, ll>>(g.begin(), g.end()) == s && g.size() == s.size());
        for (ll x = -3; x <= 43; x++)
            for (ll y = -3; y <= 43; y++)
                chk("is_valid_wythoff_move", is_valid_wythoff_move(a, b, x, y) == (bool)s.count({x, y}));
    }

    {
        auto t0 = clock();
        ll cnt = 0;
        for (int i = 0; i < 1000000; i++) {
            ll a = rng() % (ll)1e18, b = rng() % (ll)1e18;
            cnt += is_wythoff_losing(a, b);
            auto p = get_kth_wythoff_pair(a);
            cnt += is_wythoff_losing(p.first, p.second);
        }
        auto v = wythoff_pairs(1000000);
        printf("1e6 x (is_wythoff_losing + get_kth_wythoff_pair) up to 1e18, wythoff_pairs(1e6): %.3fs "
               "(cnt=%lld, last=%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, cnt, v.back().first);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
