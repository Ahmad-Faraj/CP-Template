#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) int(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;
ll max(ll n1, ll n2) { return (n1 >= n2) ? n1 : n2; }
ll min(ll n1, ll n2) { return (n1 >= n2) ? n2 : n1; }
void fastio()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
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
        return a.val + b.val;
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
 
    Node query_Node(const int l, const int r)
    {
        return query(l, r, 1, 1, size);
    }
 
    T query(const int l, const int r)
    {
        return query_Node(l, r).val;
    }
 
    T get(const int idx)
    {
        return query_Node(idx, idx).val;
    }
 
    friend ostream &operator<<(ostream &out, const Node &node)
    {
        out << node.val << ' ';
        return out;
    }
};
vector<vector<int>> adj;
vector<int> subtree_size;
vector<int> euler_tour;
void dfs(int u, int p)
{
    subtree_size[u] = 1;
    euler_tour.push_back(u);
    for (auto v : adj[u])
    {
        if (v != p)
        {
            dfs(v, u);
            subtree_size[u] += subtree_size[v];
            //euler_tour.push_back(u);
        }
    }
}
void solve(int tc)
{
    int n, q;
    cin >> n >> q;
    adj.resize(n + 1);
    subtree_size.resize(n + 1);
    vector<int> nodes_val(n + 1);
    for (int i = 1; i <= n; i++)
        cin >> nodes_val[i];
    for (int i = 2; i <= n; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1, 0);
    // for (auto &i : euler_tour)
    //     cout << i << sp;
    // cout << nl;
    // for (int i = 1; i <= n; i++)
    //     cout << subtree_size[i] << sp;
    // cout << nl;
    vector<int> idxs(n + 1);
    for (int i = 0; i < sz(euler_tour); i++)
        idxs[euler_tour[i]] = i + 1;
    vector<ll> euler_tour_val(sz(euler_tour));
    for (int i = 0; i < sz(euler_tour); i++)
        euler_tour_val[i] = nodes_val[euler_tour[i]];
    Segment_Tree<ll> seg(sz(euler_tour), euler_tour_val);
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            int s, x;
            cin >> s >> x;
            seg.update(idxs[s], x);
        }
        else
        {
            int s;
            cin >> s;
            cout << seg.query(idxs[s], idxs[s] + subtree_size[s] - 1) << nl;
        }
    }
}
int main(void)
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
