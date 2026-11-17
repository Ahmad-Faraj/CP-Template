template <typename T = int>
struct LCA
{

    int N, LOG;
    vector<vector<T>> anc;
    vector<vector<T>> adj;
    vector<int> dep;

    LCA(int n = 0)
    {
        N = n + 10, LOG = 0;
        while ((1 << LOG) <= N)
            LOG++;
        dep = vector<int>(N);
        adj = vector<vector<T>>(N);
        anc = vector<vector<T>>(N, vector<T>(LOG));
    }

    LCA(int n, const vector<vector<T>> &G) : adj(G)
    {
        N = n + 10, LOG = 0;
        while ((1 << LOG) <= N)
            LOG++;
        dep = vector<int>(N);
        anc = vector<vector<T>>(N, vector<T>(LOG));
    }

    void add_edge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void build_adj(int edges)
    {
        for (int i = 0, u, v; i < edges && cin >> u >> v; i++)
            add_edge(u, v);
    }

    void dfs(int u, int p = 0)
    {
        for (auto &v : adj[u])
        {
            if (v == p)
                continue;
            dep[v] = dep[u] + 1, anc[v][0] = u;
            for (int bit = 1; bit < LOG; bit++)
                anc[v][bit] = anc[anc[v][bit - 1]][bit - 1];
            dfs(v, u);
        }
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

    T query(int u, int v)
    {
        int lca = get_lca(u, v);
        return dep[u] + dep[v] - 2 * dep[lca];
    }

    void build(int root = 1)
    {
        dfs(root);
    }
};