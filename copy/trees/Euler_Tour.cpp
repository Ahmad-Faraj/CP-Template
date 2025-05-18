#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

#define int long long

#define debug(x) cout << #x << ": " << (x) << "\n";
#define ull unsigned long long int
#define ld long double
#define ll long long
#define PI acos(-1)
#define endl "\n"
#define sz(x) (int)x.size()
#define all(x) x.begin(), x.end()
#define fixed(n) cout << fixed << setprecision(n)
#define loop(i, n) for (int i = 0; i < (int)n; i++)
#define ordered_set tree<ll, null_type, less_equal<>, rb_tree_tag, tree_order_statistics_node_update>

const int N = 500005;
const double EPS = 1e-9;
const int MOD = 1e9 + 7;
const int OO = 0x3f3f3f3f;
const ll INF = LLONG_MAX;
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
        return a.val + b.val;
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

vector<vector<int>> adj;
int timer = 0;
int start[N];
int endd[N];

void euler_tour(int src, int par)
{
    start[src] = ++timer;
    for (int child : adj[src])
    {
        if (child != par)
        {
            euler_tour(child, src);
        }
    }
    endd[src] = timer; // Fix this to properly end the subtree range.
}

void solve()
{
    int n, q;
    cin >> n >> q;
    adj.resize(n + 1); // Fix: Initialize adjacency list for the nodes
    vector<int> v(n);
    cin >> v;

    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }

    euler_tour(1, -1); // Generate the Euler Tour

    vector<int> euler_values(n + 1);
    for (int i = 1; i <= n; i++)
    {
        euler_values[start[i]] = v[i - 1]; // Reassign the values based on the Euler tour
    }

    Segment_Tree<int, 1> tree(n, euler_values); // Build the Segment Tree with Euler values

    while (q--)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            int s, x;
            cin >> s >> x;
            tree.update(start[s], x); // Update the tree
        }
        else
        {
            int s;
            cin >> s;
            cout << tree.query(start[s], endd[s]) << endl; // Query the subtree range
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