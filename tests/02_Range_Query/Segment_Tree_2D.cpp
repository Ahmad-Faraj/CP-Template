// Test for 02_Range_Query/Segment_Tree_2D.cpp against summing the rectangle of a plain grid.
// Every public method is checked: both constructors, build (incl. a grid smaller than n x m),
// update, query (incl. empty ranges), merge.
#include "../../02_Range_Query/Segment_Tree_2D.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(41);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int brute(const vector<vector<int>> &g, int x1, int x2, int y1, int y2) {
    int s = 0;
    for (int i = x1; i <= x2; i++)
        for (int j = y1; j <= y2; j++) s += g[i - 1][j - 1];
    return s;
}

int main() {
    { // grid of the CSES 1739 (Forest Queries II) sample, '*' = 1, values checked by hand
        vector<string> f = {".*..", "*.**", "**..", "****"};
        vector<vector<int>> g(4, vector<int>(4));
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) g[i][j] = f[i][j] == '*';
        SegTree2D st(4, 4, g);
        vector<int> got;
        got.push_back(st.query(2, 3, 2, 4)); // "2 2 3 4" -> 3
        st.update(3, 3, 1);                  // "1 3 3": cell was '.', now a tree
        got.push_back(st.query(2, 3, 2, 4)); // -> 4
        got.push_back(st.query(1, 4, 1, 4)); // "2 1 1 4 4" -> 11 (10 + new tree)
        chk("example", got == vector<int>{3, 4, 11});
    }
    for (int it = 0; it < 6000; it++) {
        int n = rng() % 7 + 1, m = rng() % 7 + 1;
        vector<vector<int>> g(n, vector<int>(m, 0));
        int mode = rng() % 3;
        SegTree2D st(1, 1);
        string tag;
        if (mode == 0) { // (n, m): all zeros
            st = SegTree2D(n, m), tag = "SegTree2D(n, m)";
        } else if (mode == 1) { // (n, m, a): full grid
            for (auto &row : g)
                for (auto &x : row) x = (int)(rng() % 21) - 10;
            st = SegTree2D(n, m, g), tag = "SegTree2D(n, m, a)";
        } else { // build() from a smaller grid: the rest stays 0
            int r = rng() % n + 1, c = rng() % m + 1;
            vector<vector<int>> small(r, vector<int>(c));
            for (int i = 0; i < r; i++)
                for (int j = 0; j < c; j++) g[i][j] = small[i][j] = (int)(rng() % 21) - 10;
            st = SegTree2D(n, m), st.build(small), tag = "build(smaller grid)";
        }
        int q = rng() % 30;
        for (int i = 0; i < q; i++) {
            if (rng() % 3 == 0) {
                int x = rng() % n + 1, y = rng() % m + 1, v = (int)(rng() % 2001) - 1000;
                g[x - 1][y - 1] = v, st.update(x, y, v);
                chk("update", st.query(x, x, y, y) == v);
                continue;
            }
            int x1 = rng() % n + 1, x2 = rng() % n + 1, y1 = rng() % m + 1, y2 = rng() % m + 1;
            if (x1 > x2) swap(x1, x2);
            if (y1 > y2) swap(y1, y2);
            int b = brute(g, x1, x2, y1, y2);
            chk("query", st.query(x1, x2, y1, y2) == b);
            chk(tag, st.query(x1, x2, y1, y2) == b);
            if (x1 < x2) chk("query(empty)", st.query(x2, x1, y1, y2) == st.Default);
            if (y1 < y2) chk("query(empty)", st.query(x1, x2, y2, y1) == st.Default);
        }
        chk("merge", st.merge(3, -5) == -2 && st.merge(st.Default, 7) == 7);
    }
    { // timing: 1000 x 1000 build + 2e5 mixed ops (CSES 1739 limits)
        int n = 1000, m = 1000;
        vector<vector<int>> g(n, vector<int>(m));
        for (auto &row : g)
            for (auto &x : row) x = rng() % 2;
        auto t0 = clock();
        SegTree2D st(n, m, g);
        long long s = 0;
        for (int i = 0; i < 200000; i++) {
            int x1 = rng() % n + 1, x2 = rng() % n + 1, y1 = rng() % m + 1, y2 = rng() % m + 1;
            if (i & 1) st.update(x1, y1, rng() % 2);
            else s += st.query(min(x1, x2), max(x1, x2), min(y1, y2), max(y1, y2));
        }
        printf("1000x1000 build + 2e5 ops: %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
