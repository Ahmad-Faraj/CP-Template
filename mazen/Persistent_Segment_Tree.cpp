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
// 1-based indexing
// 1-based version indexing
template <typename T = int>
struct PST
{

    struct Node
    {

        T val;
        Node *left, *right;

        Node(T _val = 0)
        {
            this->val = _val;
            left = right = this;
        }

        Node(Node *node, Node *l = new Node, Node *r = new Node)
        {
            val = node->val;
            left = l;
            right = r;
        }
    };

    vector<Node *> roots = {new Node()};
    int N;
    T Lx, Rx;

    PST(int n = 0, T lx = -1e9, T rx = 1e9) : N(n), Lx(lx), Rx(rx) {}

    // use build if the nums represents the frequency of the elements
    // and the elements are in the range [1, n]
    Node *build(const vector<T> &nums, T l, T r)
    {
        if (l == r)
            return new Node(nums[l - 1]);
        T mx = l + (r - l) / 2;
        Node *L = build(nums, l, mx);
        Node *R = build(nums, mx + 1, r);
        return new Node(merge(L, R), L, R);
    }

    void build(const vector<T> &nums)
    {
        roots.push_back(build(nums, Lx, Rx));
    }

    Node *merge(Node *a, Node *b)
    {
        Node *node = new Node();
        node->val = a->val + b->val;
        return node;
    }

    Node *update(Node *root, int idx, T val, T lx, T rx)
    {
        if (idx < lx || idx > rx)
            return root;
        if (lx == rx)
            return new Node(val);
        T mx = lx + (rx - lx) / 2;
        Node *L = update(root->left, idx, val, lx, mx);
        Node *R = update(root->right, idx, val, mx + 1, rx);
        return new Node(merge(L, R), L, R);
    }

    void update(int idx, T val)
    {
        roots.push_back(update(roots.back(), idx, val, Lx, Rx));
    }

    void update_version(int idx, T val, int version)
    {
        roots[version] = (update(roots[version], idx, val, Lx, Rx));
    }
    Node *query(Node *root, int l, int r, T lx, T rx)
    {
        if (root == nullptr)
            return new Node(); // Base case for null pointer
        if (lx > r || l > rx)
            return new Node(); // Base case for out-of-range interval
        if (lx >= l && rx <= r)
            return root;
        int mx = lx + (rx - lx) / 2;
        Node *L = query(root->left, l, r, lx, mx);
        Node *R = query(root->right, l, r, mx + 1, rx);
        return merge(L, R);
    }

    T query(int l, int r, int version)
    {
        return query(roots[version], l, r, Lx, Rx)->val;
    }

    void copy(int version)
    {
        roots.push_back(roots[version]);
    }

    T get(int time, int idx)
    {
        return query(idx, idx, time)->val;
    }
};

void solve(int tc)
{
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];
    PST<int> pst(n, 1, n);
    pst.build(a);
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            int idx, val;
            cin >> idx >> val;
            pst.update(idx, val);
        }
        else if (type == 2)
        {
            int k, l, r;
            cin >> k >> l >> r;
            cout << pst.query(l, r, k) << nl;
        }
    }
}
signed main(void)
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