// Test for 04_Graphs/Block_Cut_Tree.cpp against brute force on tiny graphs:
// cut vertices by deleting each vertex and recounting components; blocks by grouping edges that lie
// on a common simple cycle (enumerated exhaustively), plus isolated vertices as singleton blocks.
// Every public member is checked: add_edge, build, comps, is_cut, tree (shape, forest, and the
// "cut vertices strictly inside the tree path = vertices every u-v path must use" query).
#include "../../04_Graphs/Block_Cut_Tree.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(31);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int n, M;
vector<pair<int, int>> es;

int comps_without(int x) { // connected components of the graph minus vertex x (x = 0: none removed)
    vector<int> seen(n + 1, 0);
    int c = 0;
    for (int s = 1; s <= n; s++)
        if (s != x && !seen[s]) {
            c++;
            vector<int> q{s};
            seen[s] = 1;
            while (!q.empty()) {
                int u = q.back();
                q.pop_back();
                for (auto [a, b] : es)
                    for (int k = 0; k < 2; k++, swap(a, b))
                        if (a == u && b != x && !seen[b]) seen[b] = 1, q.push_back(b);
            }
        }
    return c;
}
bool connected_without(int u, int v, int x) {
    vector<int> seen(n + 1, 0), q{u};
    seen[u] = 1;
    while (!q.empty()) {
        int a0 = q.back();
        q.pop_back();
        for (auto [a, b] : es)
            for (int k = 0; k < 2; k++, swap(a, b))
                if (a == a0 && b != x && !seen[b]) seen[b] = 1, q.push_back(b);
    }
    return seen[v];
}

vector<int> par;
int fnd(int x) { return par[x] == x ? x : par[x] = fnd(par[x]); }
void cycles(int s, int cur, int vmask, int emask) { // simple cycles through s, union their edges
    for (int e = 0; e < M; e++) {
        if (emask >> e & 1) continue;
        auto [a, b] = es[e];
        if (a == b) continue;
        for (int k = 0; k < 2; k++, swap(a, b)) {
            if (a != cur) continue;
            if (b == s && emask) {
                int f = __builtin_ctz(emask);
                for (int g = 0; g < M; g++)
                    if ((emask | 1 << e) >> g & 1) par[fnd(g)] = fnd(f);
            } else if (!(vmask >> b & 1)) cycles(s, b, vmask | 1 << b, emask | 1 << e);
        }
    }
}

int main() {
    for (int it = 0; it < 30000; it++) {
        n = rng() % 7 + 1, M = rng() % 10;
        if (it % 10 == 0) M = 0;
        es.clear();
        Block_Cut_Tree B(n);
        for (int i = 0; i < M; i++) {
            int u = rng() % n + 1, v = rng() % n + 1;
            if (i && rng() % 6 == 0) u = es[i - 1].first, v = es[i - 1].second; // multi-edge
            es.push_back({u, v}), B.add_edge(u, v);
        }
        B.build();
        string t = M ? "" : "m=0:";
        // is_cut
        int base = comps_without(0);
        bool okc = (int)B.is_cut.size() == n + 1;
        for (int x = 1; x <= n; x++) {
            bool cut = false; // more components without x (an isolated x just disappears)
            bool iso = true;
            for (auto [a, b] : es) iso &= !((a == x || b == x) && a != b);
            cut = comps_without(x) > base - (iso ? 1 : 0);
            okc &= B.is_cut[x] == cut;
        }
        chk(t + "is_cut", okc);
        // comps
        par.resize(M);
        iota(par.begin(), par.end(), 0);
        for (int s = 1; s <= n; s++) cycles(s, s, 1 << s, 0);
        map<int, set<int>> grp;
        vector<int> touched(n + 1, 0);
        for (int e = 0; e < M; e++)
            if (es[e].first != es[e].second) {
                grp[fnd(e)].insert(es[e].first), grp[fnd(e)].insert(es[e].second);
                touched[es[e].first] = touched[es[e].second] = 1;
            }
        multiset<vector<int>> want, got;
        for (auto &[r, s] : grp) want.insert(vector<int>(s.begin(), s.end()));
        for (int v = 1; v <= n; v++)
            if (!touched[v]) want.insert({v});
        for (auto c : B.comps) sort(c.begin(), c.end()), got.insert(c);
        chk(t + "comps", got == want);
        // tree: shape and forest
        int N = n + 1 + B.comps.size();
        bool okt = (int)B.tree.size() == N;
        int tedges = 0;
        for (int b = 0; okt && b < (int)B.comps.size(); b++) {
            set<int> a(B.tree[n + 1 + b].begin(), B.tree[n + 1 + b].end());
            okt &= a == set<int>(B.comps[b].begin(), B.comps[b].end());
            tedges += B.tree[n + 1 + b].size();
        }
        for (int v = 1; okt && v <= n; v++) {
            int cnt = 0;
            for (auto &c : B.comps) cnt += count(c.begin(), c.end(), v);
            okt &= (int)B.tree[v].size() == cnt;
            for (int w : B.tree[v]) okt &= w > n;
        }
        vector<int> tc(N, -1);
        int tcomp = 0;
        for (int s = 1; s < N; s++)
            if (tc[s] == -1) {
                tcomp++;
                vector<int> q{s};
                tc[s] = s;
                while (!q.empty()) {
                    int u = q.back();
                    q.pop_back();
                    for (int w : B.tree[u])
                        if (tc[w] == -1) tc[w] = s, q.push_back(w);
                }
            }
        okt &= tedges == (N - 1) - tcomp && tcomp == base; // forest, one tree per graph component
        chk(t + "tree", okt);
        // query: cut vertices strictly inside the tree path u..v
        bool okq = true;
        for (int u = 1; u <= n; u++)
            for (int v = 1; v <= n; v++) {
                if (u == v || !connected_without(u, v, 0)) continue;
                vector<int> from(N, -1), q{u};
                from[u] = u;
                for (size_t i = 0; i < q.size(); i++)
                    for (int w : B.tree[q[i]])
                        if (from[w] == -1) from[w] = q[i], q.push_back(w);
                set<int> on;
                for (int x = from[v]; x != u; x = from[x])
                    if (x <= n) on.insert(x);
                set<int> must;
                for (int x = 1; x <= n; x++)
                    if (x != u && x != v && !connected_without(u, v, x)) must.insert(x);
                okq &= on == must;
            }
        chk(t + "tree path query", okq);
    }
    { // large: random graph plus a long path (deep recursion), n = 5e5, m = 5e5
        int n = 500000, m = 500000;
        Block_Cut_Tree B(n);
        for (int i = 1; i < n / 2; i++) B.add_edge(i, i + 1);
        for (int i = n / 2; i < m; i++) B.add_edge(rng() % n + 1, rng() % n + 1);
        auto t0 = clock();
        B.build();
        int cuts = 0;
        for (int v = 1; v <= n; v++) cuts += B.is_cut[v];
        printf("n=5e5 m=5e5 build: %.3fs (blocks=%zu cuts=%d)\n",
               double(clock() - t0) / CLOCKS_PER_SEC,
               B.comps.size(), cuts);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
