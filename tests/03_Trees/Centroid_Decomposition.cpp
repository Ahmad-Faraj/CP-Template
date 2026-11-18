// Test for 03_Trees/Centroid_Decomposition.cpp against the defining properties of a centroid tree,
// checked by brute BFS: every piece S(c) is connected in the original tree, removing c splits it into
// parts of size <= |S(c)|/2, and c's centroid-tree children are exactly those parts.
// Every public method is checked: add_edge, get_subtree_size, get_centroid, decompose.
#include "../../03_Trees/Centroid_Decomposition.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(7);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

vector<pair<int, int>> rand_tree(int n) {
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 1);
    shuffle(perm.begin(), perm.end(), rng);
    int shape = rng() % 4;
    vector<pair<int, int>> e;
    for (int i = 1; i < n; i++) {
        int p = shape == 0 ? i - 1 : shape == 1 ? 0 : (int)(rng() % i);
        e.push_back({perm[i], perm[p]});
    }
    return e;
}

// components of the graph induced on set S (given as a mask over nodes)
vector<vector<int>> comps(int n, const vector<vector<int>> &g, const vector<char> &in) {
    vector<char> seen(n + 1);
    vector<vector<int>> res;
    for (int s = 1; s <= n; s++) {
        if (!in[s] || seen[s]) continue;
        vector<int> c = {s};
        seen[s] = 1;
        for (int i = 0; i < (int)c.size(); i++)
            for (int v : g[c[i]])
                if (in[v] && !seen[v]) seen[v] = 1, c.push_back(v);
        sort(c.begin(), c.end());
        res.push_back(c);
    }
    return res;
}

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 14 + 1;
        auto e = rand_tree(n);
        vector<vector<int>> g(n + 1);
        centroid_decomposition cd(n);
        for (auto [u, v] : e) cd.add_edge(u, v), g[u].push_back(v), g[v].push_back(u);
        {   // get_subtree_size and get_centroid on the whole tree, rooted at r
            centroid_decomposition c2 = cd;
            int r = rng() % n + 1;
            chk("get_subtree_size(total)", c2.get_subtree_size(r, -1) == n);
            bool ok = true;
            for (int x = 1; x <= n; x++) { // brute: size of x's side when the edge to its parent is cut
                vector<char> in(n + 1, 1);
                // parent of x when rooted at r: neighbour of x on the r side
                if (x == r) { ok &= c2.sz[x] == n; continue; }
                int par = -1;
                for (auto &c : comps(n, g, [&] { auto t = in; t[x] = 0; return t; }()))
                    if (binary_search(c.begin(), c.end(), r))
                        for (int y : g[x])
                            if (binary_search(c.begin(), c.end(), y)) par = y;
                in[par] = 0;
                for (auto &c : comps(n, g, in))
                    if (binary_search(c.begin(), c.end(), x)) ok &= c2.sz[x] == (int)c.size();
            }
            chk("get_subtree_size(sz[])", ok);
            int c = c2.get_centroid(r, -1, n);
            vector<char> in(n + 1, 1);
            in[c] = 0;
            bool cen = true;
            for (auto &cc : comps(n, g, in)) cen &= 2 * (int)cc.size() <= n;
            chk("get_centroid", cen);
        }
        cd.decompose();
        // structure: one root, parent chain acyclic, depth <= log2(n) + 1
        int roots = 0, maxd = 0;
        bool ok = true;
        vector<vector<int>> ch(n + 1);
        for (int v = 1; v <= n; v++) {
            if (cd.par[v] == -1) roots++;
            else if (cd.par[v] < 1 || cd.par[v] > n) ok = false;
            else ch[cd.par[v]].push_back(v);
            int d = 0;
            for (int x = v; ok && x != -1; x = cd.par[x])
                if (++d > n) ok = false;
            maxd = max(maxd, d);
        }
        chk("decompose:one root", roots == 1 && ok);
        if (roots != 1 || !ok) continue;
        chk("decompose:depth<=log", (1 << (maxd - 1)) <= n);
        // pieces
        bool good = true;
        for (int c = 1; c <= n; c++) {
            vector<char> in(n + 1, 0);
            int sz = 0;
            for (int v = 1; v <= n; v++)
                for (int x = v; x != -1; x = cd.par[x])
                    if (x == c) { in[v] = 1, sz++; break; }
            good &= comps(n, g, in).size() == 1;
            in[c] = 0;
            auto parts = comps(n, g, in);
            vector<vector<int>> kids;
            for (int k : ch[c]) {
                vector<char> kin(n + 1, 0);
                for (int v = 1; v <= n; v++)
                    for (int x = v; x != -1; x = cd.par[x])
                        if (x == k) { kin[v] = 1; break; }
                vector<int> s;
                for (int v = 1; v <= n; v++)
                    if (kin[v]) s.push_back(v);
                kids.push_back(s);
            }
            for (auto &p : parts) good &= 2 * (int)p.size() <= sz;
            sort(parts.begin(), parts.end()), sort(kids.begin(), kids.end());
            good &= parts == kids;
        }
        chk("decompose:pieces", good);
    }
    for (int shape = 0; shape < 2; shape++) {
        int n = 200000;
        centroid_decomposition cd(n);
        for (int i = 2; i <= n; i++) cd.add_edge(i, shape ? i - 1 : (int)(rng() % (i - 1)) + 1);
        auto t0 = clock();
        cd.decompose();
        double t = double(clock() - t0) / CLOCKS_PER_SEC;
        int maxd = 0;
        for (int v = 1; v <= n; v++) {
            int d = 0;
            for (int x = v; x != -1; x = cd.par[x]) d++;
            maxd = max(maxd, d);
        }
        chk("large:depth<=18", maxd <= 18);
        printf("%s n=2e5: %.3fs (centroid depth %d)\n", shape ? "path" : "random", t, maxd);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
