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

template < typename T = int >
struct Dijkstra {

    struct Edge {
        T v, w;
        Edge(T V = 0, T W = 0): v(V), w(W) {}
        bool operator < (const Edge& e) const {
            return w > e.w;
        }
    };

    vector < vector < Edge > > adj;

    Dijkstra(int edges, bool indirected = true) {
        adj.resize(edges);
        for(int i = 0, u, v, w; i < edges; i++) {
            cin >> u >> v >> w;
            adj[u].push_back(Edge(v, w));
            if(indirected)
                adj[v].push_back(Edge(u, w));
        }
    }

    T Min_Cost(int src, int dest) {
        int n = adj.size();
        vector < T > dist(n, LLONG_MAX);
        dist[src] = 0;
        priority_queue < Edge > Dij;
        Dij.push(Edge(src, 0));
        while(!Dij.empty()) {
            Edge top = Dij.top();
            T u = top.v;
            T cost = top.w;
            Dij.pop();
            for(const Edge& edge : adj[u]) {
                T v = edge.v;
                T w = edge.w;
                if(dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    Dij.push(Edge(v, dist[v]));
                }
            }
        }
        return (dist[dest] == LLONG_MAX ? -1 : dist[dest]);
    }

    vector < T > get_dist(int src) {
        int n = adj.size();
        vector < T > dist(n, LLONG_MAX);
        dist[src] = 0;
        priority_queue < Edge > Dij;
        Dij.push(Edge(src, 0));
        while(!Dij.empty()) {
            Edge top = Dij.top();
            T u = top.v;
            T cost = top.w;
            Dij.pop();
            for(const Edge& edge : adj[u]) {
                T v = edge.v;
                T w = edge.w;
                if(dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    Dij.push(Edge(v, dist[v]));
                }
            }
        }
        return dist;
    }

    vector< T > get_path(int src, int dest) {
        int n = adj.size();
        vector < T > dist(n, LLONG_MAX);
        vector < T > parent(n, -1);
        dist[src] = 0;
        priority_queue < Edge > Dij;
        Dij.push(Edge(src, 0));
        while(!Dij.empty()) {
            Edge top = Dij.top();
            T u = top.v;
            T cost = top.w;
            Dij.pop();
            for(const Edge& edge : adj[u]) {
                T v = edge.v;
                T w = edge.w;
                if(dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    Dij.push(Edge(v, dist[v]));
                }
            }
        }
        vector < T > path;
        for(int i = dest; i != -1; i = parent[i])
            path.push_back(i);
        reverse(path.begin(), path.end());
        return path;
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
  int n, m;
  cin >> n >> m;

  Dijkstra<ll> dij(m, 1);

  vector<ll> dist = dij.get_path(1, n);
  
  for(auto& i : dist)
    cout << i << " ";
  cout << endl;
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