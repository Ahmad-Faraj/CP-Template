#include <bits/stdc++.h>

using namespace std;

#define fixed(n) fixed << setprecision(n)
#define ceil(n, m) (((n) + (m) - 1) / (m))
#define add_mod(a, b, m) (((a % m) + (b % m)) % m)
#define sub_mod(a, b, m) (((a % m) - (b % m) + m) % m)
#define mul_mod(a, b, m) (((a % m) * (b % m)) % m)
#define all(vec) vec.begin(), vec.end()
#define rall(vec) vec.rbegin(), vec.rend()
#define sz(x) int(x.size())
#define debug(x) cout << #x << ": " << (x) << "\n";
#define fi first
#define se second
#define ll long long
#define ull unsigned long long
#define EPS 1e-9
constexpr int INF = 1 << 30, Mod = 1e9 + 7;
constexpr ll LINF = 1LL << 62;
#define PI acos(-1)
template <typename T = int>
using Pair = pair<T, T>;
vector<string> RET = {"NO", "YES"};

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

struct Node
{

    int sum;
    int max_suff, max_pref, max_seg;
    int min_suff, min_pref, min_seg;

    Node(int V = 0)
    {
        sum = V;
        max_suff = max_pref = max_seg = max(V, 0);
        min_suff = min_pref = min_seg = min(V, 0);
    }

    Node operator=(const int rhs)
    {
        sum = rhs;
        max_suff = max_pref = max_seg = max(rhs, 0);
        min_suff = min_pref = min_seg = min(rhs, 0);
        return *this;
    }

    void rev()
    {
        swap(max_pref, max_suff);
        swap(min_pref, min_suff);
    }
};

Node combine(const Node &a, const Node &b)
{
    Node res;
    res.sum = a.sum + b.sum;

    res.max_pref = max(a.max_pref, a.sum + b.max_pref);
    res.max_suff = max(b.max_suff, b.sum + a.max_suff);
    res.max_seg = max({a.max_seg, b.max_seg, a.max_suff + b.max_pref});

    res.min_pref = min(a.min_pref, a.sum + b.min_pref);
    res.min_suff = min(b.min_suff, b.sum + a.min_suff);
    res.min_seg = min({a.min_seg, b.min_seg, a.min_suff + b.min_pref});

    return res;
}

template <typename T = int, int Base = 0>
struct Segment_Tree
{

    int size;
    Node DEFAULT;
    vector<Node> tree;

    Segment_Tree(int n = 0)
    {
        size = 1, DEFAULT = Node();
        while (size < n)
            size *= 2;
        tree = vector<Node>(2 * size, DEFAULT);
    }

    Segment_Tree(int n, const vector<T> &nums)
    {
        size = 1, DEFAULT = Node();
        while (size < n)
            size *= 2;
        tree = vector<Node>(2 * size, DEFAULT);
        build(nums);
    }

    // change the operation here
    Node operation(const Node &a, const Node &b)
    {
        return combine(a, b);
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

    void update(int index, T v, int idx, int lx, int rx)
    {
        if (rx == lx)
            tree[idx] = v;
        else
        {
            int mx = (rx + lx) / 2;
            if (index <= mx)
                update(index, v, 2 * idx, lx, mx);
            else
                update(index, v, 2 * idx + 1, mx + 1, rx);
            tree[idx] = operation(tree[2 * idx], tree[2 * idx + 1]);
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
        return operation(query(l, r, 2 * idx, lx, mx), query(l, r, 2 * idx + 1, mx + 1, rx));
    }

    Node query_Node(int l, int r)
    {
        return query(l, r, 1, 1, size);
    }

    Node query(int l, int r)
    {
        return query_Node(l, r);
    }

    Node get(const int idx)
    {
        return query(idx, idx);
    }
};

template <typename T = int, const int VAL_ON_EDGE = 0>
struct HLD
{
    // VAL_ON_EDGE = 0 if value on nodes
    // VAL_ON_EDGE = 1 if value on edges

    const vector<vector<T>> &adj;
    vector<T> dep, par, root, pos, SubtreeSz, child, tin;
    int nxtPos;
    Segment_Tree<T> seg;

    HLD(int n, const vector<vector<T>> &G, int treeRoot = 1) : adj(G)
    {
        nxtPos = 1;
        // child is heavy child
        dep = par = root = pos = SubtreeSz = child = vector<T>(n + 5);
        seg = Segment_Tree<T>(n + 5);
        init(treeRoot);
        build(treeRoot);
    }

    // init the tree
    void init(int u, int p = -1, int d = 0)
    {
        dep[u] = d, par[u] = p, SubtreeSz[u] = 1;
        for (auto v : adj[u])
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
        for (auto v : adj[u])
        {
            if (v == par[u] || v == child[u])
                continue;
            build(v, true);
        }
    }

    // make u lower
    bool isULower(int &u, int &v)
    {
        if (dep[root[u]] < dep[root[v]] || (root[u] == root[v] && dep[u] < dep[v]))
            return false;
        return true;
    }

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
        // move u and v to the same chain
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
        for (auto &[l, r] : queryPath(u, v))
            seg.update(l, r, val);
    }

    // query value
    Node query(int u, int v)
    {
        Node L, R;
        while (root[u] != root[v])
        {
            if (isULower(u, v))
                L = combine(seg.query(pos[root[u]], pos[u]), L), moveUp(u);
            else
                R = combine(seg.query(pos[root[v]], pos[v]), R), moveUp(v);
        }
        if (isULower(u, v))
            L = combine(seg.query(pos[v], pos[u]), L);
        else
            R = combine(seg.query(pos[u], pos[v]), R);

        L.rev();

        return combine(L, R);
    }
};

void Solve()
{
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 5);
    vector<vector<int>> query;
    vector<int> ans;
    for (int i = 1, id = 2; i <= n; i++)
    {
        char type;
        cin >> type;
        if (type == '+')
        {
            int v, x;
            cin >> v >> x;
            adj[v].push_back(id);
            query.push_back({id, x});
            id++;
        }
        else
        {
            int u, v, k;
            cin >> u >> v >> k;
            query.push_back({u, v, k});
        }
    }
    HLD<int> hld(n, adj);
    hld.update(1, 1);
    for (auto &q : query)
    {
        if (q.size() == 2)
        {
            int v = q[0], x = q[1];
            hld.update(v, x);
        }
        else
        {
            int u = q[0], v = q[1], k = q[2];
            Node res = hld.query(u, v);
            if (k >= 0)
                ans.push_back(res.max_seg >= k);
            else
                ans.push_back(res.min_seg <= k);
        }
    }
    for (auto &x : ans)
        cout << RET[x] << '\n';
}

int main()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int test_cases = 1;
    cin >> test_cases;
    for (int tc = 1; tc <= test_cases; tc++)
    {
        // cout << "Case #" << tc << ": ";
        Solve();
    }
    return 0;
}