/* Topic: 2D Hashing
 * Description: 2D String Hashing computes the polynomial rolling hash of a 2D matrix, enabling
 *   submatrix equality checks in O(1) time after O(N * M) preprocessing.
 * Usage: Hash2D hs(grid); hs.get_hash(x1, y1, x2, y2);
 */
#include "../../core.h"

// Time Complexity: O(NM) build, O(1) query
// Space Complexity: O(NM)
struct Hash2D {
    int n, m;
    vector<int> PWX, PWY;
    vector<vector<int>> hs;
    static constexpr int PX = 3731, PY = 2999, mod = 998244353;

    inline int add(int a, int b) { return (a + b) % mod; }

    inline int sub(int a, int b) { return (a - b + 2 * mod) % mod; }

    inline int mul(int a, int b) { return ((a % mod) * (b % mod)) % mod; }

    template <typename T>
    inline void calc(const T& s) {
        for (int i = 0; i < n; i++) PWX[i + 1] = mul(PWX[i], PX);
        for (int i = 0; i < m; i++) PWY[i + 1] = mul(PWY[i], PY);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) hs[i + 1][j + 1] = s[i][j];
        for (int i = 0; i <= n; i++)
            for (int j = 0; j < m; j++) hs[i][j + 1] = add(hs[i][j + 1], mul(hs[i][j], PY));
        for (int i = 0; i < n; i++)
            for (int j = 0; j <= m; j++) hs[i + 1][j] = add(hs[i + 1][j], mul(hs[i][j], PX));
    }

    template <typename T>
    Hash2D(const T& s) : n(sz(s)), m(sz(s[0])), PWX(n + 1, 1), PWY(m + 1, 1), hs(n + 1, vector<int>(m + 1, 0)) {
        calc(s);
    }

    inline int get_hash(int x1, int y1, int x2, int y2) {  // 1-indexed
        int dx = x2 - x1 + 1, dy = y2 - y1 + 1;
        int term1 = sub(hs[x2][y2], mul(hs[x2][y1 - 1], PWY[dy]));
        int term2 = sub(hs[x1 - 1][y2], mul(hs[x1 - 1][y1 - 1], PWY[dy]));
        return sub(term1, mul(term2, PWX[dx]));
    }
};
