// Test for 02_Range_Query/Segment_Tree_Dynamic_2D.cpp against a std::map of assigned cells.
// Every public method is checked: constructor (any bounds, negative too),
// update (assign, overwrite, assign 0), query (random, single cell, whole space,
// empty x1 > x2 / y1 > y2, bounds beyond the space).
#include "../../02_Range_Query/Segment_Tree_Dynamic_2D.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(2026);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return uniform_int_distribution<ll>(lo, hi)(rng); }

ll brute(const map<pair<int, int>, ll> &cells, int x1, int y1, int x2, int y2) {
    ll s = 0;
    for (auto &[p, v] : cells)
        if (x1 <= p.first && p.first <= x2 && y1 <= p.second && p.second <= y2) s += v;
    return s;
}

int main() {
    for (int it = 0; it < 6000; it++) {
        // bounds: tiny grid (dense collisions), mid, or the full +-1e9 range
        int kind = it % 3;
        int rmin = kind == 0 ? (int)rnd(-3, 3) : kind == 1 ? (int)rnd(-1000, 0) : -1000000000;
        int rmax = kind == 0 ? rmin + (int)rnd(0, 6) : kind == 1 ? (int)rnd(0, 1000) : 1000000000;
        int cmin = kind == 0 ? (int)rnd(-3, 3) : kind == 1 ? (int)rnd(-1000, 0) : -1000000000;
        int cmax = kind == 0 ? cmin + (int)rnd(0, 6) : kind == 1 ? (int)rnd(0, 1000) : 1000000000;
        if (kind == 2 && it % 2) rmin = cmin = 0; // the classic [0, 1e9] use
        SegTree2D st(rmin, rmax, cmin, cmax);
        map<pair<int, int>, ll> cells;
        vector<pair<int, int>> used;
        auto rx = [&]() { return (int)rnd(rmin, rmax); };
        auto ry = [&]() { return (int)rnd(cmin, cmax); };
        int q = (int)rnd(1, 50);
        for (int s = 0; s < q; s++) {
            if (rng() % 2) {
                int x, y;
                if (!used.empty() && rng() % 3 == 0) tie(x, y) = used[rng() % used.size()];
                else x = rx(), y = ry(), used.push_back({x, y});
                ll v = rng() % 5 == 0 ? 0 : rnd(-1000000000000LL, 1000000000000LL);
                st.update(x, y, v);
                cells[{x, y}] = v;
                chk("update (cell reads back)", st.query(x, y, x, y) == v);
            } else {
                int x1 = rx(), x2 = rx(), y1 = ry(), y2 = ry();
                if (rng() % 8) {
                    if (x1 > x2) swap(x1, x2);
                    if (y1 > y2) swap(y1, y2);
                }
                bool empty = x1 > x2 || y1 > y2;
                chk(empty ? "query (empty rect)" : "query", st.query(x1, y1, x2, y2) ==
                                                                brute(cells, x1, y1, x2, y2));
                if (!used.empty()) { // a rectangle hugging an assigned point
                    auto [px, py] = used[rng() % used.size()];
                    chk("query (edge through a point)",
                        st.query(px, cmin, px, py) == brute(cells, px, cmin, px, py) &&
                            st.query(rmin, py, px, cmax) == brute(cells, rmin, py, px, cmax));
                }
            }
        }
        chk("query (whole space)",
            st.query(rmin, cmin, rmax, cmax) == brute(cells, rmin, cmin, rmax, cmax));
        chk("query (bounds beyond the space)",
            st.query(INT_MIN, INT_MIN, INT_MAX, INT_MAX) == brute(cells, rmin, cmin, rmax, cmax));
        delete st.root;
    }
    {
        int n = 100000, q = 100000;
        auto t0 = clock();
        SegTree2D st(0, 1000000000, 0, 1000000000);
        ll sink = 0;
        for (int i = 0; i < n; i++) st.update((int)rnd(0, 1e9), (int)rnd(0, 1e9), rnd(0, 1e9));
        double tu = double(clock() - t0) / CLOCKS_PER_SEC;
        for (int i = 0; i < q; i++) {
            int x1 = (int)rnd(0, 1e9), x2 = (int)rnd(x1, 1e9);
            int y1 = (int)rnd(0, 1e9), y2 = (int)rnd(y1, 1e9);
            sink += st.query(x1, y1, x2, y2);
        }
        printf("timing [0,1e9]^2: 1e5 updates %.3fs, then 1e5 queries %.3fs (sink %lld)\n", tu,
               double(clock() - t0) / CLOCKS_PER_SEC - tu, sink & 1);
        delete st.root;
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
