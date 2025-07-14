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
vector<int> parent;
vector<vector<int>> adj;
vector<int> depth;
vector<vector<int>> up;
int n; // number of nodes
ll LOG = 20;
// O(nlogn)
// precompute up[i][j] = 2^j th ancestor of i
// case if parent[i] < i based 1
void preprocessing_special_case(){

    for (int i = 1; i <= n; i++)
    {
        // if started from 0 then parent[0] = 0 and there will be a condition to check if i != 0
        // if i != 0 
        depth[i] = depth[parent[i]] + 1;

        up[i][0] = parent[i];
        for(int j = 1; j < LOG; j++){
            up[i][j] = up[up[i][j - 1]][j - 1];
        }
    }
}
// case if parent[i] > i or parent[i] < i  general case
void preprocessing_general_case(){

    // i don't know how to calculate the depth here so i will calculate it in the dfs function
    for(int i = 1; i <= n; i++){
        up[i][0] = parent[i];
    }
    for(int j = 1; j < LOG; j++){
        for(int i = 1; i <= n; i++){
            up[i][j] = up[up[i][j - 1]][j - 1];
        }
    }
}
// O(logn)
int k_th_ancestor(int node, int k){
    if(depth[node] < k) return -1;
    for(int i = 0; i < 20; i++){
        if(k & (1 << i)){
            node = up[node][i];
        }
    }
    return node;
}

// get parent and depth of each node using dfs
void dfs(int node){
    for(int child : adj[node]){
        if(child == parent[node]) continue;
        parent[child] = node;
        depth[child] = depth[node] + 1;
        dfs(child);
    }
}


// get lca 
ll lca(int a, int b)
{
    if (depth[a] < depth[b])
        swap(a, b);
    int k = abs(depth[a] - depth[b]);
    for (int i = LOG-1; i >= 0; i--)
    {
        if (k & (1 << i))
        {
            a = up[a][i];
        }
    }
    if (a == b)
        return a;
    for (int j = LOG-1; j >= 0; j--)
    {
        if(up[a][j]!=up[b][j]){
            a = up[a][j];
            b = up[b][j];
        }
    }
    return up[a][0];
}
//
void solve(int tc)
{
    cin >> n;
    LOG = ceil(log2(n)) + 1;
    adj.resize(n + 1);
    parent.resize(n + 1);
    up.assign(n + 1, vector<int>(LOG));
    depth.resize(n + 1);
    int m;
    cin >> m;
    for(int i = 1; i <= m; i++){
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    // calc parent and depth of each node
    int root = 1; // suppose the root is 1
    parent[root] = root;
    depth[root] = 0;
    dfs(root);
    // precompute up[i][j] = 2^j th ancestor of i
    preprocessing_general_case();
    int q;
    cin >> q;
    while(q--){
        int u, v;
        cin >> u >> v;
        cout << lca(u, v) << nl;
    }
}
int main(void)
{
    fastio();
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