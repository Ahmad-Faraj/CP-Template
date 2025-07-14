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

template < typename T = int , int Base = 0 > struct Floyd {
    
    int n;
    vector < vector < T > > dist;
    const T DEF = numeric_limits < T > :: max() / 2;

    Floyd(int _n = 0) : n(_n), dist(n + 5, vector < T > (n + 5, DEF)) { 
        for(int i = 1; i <= n; i++)
            dist[i][i] = 0; // self loop
    }

    Floyd(int _n, const vector < vector < T > > &D) : n(_n), dist(n + 5, vector < T > (n + 5, DEF)) { 
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= n; j++)
                dist[i][j] = D[i - !Base][j - !Base];
    }

    T operation(T a, T b){
        return min(a, b);
    }

    void add_edge(int u, int v, T w){
        dist[u][v] = operation(dist[u][v], w);
    }

    void build(){
        for(int i = 1; i <= n; i++)
            for(int u = 1; u <= n; u++)
                for(int v = 1; v <= n; v++)
                    update_dist(u, v, i);
    }

    T get_dist(int u, int v){
        return dist[u][v];
    }

    void update_dist(int u, int v, int a, int b){
        dist[u][v] = operation(dist[u][v], dist[u][a] + dist[a][b] + dist[b][v]);
    }

    void update_dist(int u, int v, int k){
        dist[u][v] = operation(dist[u][v], dist[u][k] + dist[k][v]);
    }
};

void M7MDAN_FRAG(){
  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr); 
  #ifndef ONLINE_JUDGE
   freopen("input.txt", "r", stdin),freopen("output.txt", "w", stdout);
  #endif
}

const ll Mod = 1e9+7, N = 2e5 + 5, OO = 0x3f3f3f3f;

void solve(){

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