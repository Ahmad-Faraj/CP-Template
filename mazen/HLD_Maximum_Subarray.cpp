#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
#define int long long
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) (int)(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;
void fastio()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
}
struct Node
{

    int sum, pref, suff, mx_sub, lazy;

    Node(int V = 0) : sum(V), pref(V), suff(V), mx_sub(V), lazy(-100000) {}

    Node operator=(const int rhs)
    {
        sum = rhs;
        pref = suff = mx_sub = max(0LL, rhs);
        lazy = -100000;
        return *this;
    }
};
template <typename T = int, int Base = 0>
struct Segment_Tree
{

    int size;
    Node DEFAULT;
    vector<Node> tree;

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

    // Main operation to do

    Node operation(const Node &a, const Node &b)
    {
        Node res;
        res.sum = a.sum + b.sum;
        res.pref = max(a.pref, a.sum + b.pref);
        res.suff = max(b.suff, b.sum + a.suff);
        res.mx_sub = max({a.mx_sub, b.mx_sub, a.suff + b.pref});
        return res;
    }

    void push(int idx, int lx, int rx)
    {
        if (tree[idx].lazy == -100000)
            return;
        tree[idx].sum = tree[idx].lazy * (rx - lx + 1);
        tree[idx].pref = max(tree[idx].sum, 0LL);
        tree[idx].suff = max(tree[idx].sum, 0LL);
        tree[idx].mx_sub = max(tree[idx].sum, 0LL);
        if (lx != rx)
        {
            tree[2 * idx].lazy = tree[idx].lazy;
            tree[2 * idx + 1].lazy = tree[idx].lazy;
        }
        tree[idx].lazy = -100000;
    }

    // If Base is 1 so the array is 1-based else the array is 0-based

    void build(const vector<T> &nums, int idx, int lx, int rx)
    {
        if (Base ? lx >= sz(nums) : lx > sz(nums))
            return;
        if (rx == lx)
            tree[idx] = nums[lx - !Base];
        else
        {
            int mx = (rx + lx) / 2;
            build(nums, 2 * idx, lx, mx);
            build(nums, 2 * idx + 1, mx + 1, rx);
            tree[idx] = operation(tree[2 * idx], tree[2 * idx + 1]);
        }
    }

    void build(const vector<T> &nums)
    {
        build(nums, 1, 1, size);
    }

    void update(int l, int r, T v, int idx, int lx, int rx)
    {
        push(idx, lx, rx);
        if (lx > r || l > rx)
            return;
        if (lx >= l && rx <= r)
        {
            tree[idx].lazy = v;
            push(idx, lx, rx);
        }
        else
        {
            int mx = (rx + lx) / 2;
            update(l, r, v, 2 * idx, lx, mx);
            update(l, r, v, 2 * idx + 1, mx + 1, rx);
            tree[idx] = operation(tree[2 * idx], tree[2 * idx + 1]);
        }
    }

    void update(const int l, const int r, const T v)
    {
        update(l, r, v, 1, 1, size);
    }

    void update(int index, T v, int idx, int lx, int rx){
        if(rx == lx) tree[idx] = v;
        else {  
            int mx = (rx + lx) / 2;
            if(index <= mx) update(index, v, 2 * idx, lx, mx);
            else update(index, v, 2 * idx + 1, mx + 1, rx);
            tree[idx] = operation(tree[2 * idx], tree[2 * idx + 1]);
        }
    }

    void update(const int index, const T v){
        update(index, v, 1, 1, size);
    }






    Node query(int l, int r, int idx, int lx, int rx)
    {
        push(idx, lx, rx);
        if (lx > r || l > rx)
            return DEFAULT;
        if (lx >= l && rx <= r)
            return tree[idx];
        int mx = (lx + rx) / 2;
        return operation(query(l, r, 2 * idx, lx, mx), query(l, r, 2 * idx + 1, mx + 1, rx));
    }

    Node query(const int l, const int r)
    {
        return query(l, r, 1, 1, size);
    }
};
void rev(Node& a){
    swap(a.pref, a.suff);
}
template <typename T = int, const int VAL_ON_EDGE = 0>
struct HLD
{
    // VAL_ON_EDGE = 0 if value on nodes
    // VAL_ON_EDGE = 1 if value on edges

    const vector<vector<pair<T, T>>> &adj;
    vector<T> dep, par, root, pos, SubtreeSz, child;
    int nxtPos;
    Segment_Tree<T> seg;

    HLD(int n, const vector<vector<pair<T, T>>> &G, int treeRoot = 1) : adj(G)
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
    bool isUlower(int &u, int &v){
        if(dep[root[u]] < dep[root[v]] || (root[u] == root[v] && dep[u] < dep[v]))
            return false;
        return true;
    }
    // make u lower
    void makeULower(int &u, int &v)
    {
        if (dep[root[u]] < dep[root[v]] || (root[u] == root[v] && dep[u] < dep[v]))
            swap(u, v);
    }

    // move up the chain and also change the next position
    pair<T, T> moveUp(int &u)
    {
        pair<T, T> ret = {pos[root[u]], pos[u]};
        u = par[root[u]];
        return ret;
    }

    vector<pair<T, T>> queryPath(int u, int v)
    {
        // return all ranges in segment tree
        vector<pair<T, T>> ret;
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

        // sort(ret.begin(), ret.end());
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
    void update(int u_q, int v_q, T val)
    {
        for (auto &[u, v] : queryPath(u_q, v_q))
        {
            seg.update(u, v, val);
        }
    }
    // update value of edge u-v
    // void update(int u, int v, T val)
    // {
    //     u = getChild(u, v);
    //     seg.update(pos[u], val);
    // }

    // query value
    // int query(int u_q, int v_q)
    // {
    //     Node res = 0;
    //     for (auto &[u, v] : queryPath(u_q, v_q))
    //     {
    //         Node ret = seg.query_Node(u, v);
    //         cout << u << sp << v << sp << ret.sum << nl;
    //         res = seg.operation(res, ret);
    //     }
    //     return res.mx_sub;
    // }
    int query(int u, int v){
        Node L, R;
        while(root[u] != root[v]){
            if(isUlower(u, v))
                L = seg.operation(seg.query(pos[root[u]], pos[u]), L), moveUp(u);
            else
                R = seg.operation(seg.query(pos[root[v]], pos[v]), R), moveUp(v);
        }
        if(isUlower(u, v))
            L = seg.operation(seg.query(pos[v], pos[u]), L);
        else
            R = seg.operation(seg.query(pos[u], pos[v]), R);
        rev(L);
        return seg.operation(L, R).mx_sub;
    }
};
void solve(int tc)
{
    int n;
    cin >> n;
    vector<vector<pair<int, int>>> adj(n + 1);
    vector<int> val(n + 1);
    for (int i = 1; i <= n; i++)
        cin >> val[i];
    for (int i = 1; i < n; ++i)
    {
        int u, v, w;
        cin >> u >> v;
        adj[u].push_back({v, 0});
        adj[v].push_back({u, 0});
    }

    HLD<int> hld(n, adj);
    for (int i = 1; i <= n; i++)
    {
        hld.update(i, val[i]);
    }
    int q;
    cin >> q;
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            int u, v;
            cin >> u >> v;
            cout << hld.query(u, v) << "\n";
        }
        else
        {
            int u, v, x;
            cin >> u >> v >> x;
            hld.update(u, v, x);
        }
    }
}
signed main(void)
{
    fastio();
    int tc = 1;
    // cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}