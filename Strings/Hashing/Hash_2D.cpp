// 2D Hashing: one hash for any rectangle of a grid, so two rectangles can be compared in O(1).
// Use when: "does this pattern grid occur here", finding repeated submatrices, comparing two rectangles.
// Handles: grids of characters or ints, any rectangle, build once and query freely. Single mod.
// Time: build O(n m) | get_hash O(1)
// Indexing: 1-based for queries; the grid passed to the constructor is 0-based
// Note: single mod, so on adversarial input confirm a hash match by comparing the rectangles directly.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Hash_2D {
    int n, m;
    vector<int> PWX, PWY;
    vector<vector<int>> hs;
    static constexpr int PX = 3731, PY = 2999, MOD = 998244353;

    ll add(ll a, ll b) { return (a + b) % MOD; }
    ll sub(ll a, ll b) { return (a - b + 2 * MOD) % MOD; }
    ll mul(ll a, ll b) { return ((a % MOD) * (b % MOD)) % MOD; }

    Hash_2D() = default;

    template <typename T> Hash_2D(const T &s) { // s is a vector<string> or vector<vector<int>>
        n = (int)s.size();
        m = (int)s[0].size();
        PWX.assign(n + 1, 1);
        PWY.assign(m + 1, 1);
        hs.assign(n + 1, vector<int>(m + 1, 0));
        calc(s);
    }

    template <typename T> void calc(const T &s) {
        for (int i = 0; i < n; i++) PWX[i + 1] = mul(PWX[i], PX);
        for (int i = 0; i < m; i++) PWY[i + 1] = mul(PWY[i], PY);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) hs[i + 1][j + 1] = s[i][j];
        for (int i = 0; i <= n; i++)
            for (int j = 0; j < m; j++) hs[i][j + 1] = add(hs[i][j + 1], mul(hs[i][j], PY));
        for (int i = 0; i < n; i++)
            for (int j = 0; j <= m; j++) hs[i + 1][j] = add(hs[i + 1][j], mul(hs[i][j], PX));
    }

    int get_hash() { return get_hash(1, 1, n, m); } // hash of the whole grid

    int get_hash(int x1, int y1, int x2, int y2) { // hash of rows [x1,x2] by columns [y1,y2]
        assert(1 <= x1 && x1 <= x2 && x2 <= n);
        assert(1 <= y1 && y1 <= y2 && y2 <= m);
        x1--, y1--;
        int dx = x2 - x1, dy = y2 - y1;
        ll term1 = sub(hs[x2][y2], mul(hs[x2][y1], PWY[dy]));
        ll term2 = sub(hs[x1][y2], mul(hs[x1][y1], PWY[dy]));
        return (int)sub(term1, mul(term2, PWX[dx]));
    }
};

// Standard problem: how many times does a p x q pattern grid occur inside an n x m grid
void solve() {
    int n, m, p, q;
    cin >> n >> m;
    vector<string> g(n);
    for (auto &r : g) cin >> r;
    cin >> p >> q;
    vector<string> pat(p);
    for (auto &r : pat) cin >> r;
    Hash_2D G(g), P(pat);
    int target = P.get_hash(), cnt = 0;
    for (int i = 1; i + p - 1 <= n; i++)
        for (int j = 1; j + q - 1 <= m; j++)
            if (G.get_hash(i, j, i + p - 1, j + q - 1) == target) cnt++;
    cout << cnt << '\n';
}
