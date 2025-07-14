#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

#define int long long

#define ull unsigned long long int
#define ld long double
#define ll long long
#define PI acos(-1)
#define endl "\n"
#define sz(x) (int)x.size()
#define all(x) x.begin(), x.end()
#define clr(arr, val) memset(dp, -1, sizeof(dp))
#define fixed(n) cout << fixed << setprecision(n)
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ordered_set tree<ll, null_type, less_equal<>, rb_tree_tag, tree_order_statistics_node_update>

const int N = 1e6;
const double EPS = 1e-6;
const int MOD = 1000000007;
const int OO = 0x3f3f3f3f;
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

void Ahmed_Faraj()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}

template <typename T = int>
istream &operator>>(istream &in, vector<T> &v)
{
    for (auto &x : v)
        in >> x;
    return in;
}

template <typename T = int>
ostream &operator<<(ostream &out, const vector<T> &v)
{
    for (const T &x : v)
        out << x << ' ';
    return out;
}

template <typename T = int, int Base = 0>
struct Segment_Tree
{

    struct Node
    {
        T val;
        Node(T V = 0) : val(V) {}
        Node operator=(const T rhs)
        {
            val = rhs;
            return *this;
        }
    };

    int size;
    Node DEFAULT;
    vector<Node> tree;
#define LEFT (idx << 1)
#define RIGHT ((idx << 1) | 1)
#define VAL val

    Segment_Tree(int n = 0)
    {
        size = 1, DEFAULT = 0;
        while (size < n)
            size *= 2;
        tree = vector<Node>(2 * size, DEFAULT);
    }

    Segment_Tree(int n, const vector<T> &nums)
    {
        size = 1, DEFAULT = 0;
        while (size < n)
            size *= 2;
        tree = vector<Node>(2 * size, DEFAULT);
        build(nums);
    }

    Node operation(const Node &a, const Node &b)
    {
        return (a.val + b.val);
    }

    void build(const vector<T> &nums, int idx, int lx, int rx)
    {
        if (Base ? lx >= sz(nums) : lx > sz(nums))
            return;
        if (rx == lx)
            tree[idx] = nums[lx - !Base];
        else
        {
            int mx = (rx + lx) / 2;
            build(nums, LEFT, lx, mx);
            build(nums, RIGHT, mx + 1, rx);
            tree[idx] = operation(tree[LEFT], tree[RIGHT]);
        }
    }

    void build(const vector<T> &nums)
    {
        build(nums, 1, 1, size);
    }

    void update(int index, T v, int idx, int lx, int rx)
    {
        if (rx == lx)
            tree[idx] = v;
        else
        {
            int mx = (rx + lx) / 2;
            if (index <= mx)
                update(index, v, LEFT, lx, mx);
            else
                update(index, v, RIGHT, mx + 1, rx);
            tree[idx] = operation(tree[LEFT], tree[RIGHT]);
        }
    }

    void update(const int index, const T v)
    {
        update(index, v, 1, 1, size);
    }

    Node query(int l, int r, int idx, int lx, int rx)
    {
        if (lx > r || l > rx)
            return DEFAULT;
        if (lx >= l && rx <= r)
            return tree[idx];
        int mx = (lx + rx) / 2;
        return operation(query(l, r, LEFT, lx, mx), query(l, r, RIGHT, mx + 1, rx));
    }

    Node query_Node(const int l, const int r)
    {
        return query(l, r, 1, 1, size);
    }

    T query(const int l, const int r)
    {
        return query_Node(l, r).VAL;
    }

    T query(T num)
    {
        return query(num + 1, size);
    }

    T get(const int idx)
    {
        return query_Node(idx, idx).VAL;
    }

    friend ostream &operator<<(ostream &out, const Node &node)
    {
        out << node.VAL << ' ';
        return out;
    }

    void print(int idx, int lx, int rx)
    {
        if (lx == rx)
            cout << tree[idx] << ' ';
        else
        {
            int mx = (lx + rx) / 2;
            print(LEFT, lx, mx);
            print(RIGHT, mx + 1, rx);
        }
    }

    void print()
    {
        print(1, 1, size);
        cout << '\n';
    }

#undef LEFT
#undef RIGHT
#undef VAL
};

template <typename T = int>
using Pair = pair<T, T>;

