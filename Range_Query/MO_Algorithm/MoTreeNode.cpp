/*
    [1] Definition:
    Mo's Algorithm on Trees answers path queries [u, v] offline.
    It flattens a tree into a 1D array using Euler Tour (Start/End times),
    converting tree path queries into 1D range queries.

    [2] Time & Space Complexity:
    - Build Time: O(N log N)
    - Query Time: O((N + Q) * sqrt(N))
    - Space Complexity: O(N log N + Q)

    [3] Important Notes:
    - Uses 1-based indexing.
    - VAL_ON_EDGE = false for node values, true for edge values.
    - Uses Hilbert Curve ordering for fast query sorting.
    - Currently tracks unique node values on the tree path.
*/

#include "../../core.h"

// 1-based Indexing
template <typename T = int, bool VAL_ON_EDGE = false>
class MoTree {
   public:
    struct Query {
        int l, r, k, lca, queryIdx;
        int64_t ord;

        // Maps tree path query between nodes L and R to 1D range [l, r]
        Query(vector<T>& S, vector<T>& E, int L = 0, int R = 0, int QueryIdx = 0, int LCA = 0, int HilbertPow = 0) {
            if (S[L] > S[R]) swap(L, R);
            if (LCA == L)
                l = S[L] + VAL_ON_EDGE, r = S[R], lca = -1, queryIdx = QueryIdx;
            else
                l = E[L], r = S[R], lca = LCA, queryIdx = QueryIdx;
            calcOrder(HilbertPow);
        }

        // Calculates Hilbert order for fast query sorting
        void calcOrder(int hilbert_pow) { ord = MoTree::hilbertOrder(l, r, hilbert_pow, 0); }

        bool operator<(const Query& rhs) const { return ord < rhs.ord; }
    };

    MoTree(int N, int M, vector<vector<int>>& G, vector<T>& V, int root = 1)
        : curr_l(1), curr_r(0), n(N), m(M), SqrtN(n / sqrt(m) + 1), timer(1), ans(0), answers(M), val(V), adj(G) {
        LOG = calcLog(N);
        helbertPow = calcHilbertPow(2 * N + 1);
        nodeFreq = S = E = dep = vector<int>(n + 5);
        FT = vector<int>(2 * n + 5);
        anc = vector<vector<int>>(n + 5, vector<int>(LOG));
        freq = vector<int>(n + 5);
        dfs(root);
    }

    // Maps 2D coordinate (x, y) to a 1D Hilbert Curve index for optimal sorting
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

    // Reads queries from input and processes them
    void getData() {
        for (int i = 0, u, v; i < m && cin >> u >> v; i++)
            queries.emplace_back(S, E, u, v, i, getLCA(u, v), helbertPow);
        process();
    }

    // Processes all queries offline
    void process() {
        sort(queries.begin(), queries.end());

        curr_l = queries[0].l, curr_r = queries[0].l - 1;

        for (auto& q : queries) {
            setRange(q);

            // If LCA is -1, nodes are in the same subtree
            if (~q.lca && !VAL_ON_EDGE) add(q.lca);

            answers[q.queryIdx] = ans;

            if (~q.lca && !VAL_ON_EDGE) remove(q.lca);
        }
    }

    vector<T> getAnswers() const { return answers; }

   private:
    int curr_l, curr_r, n, m, SqrtN, timer, LOG, helbertPow;
    vector<T> answers, val;
    vector<int> dep, S, E, FT, nodeFreq;
    vector<vector<int>> adj, anc;
    vector<Query> queries;

    // Euler Tour DFS to flatten tree and build binary lifting table
    void dfs(int u, int p = -1) {
        S[u] = timer;
        FT[timer++] = u;
        for (auto& v : adj[u]) {
            if (v == p) continue;
            dep[v] = dep[u] + 1;
            anc[v][0] = u;
            for (int bit = 1; bit < LOG; bit++) anc[v][bit] = anc[anc[v][bit - 1]][bit - 1];
            dfs(v, u);
        }
        E[u] = timer;
        FT[timer++] = u;
    }

    // Returns k-th ancestor of node u
    int kthAncestor(int u, int k) const {
        if (dep[u] < k) return -1;
        for (int bit = LOG - 1; bit >= 0; bit--)
            if (k & (1 << bit)) u = anc[u][bit];
        return u;
    }

    // Returns Lowest Common Ancestor (LCA) of u and v
    int getLCA(int u, int v) const {
        if (dep[u] < dep[v]) swap(u, v);
        u = kthAncestor(u, dep[u] - dep[v]);
        if (u == v) return u;
        for (int bit = LOG - 1; bit >= 0; bit--)
            if (anc[u][bit] != anc[v][bit]) u = anc[u][bit], v = anc[v][bit];
        return anc[u][0];
    }

    // Adjusts current range [curr_l, curr_r] to match query [q.l, q.r]
    void setRange(Query& q) {
        while (curr_l > q.l) operation(--curr_l);
        while (curr_r < q.r) operation(++curr_r);
        while (curr_l < q.l) operation(curr_l++);
        while (curr_r > q.r) operation(curr_r--);
    }

    vector<int> freq;
    T ans;

    // Adds node u value to current path state
    void add(int u) {
        freq[val[u]]++;
        if (freq[val[u]] == 1) ans++;
    }

    // Removes node u value from current path state
    void remove(int u) {
        freq[val[u]]--;
        if (freq[val[u]] == 0) ans--;
    }

    // Toggles node u presence on the path
    void operation(int idx) {
        int u = FT[idx];
        nodeFreq[u] ^= 1;
        if (nodeFreq[u] == 1) {
            add(u);
        } else {
            remove(u);
        }
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