#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
 
using namespace std;
using namespace __gnu_pbds;
 
#define ll long long 
#define PI acos(-1)
#define all(x) x.begin(),x.end()
#define clr(v, d) memset(v, d, sizeof(v))
#define endl "\n"
#define sz(x) (int)x.size()
#define vl vector<ll>
#define pb push_back
#define ordered_set tree<ll ,  null_type ,  less_equal<> ,  rb_tree_tag ,  tree_order_statistics_node_update>
vector <int> dx {0, 0, 1, -1, 1, 1, -1, -1}, dy {1, -1, 0, 0, 1, -1, 1, -1};
//vector<int> dx{1, 1, -1, -1, 2, 2, -2, -2}, dy{2, -2, 2, -2, 1, -1, 1, -1};
 
void M7MDAN_FARAG(){
  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr); 
  #ifndef ONLINE_JUDGE
   freopen("input.txt", "r", stdin),freopen("output.txt", "w", stdout);
  #endif
}
 
const ll Mod = 1e9+7, N = 2e5 + 5, OO = 0x3f3f3f3f;
 
int n, q, m;
vector<int>adj[N];
int sparse[22][N], depth[N];
vector<int> prefix(N, 0);

void dfs(ll node, ll par = -1, ll d = 0){
  sparse[0][node] = par;
  depth[node] = d;
  for(auto child : adj[node]){
    if(child == par) continue;
    dfs(child, node, d + 1);
  }
}
 
void build(){
  dfs(1);
 
  for(int i = 1; 1 << i <= n; ++i){
    for(int j = 0; j <= n; ++j){
        sparse[i][j] = sparse[i-1][sparse[i-1][j]];
    }
  }
}
 
ll lca(ll u, ll v){
  if(depth[u] < depth[v]) swap(u, v);
 
  ll diff = depth[u] - depth[v];
 
  for(int i = 20; i >= 0; --i){
    if(diff & (1 << i)){
      u = sparse[i][u];
    }
  }
 
  if(u == v) return u;
 
  for(int i = 20; i >= 0; --i){
    if(sparse[i][u] != sparse[i][v]){
      u = sparse[i][u];
      v = sparse[i][v];
    }
  }
 
  return sparse[0][u];
}

void dfs_prefix(ll node, ll par = -1){
  for(auto child : adj[node]){
    if(child == par) continue;
    dfs_prefix(child, node);
    prefix[node] += prefix[child];
  }
}
 
void Try(){
cin >> n >> m;

for(int i = 1; i < n; ++i){
  ll u, v;
  cin >> u >> v;
  adj[u].pb(v);
  adj[v].pb(u);
}

build();

while(m--){
  ll u, v;
  cin >> u >> v;
  ll l = lca(u, v);
  prefix[u]++;
  prefix[v]++;
  prefix[l]--;
  prefix[sparse[0][l]]--;
}

dfs_prefix(1);

for(int i = 1; i <= n; ++i){
  cout << prefix[i] << " ";
}
}
 
signed main(){
M7MDAN_FARAG();
 
int t = 1;
// cin >> t;
 
while(t--){
  Try();
}
}