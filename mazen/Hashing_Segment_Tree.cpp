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
// hashing
const int N = 2e5 + 5;
const int Base = 31, Mod = 1e9 + 9;
int pw[N], inv[N];
void init(){
    pw[0] = 1;
    for(int i = 1; i < N; i++)
        pw[i] = (pw[i - 1] * Base) % Mod;
    inv[N - 1] = 1;
    int invBase = 1;
    for(int i = N - 2; i >= 0; i--){
        inv[i] = (invBase * pw[i + 1]) % Mod;
        invBase = (invBase * Base) % Mod;
    }
}
// hashing segment tree
struct node{
    int hsh, len;
    node(){
        hsh = len = 0;
    }
    node(int hsh, int len) : hsh(hsh), len(len) {}
};
node merge(node a, node b){
    return node((a.hsh + b.hsh * pw[a.len]) % Mod, a.len + b.len);
}
struct HashingSegmentTree{
    vector<node> tree;
    int n;
    HashingSegmentTree(int n) : n(n){
        tree.resize(4 * n);
    }
    void build(string &s, int id, int l, int r){
        if(l == r){
            tree[id] = node(s[l] - 'a' + 1, 1);
            return;
        }
        int mid = (l + r) >> 1;
        build(s, id << 1, l, mid);
        build(s, id << 1 | 1, mid + 1, r);
        tree[id] = merge(tree[id << 1], tree[id << 1 | 1]);
    }
    void build(string &s){
        build(s, 1, 0, n - 1);
    }
    void update(int pos, char val, int id, int l, int r){
        if(l == r){
            tree[id] = node(val - 'a' + 1, 1);
            return;
        }
        int mid = (l + r) >> 1;
        if(pos <= mid)
            update(pos, val, id << 1, l, mid);
        else
            update(pos, val, id << 1 | 1, mid + 1, r);
        tree[id] = merge(tree[id << 1], tree[id << 1 | 1]);
    }
    void update(int pos, char val){
        update(pos, val, 1, 0, n - 1);
    }
    node query(int x, int y, int id, int l, int r){
        if(x > r || y < l)
            return node();
        if(x <= l && r <= y)
            return tree[id];
        int mid = (l + r) >> 1;
        return merge(query(x, y, id << 1, l, mid), query(x, y, id << 1 | 1, mid + 1, r));
    }
    node query(int x, int y){ // 0-based index
        return query(x, y, 1, 0, n - 1);
    }
};
void solve(int tc)
{
    
}
signed main(void)
{
    fastio();
    init();
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