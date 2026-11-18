// Test for 04_Graphs/Hungarian.cpp against brute force over every matching (all permutations for
// full square matrices). Every public method is checked: Hungarian(n1, n2) square and rectangular,
// add_edge (sparse, negative, multi-edge keeps the min), maximum_matching (cost, max count, l/r
// consistent), the max-cost and "max weight, any number of pairs" recipes from the header.
#include "../../04_Graphs/Hungarian.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(31);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

const long long NO = LLONG_MAX;
int n1, n2;
vector<vector<long long>> w; // NO = no edge
pair<int, long long> best;   // (max count, min cost at that count)
void rec(int u, vector<bool> &used, int cnt, long long cost) {
    if (u > n1) {
        if (cnt > best.first || (cnt == best.first && cost < best.second)) best = {cnt, cost};
        return;
    }
    rec(u + 1, used, cnt, cost);
    for (int v = 1; v <= n2; v++)
        if (!used[v] && w[u][v] != NO) {
            used[v] = true;
            rec(u + 1, used, cnt + 1, cost + w[u][v]);
            used[v] = false;
        }
}
long long best_any; // max weight over matchings of any size
void rec_any(int u, vector<bool> &used, long long val) {
    if (u > n1) return void(best_any = max(best_any, val));
    rec_any(u + 1, used, val);
    for (int v = 1; v <= n2; v++)
        if (!used[v] && w[u][v] != NO) used[v] = true, rec_any(u + 1, used, val + w[u][v]), used[v] = false;
}

// l / r consistency; returns (count, cost of the chosen pairs) or (-1, 0) if broken
pair<int, long long> read(Hungarian &M) {
    int cnt = 0;
    long long c = 0;
    vector<bool> used(max(n1, n2) + 1, false);
    for (int u = 1; u <= max(n1, n2); u++) {
        int v = M.l[u];
        if (!v) continue;
        if (u > n1 || v > n2 || used[v] || M.r[v] != u || w[u][v] == NO) return {-1, 0};
        used[v] = true, cnt++, c += w[u][v];
    }
    for (int v = 1; v <= max(n1, n2); v++)
        if (M.r[v] && M.l[M.r[v]] != v) return {-1, 0};
    return {cnt, c};
}

int main() {
    for (int it = 0; it < 40000; it++) {
        n1 = rng() % 6 + 1, n2 = rng() % 6 + 1;
        int kind = rng() % 3; // 0 full, 1 sparse, 2 sparse + multi-edges
        if (kind == 0 && rng() % 2) n2 = n1;
        long long lo = vector<long long>{0, -50, -1000000000}[rng() % 3];
        long long span = lo == -1000000000 ? 2000000001 : 101;
        w.assign(n1 + 1, vector<long long>(n2 + 1, NO));
        Hungarian M(n1, n2);
        for (int u = 1; u <= n1; u++)
            for (int v = 1; v <= n2; v++) {
                int times = kind == 0 ? 1 : kind == 1 ? rng() % 2 : rng() % 3;
                for (int k = 0; k < times; k++) {
                    long long c = lo + (long long)(rng() % span);
                    M.add_edge(u, v, c);
                    w[u][v] = w[u][v] == NO ? c : min(w[u][v], c);
                }
            }
        best = {0, 0};
        vector<bool> used(n2 + 1, false);
        rec(1, used, 0, 0);
        long long got = M.maximum_matching();
        auto [cnt, c] = read(M);
        string tag = n1 == n2 ? "square " : "rect ";
        tag += kind == 0 ? "full" : kind == 1 ? "sparse" : "multi-edge";
        chk("maximum_matching " + tag, got == best.second && cnt == best.first && c == got);
    }
    // max cost by negating
    for (int it = 0; it < 10000; it++) {
        n1 = rng() % 6 + 1, n2 = rng() % 6 + 1;
        w.assign(n1 + 1, vector<long long>(n2 + 1, NO));
        Hungarian M(n1, n2);
        for (int u = 1; u <= n1; u++)
            for (int v = 1; v <= n2; v++)
                if (rng() % 3) {
                    long long c = (long long)(rng() % 201) - 100;
                    w[u][v] = -c, M.add_edge(u, v, -c);
                }
        best = {0, 0};
        vector<bool> used(n2 + 1, false);
        rec(1, used, 0, 0);
        chk("max cost (negate)", -M.maximum_matching() == -best.second);
    }
    // max weight with any number of pairs: 0 for every pair, -w for real edges
    for (int it = 0; it < 10000; it++) {
        n1 = rng() % 6 + 1, n2 = rng() % 6 + 1;
        w.assign(n1 + 1, vector<long long>(n2 + 1, NO));
        Hungarian M(n1, n2);
        for (int u = 1; u <= n1; u++)
            for (int v = 1; v <= n2; v++) {
                M.add_edge(u, v, 0);
                if (rng() % 2) {
                    long long x = (long long)(rng() % 201) - 100;
                    w[u][v] = x, M.add_edge(u, v, -x);
                }
            }
        best_any = 0;
        vector<bool> used(n2 + 1, false);
        rec_any(1, used, 0);
        chk("max weight any count (recipe)", -M.maximum_matching() == best_any);
    }
    // full square n = 8 against all permutations
    for (int it = 0; it < 30; it++) {
        int n = 8;
        n1 = n2 = n;
        vector<vector<long long>> a(n + 1, vector<long long>(n + 1));
        Hungarian M(n, n);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) a[i][j] = (long long)(rng() % 2001) - 1000, M.add_edge(i, j, a[i][j]);
        vector<int> p(n);
        iota(p.begin(), p.end(), 1);
        long long bp = NO;
        do {
            long long s = 0;
            for (int i = 0; i < n; i++) s += a[i + 1][p[i]];
            bp = min(bp, s);
        } while (next_permutation(p.begin(), p.end()));
        long long got = M.maximum_matching();
        long long s = 0;
        set<int> cols;
        for (int i = 1; i <= n; i++) s += a[i][M.l[i]], cols.insert(M.l[i]);
        chk("all permutations n=8", got == bp && s == bp && cols.size() == 8 && !cols.count(0));
    }
    // sample: Library Checker assignment example_00 -> 9
    {
        vector<vector<long long>> a = {{4, 3, 5}, {3, 5, 9}, {4, 1, 4}};
        Hungarian M(3, 3);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) M.add_edge(i + 1, j + 1, a[i][j]);
        long long x = M.maximum_matching(), s = 0;
        for (int i = 1; i <= 3; i++) s += a[i - 1][M.l[i] - 1];
        chk("samples", x == 9 && s == 9);
    }
    {
        int n = 500;
        Hungarian M(n, n);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) M.add_edge(i, j, (long long)(rng() % 2000000001) - 1000000000);
        auto t0 = clock();
        long long x = M.maximum_matching();
        printf("assignment n=500 |a|<=1e9: %.3fs (cost=%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, x);
        Hungarian P(n, n);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) P.add_edge(i, j, -(long long)i * j * 1000);
        t0 = clock();
        x = P.maximum_matching();
        printf("multiplication table n=500: %.3fs (cost=%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, x);
        Hungarian S(n, n);
        for (int i = 1; i <= n; i++)
            for (int k = 0; k < 3; k++) S.add_edge(i, rng() % n + 1, rng() % 1000);
        t0 = clock();
        x = S.maximum_matching();
        int cnt = 0;
        for (int i = 1; i <= n; i++) cnt += S.l[i] != 0;
        printf("sparse n=500 deg 3: %.3fs (pairs=%d cost=%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC,
               cnt, x);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-34s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
