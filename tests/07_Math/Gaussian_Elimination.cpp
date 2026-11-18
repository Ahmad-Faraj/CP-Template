// Test for 07_Math/Gaussian_Elimination.cpp. Oracle for Gauss (doubles): exact integer arithmetic
// with no elimination at all - rank = largest non-zero minor (determinants by permutation
// expansion), solution kind by Rouche-Capelli, the unique solution by Cramer's rule, and the
// determinant by permutation expansion. Oracle for Gauss_XOR: enumerate all 2^m assignments and
// the span of all row subsets. Checked: Gauss (kind, answer, rank, determinant), Gauss_XOR
// (solvable, answer, rank), on unique / none / infinite systems, square and rectangular.
#include "../../07_Math/Gaussian_Elimination.cpp"
using ll = long long;

map<string, pair<long long, long long>> cov;
mt19937_64 rng(31);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

ll det_perm(const vector<vector<ll>> &M) { // sum over permutations
    int k = M.size();
    if (k == 0) return 1;
    vector<int> p(k);
    iota(p.begin(), p.end(), 0);
    ll s = 0;
    do {
        ll t = 1;
        for (int i = 0; i < k; i++) t *= M[i][p[i]];
        int inv = 0;
        for (int i = 0; i < k; i++)
            for (int j = i + 1; j < k; j++) inv += p[i] > p[j];
        s += inv % 2 ? -t : t;
    } while (next_permutation(p.begin(), p.end()));
    return s;
}

int rank_minors(const vector<vector<ll>> &A, int rows, int cols) {
    int best = 0;
    for (int rm = 0; rm < (1 << rows); rm++)
        for (int cm = 0; cm < (1 << cols); cm++) {
            int k = __builtin_popcount(rm);
            if (k != __builtin_popcount(cm) || k <= best) continue;
            vector<vector<ll>> M;
            for (int i = 0; i < rows; i++)
                if (rm >> i & 1) {
                    M.push_back({});
                    for (int j = 0; j < cols; j++)
                        if (cm >> j & 1) M.back().push_back(A[i][j]);
                }
            if (det_perm(M) != 0) best = k;
        }
    return best;
}

