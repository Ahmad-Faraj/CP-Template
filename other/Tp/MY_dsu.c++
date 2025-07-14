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

void M7MDAN_FRAG(){
  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr); 
  #ifndef ONLINE_JUDGE
   freopen("input.txt", "r", stdin),freopen("output.txt", "w", stdout);
  #endif
}

const ll Mod = 1e9+7, N = 2e5 + 5, OO = 0x3f3f3f3f;

struct Edge{
  int u, v, w;
 
  bool operator < (const Edge &other) const{
    return w < other.w;
  }
};
 
struct DSU{
  vector<int> parent, sz;
 
  DSU(int n){
    parent.resize(n + 5);
    sz.resize(n + 5);
 
    for(int i = 0; i < n; i++){
      parent[i] = i;
      sz[i] = 1;
    }
  }
 
  int find_leader(int u){
    if(u == parent[u]) return u;
    return parent[u] = find_leader(parent[u]);
  }
 
  void unite(int u, int v){
    u = find_leader(u);
    v = find_leader(v);
 
    if(u == v) return;
 
    if(sz[u] < sz[v]) swap(u, v);
 
    parent[v] = u;
    sz[u] += sz[v];
    return;
  }
 
  bool same(int u, int v){
    return find_leader(u) == find_leader(v);
  }
 
  int GetSize(int u){
    return sz[find_leader(u)];
  }
};



void solve(){
ll n, m;
cin >> n >> m;

DSU dsu(n);

ll ans = 0;

for(int i = 0; i < m;i++){
  ll u, v;
  cin >> u >> v;
  if(!dsu.same(u, v)){
    ll sz1 = dsu.GetSize(u);
    ll sz2 = dsu.GetSize(v);
    ans += sz1 * sz2 - 1;
    dsu.unite(u, v);
  }
}
cout << ans << endl;
}


signed main(){
  M7MDAN_FRAG();
//freopen("easy.in", "r", stdin); 
//freopen("easy.out", "w", stdout);

	int test = 1, tt = 1;
  // cin >> test;
	while(test--){
		solve(); 
	}
}