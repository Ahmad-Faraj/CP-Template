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

struct Lazy_Segment_Tree
{
    struct Node
    {
        ll val, lazy;
        Node(ll _val = 0, ll _lazy = 0) : val(_val), lazy(_lazy) {}
        Node operator=(ll _val)
        {
            val = _val;
            lazy = 0;
            return *this;
        }
    };
    int size;
    vector<Node> tree;
    Node Default;
    void init(int n)
    {
        size = 1;
        while (size < n)
            size *= 2;
        Default = 0;
        tree = vector<Node>(2 * size);
    }
    Node merge(Node &a, Node &b)
    {
        return a.val + b.val;
    }
    void build(vector<int> &nums, int idx, int lx, int rx)
    {
        if (lx == rx)
        {
            if (lx <= nums.size())
                tree[idx] = nums[lx - 1];
            return;
        }
        int mx = (lx + rx) / 2;
        build(nums, 2 * idx, lx, mx);
        build(nums, 2 * idx + 1, mx + 1, rx);
        tree[idx] = merge(tree[2 * idx], tree[2 * idx + 1]);
    }
    void build(vector<int> &nums)
    {
        build(nums, 1, 1, size);
    }
    void push(int idx, int lx, int rx)
    {
        if (tree[idx].lazy == 0)
            return;
        tree[idx].val += (rx - lx + 1) * tree[idx].lazy;
        if (lx != rx)
        {
            tree[2 * idx].lazy += tree[idx].lazy;
            tree[2 * idx + 1].lazy += tree[idx].lazy;
        }
        tree[idx].lazy = 0;
    }
    void update(int l, int r, int v, int idx, int lx, int rx)
    {
        push(idx, lx, rx);
        if (rx < l || lx > r)
            return;
        if (lx >= l && rx <= r)
        {
            tree[idx].lazy += v;
            push(idx, lx, rx);
            return;
        }
        int mx = (lx + rx) / 2;
        update(l, r, v, 2 * idx, lx, mx);
        update(l, r, v, 2 * idx + 1, mx + 1, rx);
        tree[idx] = merge(tree[2 * idx], tree[2 * idx + 1]);
    }
    void update(int l, int r, int v)
    {
        update(l, r, v, 1, 1, size);
    }
    Node query(int l, int r, int idx, int lx, int rx)
    {
        push(idx, lx, rx);
        if (rx < l || lx > r)
            return Default;
        if (lx >= l && rx <= r)
            return tree[idx];
        int mx = (lx + rx) / 2;
        auto left = query(l, r, 2 * idx, lx, mx);
        auto right = query(l, r, 2 * idx + 1, mx + 1, rx);
        return merge(left, right);
    }
    ll query(int l, int r)
    {
        return query(l, r, 1, 1, size).val;
    }
};
void solve(int tc)
{
}
int main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
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