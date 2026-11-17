// Test for 06_Strings/Hashing_2D.cpp against a naive formula and explicit rectangle comparison.
// Every public method is checked: constructor (vector<string> and vector<vector<int>>), get_hash()
// and get_hash(x1, y1, x2, y2). Oracle: H = sum v[i][j] * PX^(h-1-i) * PY^(w-1-j) mod MOD with
// powers by repeated multiplication; the hash of a rectangle must equal the hash of a fresh Hash_2D
// built from the extracted rectangle; and hash equality must match content equality for every pair
// of equal-size rectangles (collisions are counted and reported).
#include "../../06_Strings/Hashing_2D.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(77);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

const ll MD = 998244353;
ll pwr(ll b, int e) {
    ll r = 1;
    while (e--) r = r * b % MD;
    return r;
}
template <class G> ll naive(const G &g, int x1, int y1, int x2, int y2) { // 1-based inclusive
    ll r = 0;
    int h = x2 - x1 + 1, w = y2 - y1 + 1;
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            ll v = ((ll)g[x1 - 1 + i][y1 - 1 + j] % MD + MD) % MD;
            r = (r + v * pwr(3731, h - 1 - i) % MD * pwr(2999, w - 1 - j)) % MD;
        }
    return r;
}
template <class G> G cut(const G &g, int x1, int y1, int x2, int y2) {
    G r;
    for (int i = x1 - 1; i < x2; i++) r.emplace_back(g[i].begin() + y1 - 1, g[i].begin() + y2);
    return r;
}

long long collisions = 0;
template <class G> void run(const G &g, const string &tag) {
    int n = g.size(), m = g[0].size();
    Hash_2D H(g);
    chk(tag + ":get_hash()", H.get_hash() == naive(g, 1, 1, n, m));
    vector<array<int, 4>> rects;
    for (int x1 = 1; x1 <= n; x1++)
        for (int x2 = x1; x2 <= n; x2++)
            for (int y1 = 1; y1 <= m; y1++)
                for (int y2 = y1; y2 <= m; y2++) rects.push_back({x1, y1, x2, y2});
    for (auto [x1, y1, x2, y2] : rects) {
        int h = H.get_hash(x1, y1, x2, y2);
        chk(tag + ":get_hash(rect)", h == naive(g, x1, y1, x2, y2) && h >= 0 && h < MD);
        chk(tag + ":rebuilt", h == Hash_2D(cut(g, x1, y1, x2, y2)).get_hash());
    }
    for (int q = 0; q < 200; q++) {
        auto a = rects[rng() % rects.size()];
        int hh = a[2] - a[0], ww = a[3] - a[1];
        int x = 1 + rng() % (n - hh), y = 1 + rng() % (m - ww); // same size, random place
        bool eq = cut(g, a[0], a[1], a[2], a[3]) == cut(g, x, y, x + hh, y + ww);
        bool heq = H.get_hash(a[0], a[1], a[2], a[3]) == H.get_hash(x, y, x + hh, y + ww);
        if (heq && !eq) collisions++;
        chk(tag + ":equal<->content equal", !eq || heq);
    }
}

int main() {
    for (int it = 0; it < 3000; it++) {
        int n = 1 + rng() % 6, m = 1 + rng() % 6, alpha = 1 + rng() % 3;
        vector<string> g(n, string(m, 'a'));
        for (auto &r : g)
            for (auto &c : r) c = 'a' + rng() % alpha;
        run(g, "string");
        vector<vector<int>> v(n, vector<int>(m));
        for (auto &r : v)
            for (auto &c : r) c = (int)(rng() % 5) - 2; // negatives and zero
        run(v, "int");
    }
    // a pattern inside a larger grid is found where it was planted
    for (int it = 0; it < 300; it++) {
        int n = 20 + rng() % 30, m = 20 + rng() % 30, p = 1 + rng() % 5, q = 1 + rng() % 5;
        vector<string> g(n, string(m, 'a')), pat(p, string(q, 'a'));
        for (auto &r : g)
            for (auto &c : r) c = 'a' + rng() % 2;
        for (auto &r : pat)
            for (auto &c : r) c = 'a' + rng() % 2;
        Hash_2D G(g), Pt(pat);
        int want = 0, got = 0;
        for (int i = 0; i + p <= n; i++)
            for (int j = 0; j + q <= m; j++) {
                bool eq = true;
                for (int a = 0; a < p && eq; a++) eq = g[i + a].compare(j, q, pat[a]) == 0;
                want += eq;
                bool h = G.get_hash(i + 1, j + 1, i + p, j + q) == Pt.get_hash();
                got += h;
                if (h && !eq) collisions++;
            }
        chk("pattern count", want == got);
    }
    printf("collisions seen: %lld\n", collisions);
    {
        int n = 2000, m = 2000;
        vector<string> g(n, string(m, 'a'));
        for (auto &r : g)
            for (auto &c : r) c = 'a' + rng() % 26;
        auto t0 = clock();
        Hash_2D H(g);
        ll s = 0;
        for (int i = 0; i < 1000000; i++) {
            int x1 = 1 + rng() % n, x2 = 1 + rng() % n, y1 = 1 + rng() % m, y2 = 1 + rng() % m;
            s += H.get_hash(min(x1, x2), min(y1, y2), max(x1, x2), max(y1, y2));
        }
        printf("2000x2000 build + 1e6 queries: %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, s % 10);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