template <typename T = int, const int VAL_ON_EDGE = 0>
struct HLD
{
    // VAL_ON_EDGE = 0 if value on nodes
    // VAL_ON_EDGE = 1 if value on edges

    const vector<vector<Pair<T>>> &adj;
    vector<T> dep, par, root, pos, SubtreeSz, child;
    int nxtPos;
    Segment_Tree<T> seg;

    HLD(int n, const vector<vector<Pair<T>>> &G, int treeRoot = 1) : adj(G)
    {
        nxtPos = 1;
        // child is heavy child
        dep = par = root = pos = SubtreeSz = child = vector<T>(n + 5);
        init(treeRoot);
        build(treeRoot);
        seg = Segment_Tree<T>(n + 5);
    }

    // init the tree
    void init(int u, int p = -1, int d = 0)
    {
        dep[u] = d, par[u] = p, SubtreeSz[u] = 1;
        for (auto [v, _] : adj[u])
        {
            if (v == p)
                continue;
            init(v, u, d + 1);
            SubtreeSz[u] += SubtreeSz[v];
            if (SubtreeSz[v] > SubtreeSz[child[u]])
                child[u] = v;
        }
    }

    // get the root of the chain
    int get_lca(int u, int v)
    {
        while (root[u] != root[v])
        {
            if (dep[root[u]] < dep[root[v]])
                swap(u, v);
            u = par[root[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }

    // build the chains
    void build(int u, bool newChain = true)
    {
        root[u] = newChain ? u : root[par[u]];
        pos[u] = nxtPos++;
        if (child[u])
            build(child[u], false);
        for (auto [v, _] : adj[u])
        {
            if (v == par[u] || v == child[u])
                continue;
            build(v, true);
        }
    }

    // make u lower
    void makeULower(int &u, int &v)
    {
        if (dep[root[u]] < dep[root[v]] || (root[u] == root[v] && dep[u] < dep[v]))
            swap(u, v);
    }

    // move up the chain and also change the next position
    Pair<T> moveUp(int &u)
    {
        Pair<T> ret = {pos[root[u]], pos[u]};
        u = par[root[u]];
        return ret;
    }

    vector<Pair<T>> queryPath(int u, int v)
    {
        // return all ranges in segment tree
        vector<Pair<T>> ret;
        while (root[u] != root[v])
        {
            makeULower(u, v);
            ret.push_back(moveUp(u));
        }
        // add range between u and v
        makeULower(u, v);
        if (!VAL_ON_EDGE) // value on nodes
            ret.push_back({pos[v], pos[u]});
        else if (u != v) // don't include the root node
            ret.push_back({pos[v] + 1, pos[u]});
        return ret;
    }

    // get child of a node for problem that value on edges
    int getChild(int u, int v)
    {
        if (par[u] == v)
            return u;
        return v;
    }

    // update value of node u
    void update(int u, T val)
    {
        seg.update(pos[u], val);
    }

    // update value of edge u-v
    void update(int u, int v, T val)
    {
        u = getChild(u, v);
        seg.update(pos[u], val);
    }

    // query value
    T query(int u_q, int v_q)
    {
        T ret = 0;
        for (auto &[u, v] : queryPath(u_q, v_q))
            ret += seg.query(u, v);
        return ret;
    }
};

vector<vector<pair<int, int>>> adj;
vector<pair<pair<int, int>, int>> edges;

void solve()
{
    int n;
    cin >> n;
    adj.resize(n + 1);
    for (int i = 0; i < n - 1; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].emplace_back(make_pair(v, w));
        adj[v].emplace_back(make_pair(u, w));
        edges.emplace_back(make_pair(make_pair(u, v), w));
    }
    HLD<int, 1> hld(n, adj);
    for (int i = 0; i < n - 1; i++)
    {
        hld.update(edges[i].first.first, edges[i].first.second, edges[i].second);
    }
    int q;
    cin >> q;
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            int u, val;
            cin >> u >> val;
            hld.update(edges[u - 1].first.first, edges[u - 1].first.second, val);
        }
        else
        {
            int a, b;
            cin >> a >> b;
            cout << hld.query(a, b) << endl;
        }
    }
}

signed main()
{
    Ahmed_Faraj();
    int test = 1;
    // cin >> test;
    for (int tc = 1; tc <= test; tc++)
    {
        // cout << "Case " << tc << ":";
        solve();
    }
    return 0;
}
