#include <bits/stdc++.h>
using namespace std;

// #define int long long
#define ll long long
#define all(x) x.begin(), x.end()
void files()
{
    ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
}

template <typename T, bool x = 0>
struct Segment_Tree
{

    // constructors
    Segment_Tree(int n, T def = 0)
    {
        this->n = n;
        this->def = def;
        tree.resize(4 * n);
        lazy.resize(4 * n, -1);
    }
    Segment_Tree() = default;

    // attributes
    int n;
    T def;
    vector<T> tree;
    vector<T> arr;
    vector<T> lazy;

    // methods

    T process(T a, T b)
    {
        return a | b;
    }

    void propagate(int node, int start, int end)
    {
        if (lazy[node] == -1)
            return;
        tree[node] = lazy[node];
        if (start != end)
        {
            lazy[2 * node] = lazy[node];
            lazy[2 * node + 1] = lazy[node];
        }
        lazy[node] = -1;
    }

    void update(int l, int r, T val, int node, int start, int end)
    {
        propagate(node, start, end);
        if (l > end || r < start)
            return;
        if (l <= start && end <= r)
        {
            lazy[node] = val;
            propagate(node, start, end);
            return;
        }
        int mid = (start + end) / 2;
        update(l, r, val, 2 * node, start, mid);
        update(l, r, val, 2 * node + 1, mid + 1, end);
        tree[node] = process(tree[2 * node], tree[2 * node + 1]);
    }

    void update(int l, int r, T val)
    {
        update(l, r, val, 1, int(x), n - int(!x));
    }

    T query(int l, int r, int node, int start, int end)
    {
        propagate(node, start, end);
        if (l > end || r < start)
            return def;
        if (l <= start && end <= r)
            return tree[node];
        int mid = (start + end) / 2;
        return process(query(l, r, 2 * node, start, mid), query(l, r, 2 * node + 1, mid + 1, end));
    }

    T query(int l, int r)
    {
        return query(l, r, 1, int(x), n - int(!x));
    }
};

struct HLD
{
    const vector<vector<int>> &adj;
    vector<int> dep, par, cntchilds, child, root, pos;
    Segment_Tree<int, 1> seg;
    int nxt;
    bool edges;
    HLD(int n, const vector<vector<int>> &tree, int tree_root = 1, bool edges = 0) : adj(tree)
    {
        nxt = 1;
        dep.resize(n + 5);
        par.resize(n + 5);
        root.resize(n + 5);
        pos.resize(n + 5);
        cntchilds.resize(n + 5);
        child.resize(n + 5);
        seg = Segment_Tree<int, 1>(n + 5);
        this->edges = edges;
        init(tree_root, 0, 1);
        build(tree_root);
    }

    void init(int u, int p, int d)
    {
        dep[u] = d;
        par[u] = p;
        cntchilds[u] = 1;
        for (auto &v : adj[u])
        {
            if (v == p)
                continue;
            init(v, u, d + 1);
            cntchilds[u] += cntchilds[v];
            if (cntchilds[v] > cntchilds[child[u]])
                child[u] = v;
        }
    }

    void build(int u, bool new_chain = true)
    {
        root[u] = new_chain ? u : root[par[u]];
        pos[u] = nxt++;
        if (child[u])
            build(child[u], false);
        for (auto &v : adj[u])
        {
            if (v != par[u] && v != child[u])
                build(v, true);
        }
    }

    void update(int u, int val)
    {
        seg.update(pos[u], pos[u] + cntchilds[u] - 1, val);
    }

    inline pair<int, int> moveUp(int &u)
    {
        pair<int, int> ans = {pos[root[u]], pos[u]};
        u = par[root[u]];
        return ans;
    }

    inline void makeUlower(int &u, int &v)
    {
        if (dep[root[u]] < dep[root[v]] || (root[u] == root[v] && dep[u] < dep[v]))
            swap(u, v);
    }

    bool isUlower(int u, int v)
    {
        if (dep[root[u]] < dep[root[v]] || (root[u] == root[v] && dep[u] < dep[v]))
            return false;
        return true;
    }

    vector<pair<int, int>> Q_path(int u, int v)
    {
        vector<pair<int, int>> ans;
        while (root[u] != root[v])
        {
            makeUlower(u, v);
            ans.push_back(moveUp(u));
        }

        makeUlower(u, v);
        if (!edges)
            ans.push_back({pos[v], pos[u]});
        else if (u != v)
            ans.push_back({pos[v] + 1, pos[u]});

        return ans;
    }
};

void solve()
{
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    HLD hld(n, adj);
    int q;
    cin >> q;
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            int node;
            cin >> node;
            hld.update(node, 1);
        }
        else if (type == 2)
        {
            int node;
            cin >> node;
            vector<pair<int, int>> path = hld.Q_path(1, node);
            for (auto &p : path)
            {
                hld.seg.update(p.first, p.second, 0);
            }
        }
        else
        {
            int node;
            cin >> node;
            cout << hld.seg.query(hld.pos[node], hld.pos[node]) << '\n';
        }
    }
}

signed main()
{
    files();
    int t = 1;
    // cin >> t;
    while (t--)
    {
        solve();
    }
    return 0;
}