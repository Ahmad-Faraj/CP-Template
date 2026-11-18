// Test for 03_Trees/Tree_Isomorphism.cpp against brute force over all n! relabelings (n <= 7).
// Every public function is checked: canonical_rooted (roots must map to each other), canonical_unrooted
// (any relabeling), node_rep (through canonical_unrooted and directly).
#include "../../03_Trees/Tree_Isomorphism.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(5);
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

vector<vector<int>> to_adj(int n, const vector<pair<int, int>> &e) {
    vector<vector<int>> g(n + 1);
    for (auto [u, v] : e) g[u].push_back(v), g[v].push_back(u);
    return g;
}

// is there a bijection p with p(edge set A) = edge set B (and p(ra) = rb if ra > 0)?
bool brute(int n, const vector<pair<int, int>> &a, const vector<pair<int, int>> &b, int ra, int rb) {
    set<pair<int, int>> eb;
    for (auto [u, v] : b) eb.insert({min(u, v), max(u, v)});
    vector<int> p(n + 1);
    iota(p.begin(), p.end(), 0);
    do {
        if (ra && p[ra] != rb) continue;
        bool ok = true;
        for (auto [u, v] : a) ok &= eb.count({min(p[u], p[v]), max(p[u], p[v])}) > 0;
        if (ok) return true;
    } while (next_permutation(p.begin() + 1, p.end()));
    return false;
}

int main() {
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 7 + 1, n2 = rng() % 10 == 0 ? rng() % 7 + 1 : n;
        auto a = rand_tree(n);
        vector<pair<int, int>> b;
        int ra = rng() % n + 1, rb = rng() % n2 + 1;
        if (n2 == n && rng() % 2) { // a relabeled copy, so isomorphic unrooted cases are common
            vector<int> p(n + 1);
            iota(p.begin(), p.end(), 0);
            shuffle(p.begin() + 1, p.end(), rng);
            for (auto [u, v] : a) b.push_back({p[u], p[v]});
            if (rng() % 2) rb = p[ra];
        } else
            b = rand_tree(n2);
        auto ga = to_adj(n, a), gb = to_adj(n2, b);
        bool un = n == n2 && brute(n, a, b, 0, 0), ro = n == n2 && brute(n, a, b, ra, rb);
        chk(un ? "canonical_unrooted(iso)" : "canonical_unrooted(non-iso)",
            (canonical_unrooted(ga, n) == canonical_unrooted(gb, n2)) == un);
        chk(ro ? "canonical_rooted(iso)" : "canonical_rooted(non-iso)",
            (canonical_rooted(ga, ra) == canonical_rooted(gb, rb)) == ro);
    }
    {
        vector<vector<string>> rep = {{}, {"(())", "()"}};
        chk("node_rep", node_rep(rep, 1) == "((())())");
        vector<vector<int>> g1(2), g2 = {{}, {2}, {1}};
        chk("n=1 / n=2", canonical_unrooted(g1, 1) == "()" && canonical_unrooted(g2, 2) == "(())" &&
                             canonical_rooted(g1, 1) == "()");
    }
    {   // the same random tree, relabeled, at contest size
        int n = 200000;
        auto a = rand_tree(n);
        vector<int> p(n + 1);
        iota(p.begin(), p.end(), 0);
        shuffle(p.begin() + 1, p.end(), rng);
        vector<pair<int, int>> b;
        for (auto [u, v] : a) b.push_back({p[u], p[v]});
        auto ga = to_adj(n, a), gb = to_adj(n, b);
        auto t0 = clock();
        bool same = canonical_unrooted(ga, n) == canonical_unrooted(gb, n);
        bool samer = canonical_rooted(ga, a[0].first) == canonical_rooted(gb, p[a[0].first]);
        chk("large relabeled", same && samer);
        printf("random-shape n=2e5 (2x unrooted + 2x rooted): %.3fs\n", double(clock() - t0) / CLOCKS_PER_SEC);
    }
    for (int n : {5000, 20000}) { // a path is the worst case for the string method: O(n^2) time AND memory
        vector<pair<int, int>> e;
        for (int i = 2; i <= n; i++) e.push_back({i - 1, i});
        auto g = to_adj(n, e);
        auto t0 = clock();
        size_t len = canonical_rooted(g, 1).size() + canonical_unrooted(g, n).size();
        printf("path n=%d (rooted at an end + unrooted): %.3fs (len %zu)\n", n,
               double(clock() - t0) / CLOCKS_PER_SEC, len);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-30s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
