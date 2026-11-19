// Hash_2D: one hash for any rectangle of a grid, so two equal-size rectangles compare in O(1).
// Use when: "does this pattern grid occur here", count occurrences of a submatrix, repeated
// submatrices, comparing two rectangles of a char or int grid.
// Usage: Hash_2D H(grid); H.get_hash(x1, y1, x2, y2) == P.get_hash();
// Time: build O(n m) | get_hash O(1)
// Indexing: queries 1-based inclusive (rows x, columns y); the grid itself is 0-based
// Note: grid is vector<string> or vector<vector<int>>, n, m >= 1. Cell values must lie in a range
// narrower than 2999 (chars are fine); compress bigger ints first or equal-hash grids exist.
// Single mod 998244353 with fixed bases: confirm matches directly if many hashes are compared.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Hash_2D {
    static constexpr int PX = 3731, PY = 2999, MOD = 998244353;
    int n = 0, m = 0;
    vector<int> PWX, PWY;
    vector<vector<int>> hs;

    static ll add(ll a, ll b) { return (a + b) % MOD; }
    static ll sub(ll a, ll b) { return (a - b + 2 * MOD) % MOD; }
    static ll mul(ll a, ll b) { return ((a % MOD) * (b % MOD)) % MOD; }

    Hash_2D() {}
    template <typename T> Hash_2D(const T &s) : n((int)s.size()), m((int)s[0].size()) {
        PWX.assign(n + 1, 1), PWY.assign(m + 1, 1);
        hs.assign(n + 1, vector<int>(m + 1, 0));
        for (int i = 0; i < n; i++) PWX[i + 1] = mul(PWX[i], PX);
        for (int i = 0; i < m; i++) PWY[i + 1] = mul(PWY[i], PY);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) hs[i + 1][j + 1] = s[i][j];
        for (int i = 0; i <= n; i++)
            for (int j = 0; j < m; j++) hs[i][j + 1] = add(hs[i][j + 1], mul(hs[i][j], PY));
        for (int i = 0; i < n; i++)
            for (int j = 0; j <= m; j++) hs[i + 1][j] = add(hs[i + 1][j], mul(hs[i][j], PX));
    }

    int get_hash() const { return get_hash(1, 1, n, m); } // the whole grid

    int get_hash(int x1, int y1, int x2, int y2) const { // rows [x1, x2] x columns [y1, y2]
        assert(1 <= x1 && x1 <= x2 && x2 <= n);
        assert(1 <= y1 && y1 <= y2 && y2 <= m);
        x1--, y1--;
        int dx = x2 - x1, dy = y2 - y1;
        ll term1 = sub(hs[x2][y2], mul(hs[x2][y1], PWY[dy]));
        ll term2 = sub(hs[x1][y2], mul(hs[x1][y1], PWY[dy]));
        return (int)sub(term1, mul(term2, PWX[dx]));
    }
};