int main() {
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 5 + 1, m = rng() % 5 + 1;
        if (it % 3 == 0) m = n; // plenty of square systems
        int R = vector<int>{1, 2, 3, 5}[rng() % 4];
        vector<vector<ll>> A(n, vector<ll>(m + 1));
        for (auto &r : A)
            for (auto &v : r) v = (ll)(rng() % (2 * R + 1)) - R;
        if (n > 1 && rng() % 3 == 0) { // force dependent rows: row1 = k * row0 (+ maybe a broken rhs)
            ll k = (ll)(rng() % 5) - 2;
            for (int j = 0; j <= m; j++) A[1][j] = k * A[0][j];
            if (rng() % 2) A[1][m] += 1;
        }
        vector<vector<double>> aug(n, vector<double>(m + 1));
        for (int i = 0; i < n; i++)
            for (int j = 0; j <= m; j++) aug[i][j] = A[i][j];
        Gauss g(aug, n, m);
        int rA = rank_minors(A, n, m), rAug = rank_minors(A, n, m + 1);
        Solutions want = rA < rAug ? NO_SOLUTION : rA == m ? ONE_SOLUTION : INFINITE_SOLUTIONS;
        chk("Gauss.kind", g.kind == want);
        chk("Gauss.rank", g.rank == rA);
        string tag = want == NO_SOLUTION ? "none" : want == ONE_SOLUTION ? "unique" : "infinite";
        chk("Gauss.kind(" + tag + ")", g.kind == want);
        if (want != NO_SOLUTION) { // answer must satisfy every equation
            bool ok = (int)g.answer.size() == m;
            for (int i = 0; i < n && ok; i++) {
                double s = 0;
                for (int j = 0; j < m; j++) s += A[i][j] * g.answer[j];
                ok &= fabs(s - A[i][m]) < 1e-6;
            }
            chk("Gauss.answer(satisfies)", ok);
        }
        if (n == m) {
            vector<vector<ll>> sq(n, vector<ll>(n));
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++) sq[i][j] = A[i][j];
            ll D = det_perm(sq);
            chk("Gauss.determinant", fabs(g.determinant - D) < 1e-6 * max(1.0, fabs((double)D)));
            if (D != 0) { // Cramer
                bool ok = true;
                for (int c = 0; c < n; c++) {
                    auto Mc = sq;
                    for (int i = 0; i < n; i++) Mc[i][c] = A[i][m];
                    ok &= fabs(g.answer[c] - (double)det_perm(Mc) / D) < 1e-6;
                }
                chk("Gauss.answer(Cramer)", ok);
            }
        }
    }
    // GF(2)
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 8, m = rng() % 10 + 1;
        vector<pair<unsigned long long, int>> rows(n);
        for (auto &[mask, b] : rows) mask = rng() & ((1ULL << m) - 1), b = rng() % 2;
        if (n > 2 && rng() % 2) rows[2].first = rows[0].first ^ rows[1].first, rows[2].second = rng() % 2;
        Gauss_XOR gx(rows, m);
        int cnt = 0;
        for (int x = 0; x < (1 << m); x++) {
            bool ok = true;
            for (auto &[mask, b] : rows) ok &= __builtin_popcountll(mask & x) % 2 == b;
            cnt += ok;
        }
        set<unsigned long long> span;
        for (int s = 0; s < (1 << n); s++) {
            unsigned long long v = 0;
            for (int i = 0; i < n; i++)
                if (s >> i & 1) v ^= rows[i].first;
            span.insert(v);
        }
        int rk = __builtin_ctz(span.size());
        chk("Gauss_XOR.solvable", gx.solvable == (cnt > 0));
        chk("Gauss_XOR.rank", gx.rank == rk && (cnt == 0 || cnt == 1 << (m - rk)));
        if (cnt) {
            bool ok = (int)gx.answer.size() == m;
            for (auto &[mask, b] : rows) {
                int s = 0;
                for (int j = 0; j < m; j++) s ^= (mask >> j & 1) & gx.answer[j];
                ok &= s == b;
            }
            chk("Gauss_XOR.answer", ok);
        }
    }
    { // 64 columns, full width masks
        vector<pair<unsigned long long, int>> rows;
        vector<int> x(64);
        for (auto &v : x) v = rng() % 2;
        for (int i = 0; i < 100; i++) {
            unsigned long long mask = rng();
            int b = 0;
            for (int j = 0; j < 64; j++) b ^= (mask >> j & 1) & x[j];
            rows.push_back({mask, b});
        }
        Gauss_XOR gx(rows, 64);
        chk("Gauss_XOR(m=64)", gx.solvable && gx.rank == 64 && gx.answer == x);
    }
    {
        int n = 500;
        vector<vector<double>> aug(n, vector<double>(n + 1));
        vector<double> x(n);
        for (auto &v : x) v = (double)(rng() % 2001) - 1000;
        for (int i = 0; i < n; i++) {
            double s = 0;
            for (int j = 0; j < n; j++) aug[i][j] = (double)(rng() % 201) - 100, s += aug[i][j] * x[j];
            aug[i][n] = s;
        }
        auto t0 = clock();
        Gauss g(aug, n, n);
        vector<pair<unsigned long long, int>> rows(4000);
        for (auto &[mask, b] : rows) mask = rng(), b = rng() % 2;
        Gauss_XOR gx(rows, 64);
        double err = 0;
        for (int i = 0; i < n; i++) err = max(err, fabs(g.answer[i] - x[i]));
        printf("Gauss 500x500 + Gauss_XOR 4000x64: %.3fs (max err %.1e)\n", double(clock() - t0) / CLOCKS_PER_SEC, err);
        chk("Gauss(500x500)", g.kind == ONE_SOLUTION && err < 1e-6);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
