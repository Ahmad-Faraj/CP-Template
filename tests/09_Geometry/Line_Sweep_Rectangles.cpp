// Test for 09_Geometry/Line_Sweep_Rectangles.cpp against a coordinate-compressed grid and a
// unit-cell count. Every public method is checked: Rect_Union::area (also reused across calls),
// Rectangle::intersect, Rectangle constructors.
#include "../../09_Geometry/Line_Sweep_Rectangles.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(17);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }

Rectangle rnd_rect(ll C) {
    ll a = rnd(-C, C), b = rnd(-C, C), c = rnd(-C, C), d = rnd(-C, C);
    if (rng() % 8 == 0) c = a; // zero width
    if (rng() % 8 == 0) d = b; // zero height
    return Rectangle(min(a, c), min(b, d), max(a, c), max(b, d));
}

// compressed grid: every cell between consecutive distinct x's and y's, covered or not
__int128 brute_grid(const vector<Rectangle> &rs) {
    vector<ll> xs, ys;
    for (auto &r : rs) xs.push_back(r.x1), xs.push_back(r.x2), ys.push_back(r.y1), ys.push_back(r.y2);
    sort(xs.begin(), xs.end()), xs.erase(unique(xs.begin(), xs.end()), xs.end());
    sort(ys.begin(), ys.end()), ys.erase(unique(ys.begin(), ys.end()), ys.end());
    __int128 s = 0;
    for (int i = 0; i + 1 < (int)xs.size(); i++)
        for (int j = 0; j + 1 < (int)ys.size(); j++)
            for (auto &r : rs)
                if (r.x1 <= xs[i] && xs[i + 1] <= r.x2 && r.y1 <= ys[j] && ys[j + 1] <= r.y2) {
                    s += (__int128)(xs[i + 1] - xs[i]) * (ys[j + 1] - ys[j]);
                    break;
                }
    return s;
}

// unit cells [x, x+1] x [y, y+1], small coordinates only
ll brute_cells(const vector<Rectangle> &rs, ll C) {
    ll s = 0;
    for (ll x = -C; x < C; x++)
        for (ll y = -C; y < C; y++) {
            bool in = false;
            for (auto &r : rs) in |= r.x1 <= x && x + 1 <= r.x2 && r.y1 <= y && y + 1 <= r.y2;
            s += in;
        }
    return s;
}

int main() {
    Rect_Union shared; // reused across every call to catch stale state
    for (int it = 0; it < 30000; it++) {
        int n = rnd(0, 9);
        ll C = vector<ll>{2, 6, 1000000000}[rng() % 3];
        vector<Rectangle> rs;
        for (int i = 0; i < n; i++) {
            if (!rs.empty() && rng() % 6 == 0) rs.push_back(rs[rng() % rs.size()]); // duplicate
            else rs.push_back(rnd_rect(C));
        }
        ll got = shared.area(rs);
        Rect_Union fresh;
        chk("area(fresh==reused)", fresh.area(rs) == got);
        chk(C == 1000000000 ? "area vs grid(1e9)" : "area vs grid(small)", (__int128)got == brute_grid(rs));
        if (C <= 6) chk("area vs unit cells", got == brute_cells(rs, C));
    }
    {
        chk("area(edge)", shared.area({}) == 0);
        chk("area(edge)", shared.area({Rectangle(0, 5, 3, 5)}) == 0);
        chk("area(edge)", shared.area({Rectangle(2, 0, 2, 9), Rectangle(-1, 4, 7, 4)}) == 0);
        chk("area(edge)", shared.area({Rectangle(1, 1, 4, 3)}) == 6);
        ll M = 1000000000;
        chk("area(edge)", shared.area({Rectangle(-M, -M, M, M), Rectangle(0, 0, M, M)}) == 4 * M * M);
    }
    for (int it = 0; it < 200000; it++) {
        ll C = rng() % 2 ? 4 : 1000000000;
        Rectangle A = rnd_rect(C), B = rnd_rect(C), I = A.intersect(B);
        ll x1 = max(A.x1, B.x1), x2 = min(A.x2, B.x2), y1 = max(A.y1, B.y1), y2 = min(A.y2, B.y2);
        bool pos = x1 < x2 && y1 < y2;
        bool clip = I.x1 == x1 && I.y1 == y1 && I.x2 == x2 && I.y2 == y2;
        bool zero = I.x1 == 0 && I.y1 == 0 && I.x2 == 0 && I.y2 == 0;
        bool degen = A.x1 == A.x2 || A.y1 == A.y2 || B.x1 == B.x2 || B.y1 == B.y2;
        // a zero-width/height input may come back as its zero-area clipped overlap
        if (degen) chk("intersect(degenerate input)", zero || (clip && x1 <= x2 && y1 <= y2));
        else chk("intersect", pos ? clip : zero);
        if (pos) chk("intersect area vs union", shared.area({A}) + shared.area({B}) -
                                                   shared.area({A, B}) == (x2 - x1) * (y2 - y1));
    }
    {
        Rectangle d, e(1, 2, 3, 4);
        chk("Rectangle()", d.x1 == 0 && d.y1 == 0 && d.x2 == 0 && d.y2 == 0);
        chk("Rectangle(x1,y1,x2,y2)", e.x1 == 1 && e.y1 == 2 && e.x2 == 3 && e.y2 == 4);
    }
    {
        int n = 200000;
        vector<Rectangle> rs;
        for (int i = 0; i < n; i++) rs.push_back(rnd_rect(1000000000));
        auto t0 = clock();
        ll a = shared.area(rs);
        printf("area n=2e5 random rectangles in 1e9: %.3fs (area=%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, a);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
