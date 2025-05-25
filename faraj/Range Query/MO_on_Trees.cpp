#include <bits/stdc++.h>
using namespace std;

int N = 1e6 + 5;

// solve Count on a tree II problem

template <typename T = int>
struct MO_Tree
{

    static inline int64_t hilbertOrder(int x, int y, int pow, int rotate)
    {
        if (pow == 0)
            return 0;
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

    struct query
    {

        int l, r, lca, query_idx;
        int64_t ord;
        int block_idx; // Add this line to define block_idx

        query(vector<T> &S, vector<T> &E, int L = 0, int R = 0, int Query_idx = 0, int LCA = 0)
        {
            if (S[L] > S[R])
                swap(L, R);
            if (LCA == L)
                l = S[L], r = S[R], lca = -1, query_idx = Query_idx;
            else
                l = E[L], r = S[R], lca = LCA, query_idx = Query_idx;
            calcOrder();
        }

        query() {}

        inline void calcOrder()
        {
            constexpr int K = 19;
            // K should be minimum such that 2^K >= n
            ord = hilbertOrder(l, r, K, 0);
        }

        bool operator<(const query &rhs) const
        {
            return ord < rhs.ord;
        }
    };

    T curr_l, curr_r, ans, n, m, Sqrt_N, timer, LOG;
    vector<T> answers, val;
    vector<vector<T>> adj, anc;
    vector<T> dep, S, E, FT, node_freq;
    vector<query> queries;
    int cnt[N];
    MO_Tree() {}

    MO_Tree(int N, int M, vector<vector<T>> &G, vector<T> &V, int root = 1)
    {

        curr_l = 1, curr_r = 0, ans = 0, n = N, m = M, Sqrt_N = sqrt(n);
        LOG = 0, timer = 1;

        while ((1 << LOG) <= n)
            LOG++;

        queries = vector<query>(m);
        answers = vector<T>(m);
        node_freq = S = E = dep = vector<T>(n + 5);
        FT = vector<T>(2 * n + 5);
        anc = vector<vector<T>>(n + 5, vector<T>(LOG));
        adj = G, val = V;

        dfs(root);
    }

    void dfs(int u, int p = -1)
    {
        S[u] = timer;
        FT[timer++] = u;
        for (auto &v : adj[u])
        {
            if (v == p)
                continue;
            dep[v] = dep[u] + 1, anc[v][0] = u;
            for (int bit = 1; bit < LOG; bit++)
                anc[v][bit] = anc[anc[v][bit - 1]][bit - 1];
            dfs(v, u);
        }
        E[u] = timer;
        FT[timer++] = u;
    }

    int kth_ancestor(int u, int k)
    {
        if (dep[u] < k)
            return -1;
        for (int bit = LOG - 1; bit >= 0; bit--)
            if (k & (1 << bit))
                u = anc[u][bit];
        return u;
    }

    int get_lca(int u, int v)
    {
        if (dep[u] < dep[v])
            swap(u, v);
        u = kth_ancestor(u, dep[u] - dep[v]);
        if (u == v)
            return u;
        for (int bit = LOG - 1; bit >= 0; bit--)
            if (anc[u][bit] != anc[v][bit])
                u = anc[u][bit], v = anc[v][bit];
        return anc[u][0];
    }

    void set_block_id()
    {
        for (int i = 0; i < m; i++)
            queries[i].block_idx = queries[i].l / Sqrt_N;
    }

    void Get_Data()
    {
        for (int i = 0, u, v; i < m && cin >> u >> v; i++)
            queries[i] = query(S, E, u, v, i, get_lca(u, v));
        set_block_id();
        Process()
    }

    void add(int idx)
    {
        // fill this function with what problem needs
        int u = FT[idx];
        node_freq[u]++;
        if (node_freq[u] == 1)
        {
            // add u to the path (u is in the path)
            if (++cnt[val[u]] == 1)
                ans++;
        }
        else
        {
            // remove u from the path (u is not in the path anymore)
            if (--cnt[val[u]] == 0)
                ans--;
        }
    }

    void remove(int idx)
    {
        // fill this function with what problem needs
        int u = FT[idx];
        node_freq[u]--;
        if (node_freq[u] == 1)
        {
            // add u to the path (u is in the path)
            if (++cnt[val[u]] == 1)
                ans++;
        }
        else
        {
            // remove u from the path (u is not in the path anymore)
            if (--cnt[val[u]] == 0)
                ans--;
        }
    }

    void set_range(query &q)
    {
        while (curr_l > q.l)
            curr_l--, add(curr_l);
        while (curr_r < q.r)
            curr_r++, add(curr_r);
        while (curr_l < q.l)
            remove(curr_l), curr_l++;
        while (curr_r > q.r)
            remove(curr_r), curr_r--;
    }

    void Process()
    {
        sort(all(queries));

        // start with the first query
        curr_l = queries[0].l, curr_r = queries[0].l - 1;

        for (int i = 0; i < m; i++)
        {
            set_range(queries[i]);
            // if lca is -1 then the two nodes are in the same subtree
            if (queries[i].lca != -1)
                add(S[queries[i].lca]);

            answers[queries[i].query_idx] = ans;

            if (queries[i].lca != -1)
                remove(S[queries[i].lca]);
        }
    }

    void Print_queries_answers()
    {
        for (int i = 0; i < m; i++)
            cout << answers[i] << '\n';
    }

    vector<T> get_answers()
    {
        return answers;
    }
};

// cord compression

template <typename T = int>
struct cord_compression
{

    vector<T> a, b, c;
    int n;

    cord_compression(vector<T> &A)
    {
        a = A;
        n = sz(a);
        b = a;
        sort(all(b));
        b.resize(unique(all(b)) - b.begin());
        c.resize(n);
        for (int i = 0; i < n; i++)
            c[i] = lower_bound(all(b), a[i]) - b.begin();
    }

    vector<T> get_compressed()
    {
        return c;
    }

    vector<T> get_original()
    {
        return b;
    }
};

void Try()
{
    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);
    vector<int> val(n + 1);

    for (int i = 1; i <= n; i++)
        cin >> val[i];

    // cord_compression<int> cc(val);
    // val = cc.get_compressed();

    for (int i = 1, u, v; i < n && cin >> u >> v; i++)
        adj[u].push_back(v), adj[v].push_back(u);

    MO_Tree<int> mo(n, m, adj, val);

    mo.Get_Data();
    mo.Process();
    mo.Print_queries_answers();
}

signed main()
{
    int t = 1;
    // cin >> t;
    while (t--)
    {
        Try();
    }
}