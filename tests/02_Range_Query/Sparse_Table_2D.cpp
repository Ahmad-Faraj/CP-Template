// Test for 02_Range_Query/Sparse_Table_2D.cpp against a brute-force max over the submatrix.
// Every public method is checked: constructor (int and long long, 1 x 1 up to 33 x 33 grids,
// non-square, negatives, duplicates) and query over EVERY rectangle of each small grid.
#include "../../02_Range_Query/Sparse_Table_2D.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(7);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
void chk_n(const string &m, long long checks, long long bad) {
    cov[m].first += checks, cov[m].second += bad;
}
long long rnd(long long lo, long long hi) {
    return uniform_int_distribution<long long>(lo, hi)(rng);
}

template <typename T> void run(const string &tag, T V, int iters, int maxn) {
    for (int it = 0; it < iters; it++) {
        int n = (int)rnd(1, maxn), m = (int)rnd(1, maxn);
        if (it % 5 == 0) n = 1;
        if (it % 7 == 0) m = 1;
        T lo = rng() % 3 ? -V : (T)rnd(-3, 0), hi = lo == -V ? V : (T)rnd(0, 3);
        vector<vector<T>> g(n, vector<T>(m));
        for (auto &r : g)
            for (auto &x : r) x = (T)rnd(lo, hi);
        SparseTable2D<T> st(g);
        long long checks = 0, bad = 0;
        for (int x1 = 0; x1 < n; x1++)
            for (int x2 = x1; x2 < n; x2++) {
                // running column-wise max over rows x1..x2, then over columns y1..y2
                vector<T> col(m, numeric_limits<T>::min());
                for (int i = x1; i <= x2; i++)
                    for (int j = 0; j < m; j++) col[j] = max(col[j], g[i][j]);
                for (int y1 = 0; y1 < m; y1++) {
                    T best = numeric_limits<T>::min();
                    for (int y2 = y1; y2 < m; y2++) {
                        best = max(best, col[y2]);
                        checks++, bad += st.query(x1, y1, x2, y2) != best;
                    }
                }
            }
        chk_n(tag + " query (all rectangles)", checks, bad);
    }
}

int main() {
    run<int>("int", 1000000000, 3000, 12);
    run<int>("int", 5, 3000, 9);
    run<long long>("ll", (long long)4e18, 1500, 12);
    run<int>("int big", 1000000000, 60, 33);
    {
        int n = 500, m = 500, q = 1000000;
        vector<vector<int>> g(n, vector<int>(m));
        for (auto &r : g)
            for (auto &x : r) x = (int)rnd(-1e9, 1e9);
        auto t0 = clock();
        SparseTable2D<int> st(g);
        long long sink = 0;
        for (int i = 0; i < q; i++) {
            int x1 = (int)rnd(0, n - 1), x2 = (int)rnd(x1, n - 1);
            int y1 = (int)rnd(0, m - 1), y2 = (int)rnd(y1, m - 1);
            sink += st.query(x1, y1, x2, y2);
        }
        printf("timing 500x500 build + 1e6 queries: %.3fs (sink %lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, sink & 1);
        for (int i = 0; i < 300; i++) { // spot-check the big grid too
            int x1 = (int)rnd(0, n - 1), x2 = (int)rnd(x1, n - 1);
            int y1 = (int)rnd(0, m - 1), y2 = (int)rnd(y1, m - 1);
            int best = INT_MIN;
            for (int a = x1; a <= x2; a++)
                for (int b = y1; b <= y2; b++) best = max(best, g[a][b]);
            chk("int 500x500 query", st.query(x1, y1, x2, y2) == best);
        }
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first,
               c.second);
        ok &= c.first > 0 && c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
