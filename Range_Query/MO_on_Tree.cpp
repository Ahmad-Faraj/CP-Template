// MO on Tree: offline path queries on a tree - by default, the number of distinct values on the path u..v.
// Use when: "q queries each asking something about the path between u and v", no updates, ranges cannot be merged.
// Handles: values on nodes (default) or on edges via MoTree<int, true>. Hilbert-order sorted. Offline only.
// Time: O((n + q) * sqrt(q)) calls to add/remove
// Indexing: 1-based nodes; answers come back in the order the queries were added
// Note: values must be small non-negative ints - compress them first, as solve() does. Edit add()/remove() per problem.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = int, bool VAL_ON_EDGE = false> class MoTree {
  public:
    struct Query {
        int l, r, lca, queryIdx;
        int64_t ord;

        Query(vector<int> &S, vector<int> &E, int L, int R, int QueryIdx, int LCA, int HilbertPow) {
            if (S[L] > S[R]) swap(L, R);
            if (LCA == L)
                l = S[L] + VAL_ON_EDGE, r = S[R], lca = -1, queryIdx = QueryIdx;
            else
                l = E[L], r = S[R], lca = LCA, queryIdx = QueryIdx;
            ord = MoTree::hilbertOrder(l, r, HilbertPow, 0);
        }

        bool operator<(const Query &rhs) const { return ord < rhs.ord; }
    };

    // G[u] holds {v, w}. With VAL_ON_EDGE the w of the edge into v becomes val[v]; otherwise pass V per node.
    MoTree(int N, int M, vector<vector<pair<int, int>>> &G, vector<T> V = vector<T>(), int root = 1)
        : curr_l(1), curr_r(0), n(N), m(M), timer(1), ans(0), answers(M), val(V), adj(G) {
        LOG = calcLog(N);
        helbertPow = calcHilbertPow(2 * N + 1);
        nodeFreq = S = E = dep = vector<int>(n + 5);
        FT = vector<int>(2 * n + 5);
        anc = vector<vector<int>>(n + 5, vector<int>(LOG));
        if ((int)val.size() < n + 5) val.resize(n + 5, 0);
        dfs(root);
        freq.assign((size_t)*max_element(val.begin(), val.end()) + 2, 0);
    }

    static inline int64_t hilbertOrder(int x, int y, int pow, int rotate) {
        if (pow == 0) return 0;
        int hpow = 1 << (pow - 1);
        int seg = (x < hpow) ? ((y < hpow) ? 0 : 3) : ((y < hpow) ? 1 : 2);
        seg = (seg + rotate) & 3;
        const int rotateDelta[4] = {3, 0, 0, 1};
        int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
        int nrot = (rotate + rotateDelta[seg]) & 3;
        int64_t subSquareSize = int64_t(1) << (2 * pow - 2);
        int64_t ordd = seg * subSquareSize;
        int64_t add = hilbertOrder(nx, ny, pow - 1, nrot);
        ordd += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
        return ordd;
    }

    void add_query(int u, int v) { // ask about the path u..v
        queries.emplace_back(S, E, u, v, (int)queries.size(), getLCA(u, v), helbertPow);
    }

    void read_queries() { // read m queries "u v" from stdin
        for (int i = 0, u, v; i < m && cin >> u >> v; i++) add_query(u, v);
    }

    vector<T> run() { // answers indexed by the order add_query() was called
        sort(queries.begin(), queries.end());
        curr_l = queries[0].l, curr_r = queries[0].l - 1;
        for (auto &q : queries) {
            setRange(q);
            if (~q.lca && !VAL_ON_EDGE) add(q.lca);
            answers[q.queryIdx] = ans;
            if (~q.lca && !VAL_ON_EDGE) remove(q.lca);
        }
        return answers;
    }

  private:
    int curr_l, curr_r, n, m, timer, LOG, helbertPow;
    T ans;
    vector<T> answers, val;
    vector<int> dep, S, E, FT, nodeFreq, freq;
    vector<vector<pair<int, int>>> adj;
    vector<vector<int>> anc;
    vector<Query> queries;

    void dfs(int u, int p = -1) {
        S[u] = timer;
        FT[timer++] = u;
        for (auto &[v, w] : adj[u]) {
            if (v == p) continue;
            dep[v] = dep[u] + 1;
            anc[v][0] = u;
            for (int bit = 1; bit < LOG; bit++) anc[v][bit] = anc[anc[v][bit - 1]][bit - 1];
            if (VAL_ON_EDGE) val[v] = w;
            dfs(v, u);
        }
        E[u] = timer;
        FT[timer++] = u;
    }

    int kthAncestor(int u, int k) const {
        if (dep[u] < k) return -1;
        for (int bit = LOG - 1; bit >= 0; bit--)
            if (k & (1 << bit)) u = anc[u][bit];
        return u;
    }

    int getLCA(int u, int v) const {
        if (dep[u] < dep[v]) swap(u, v);
        u = kthAncestor(u, dep[u] - dep[v]);
        if (u == v) return u;
        for (int bit = LOG - 1; bit >= 0; bit--)
            if (anc[u][bit] != anc[v][bit]) u = anc[u][bit], v = anc[v][bit];
        return anc[u][0];
    }

    void setRange(Query &q) {
        while (curr_l > q.l) operation(--curr_l);
        while (curr_r < q.r) operation(++curr_r);
        while (curr_l < q.l) operation(curr_l++);
        while (curr_r > q.r) operation(curr_r--);
    }

    inline void add(int u) { // bring node u's value onto the path
        freq[val[u]]++;
        if (freq[val[u]] == 1) ans++;
    }

    inline void remove(int u) { // drop node u's value from the path
        freq[val[u]]--;
        if (freq[val[u]] == 0) ans--;
    }

    inline void operation(int idx) {
        int u = FT[idx];
        nodeFreq[u] ^= 1;
        if (nodeFreq[u] == 1)
            add(u);
        else
            remove(u);
    }

    int calcLog(int max_n) const {
        int log = 0;
        while ((1 << log) <= max_n) log++;
        return log;
    }

    int calcHilbertPow(int max_n) const {
        int pow = 0;
        while ((1 << pow) < max_n) pow++;
        return pow;
    }
};

// Standard problem: count distinct values on the path u..v, for q queries (SPOJ COT2)
void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> G(n + 1);
    vector<int> V(n + 1), compress;
    for (int i = 1; i <= n; i++) cin >> V[i], compress.push_back(V[i]);
    sort(compress.begin(), compress.end());
    compress.resize(unique(compress.begin(), compress.end()) - compress.begin());
    for (int i = 1; i <= n; i++) V[i] = lower_bound(compress.begin(), compress.end(), V[i]) - compress.begin();
    for (int i = 1, u, v; i < n && cin >> u >> v; i++) G[u].push_back({v, 0}), G[v].push_back({u, 0});

    MoTree<int> mo(n, m, G, V); // MoTree<int, true> instead, with real edge weights in G, for values on edges
    mo.read_queries();
    for (auto &x : mo.run()) cout << x << '\n';
}
