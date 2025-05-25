#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
#define nl "\n"
#define oo (ll)1e9 + 1
#define OO (ll)1e18 + 1
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

struct SegTree{

    struct Node{
        ll val;
        Node(ll _val = 0){
            val = _val;
        }
        Node operator = (ll _val){
            val = _val;
            return *this;
        }
    };
    int size;
    vector<Node> tree;
    Node Default;
    void init(int n){
        size = 1;
        while(size < n)
            size *= 2;
        Default = 0;
        tree = vector<Node>(2 * size + 1, Default);
    }
    Node merge(Node& a, Node& b){
        Node res = a.val + b.val;
        return res;
    }
    void build(vector<int>&nums, int idx, int lx, int rx){
        if(lx == rx){
            if(lx <= nums.size()){
                tree[idx] = nums[lx - 1];
            }
            return;
        }
        int mx = (lx + rx) / 2;
        build(nums, 2 * idx, lx, mx);
        build(nums, 2 * idx + 1, mx + 1, rx);
        tree[idx] = merge(tree[2 * idx], tree[2 * idx + 1]);
    }
    void build(vector<int>& nums){
        build(nums, 1, 1, size);
    }
    void update(int i, int v, int idx, int lx, int rx){
        if(lx == rx){
            tree[idx] = v;
            return;
        }
        int mx = (lx + rx) / 2;
        if(i <= mx)
            update(i, v, 2 * idx, lx, mx);
        else
            update(i, v, 2 * idx + 1, mx + 1, rx);
        tree[idx] = merge(tree[2 * idx], tree[2 * idx + 1]);
    }
    void update(int i, int v){
        update(i, v, 1, 1, size);
    }
    Node query(int l, int r, int idx, int lx, int rx){
        if(lx > r || rx < l)
            return Default;
        if(lx >= l && rx <= r)
            return tree[idx];
        int mx = (lx + rx) / 2;
        auto left = query(l, r, 2 * idx, lx, mx);
        auto right = query(l, r, 2 * idx + 1, mx + 1, rx);
        return merge(left, right);
    } 
    ll query(int l, int r){
        return query(l, r, 1, 1, size).val;
    }
};
void solve(int tc)
{
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for(auto  &i : a)
        cin >> i;
    SegTree st;
    st.init(n);
    st.build(a);

    while (q--)
    {
        int type, x, y;
        cin >> type >> x >> y;
        x++;
        if(type == 1)
            st.update(x, y);
        else
            cout << st.query(x, y) << "\n";
    }
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    //cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}