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

template < typename T = int > using Pair = pair < T, T >;
vector < string > RET = {"NO", "YES"};

template < typename T = int > istream& operator >> (istream &in, vector < T > &v) {
    for (auto &x : v) in >> x;
    return in;
}

template < typename T = int > ostream& operator << (ostream &out, const vector < T > &v) { 
    for (const T &x : v) out << x << ' '; 
    return out;
}

const ll Mod = 1e9+7, N = 2e5 + 5, OO = 0x3f3f3f3f;

int timer;
vector < vector < int > > adj, comps;
vector < int > low_link, in_stack, node_idx, comp_idx;
stack < int > stk;
vector < Pair < int > > bridges;
set < int > art_points;

void init(int n){
    timer = 0;
    adj = vector < vector < int > > (n + 5);
    low_link = node_idx = comp_idx = vector < int > (n + 5, -1);
    in_stack = vector < int > (n + 5, false);
    comps.clear();
    stk = stack < int > ();
}

void Tarjan(int u){
    in_stack[u] = true;
    low_link[u] = node_idx[u] = timer++;
    stk.push(u);
    for(auto& v : adj[u]){
        if(node_idx[v] == -1){
            Tarjan(v);
            // minimize ancestor of my child
            low_link[u] = min(low_link[u], low_link[v]);
            // add bridge
            if(low_link[v] == node_idx[v])
                bridges.push_back({u, v});
            // add articulation point
            if(low_link[v] >= node_idx[u])
                art_points.insert(u);
        }else if(in_stack[v])
            low_link[u] = min(low_link[u], node_idx[v]);
    }

    // head of SCC
    if(low_link[u] == node_idx[u]){
        comps.push_back(vector < int > ());
        int v = -1;
        while(v != u){
            v = stk.top();
            stk.pop();
            in_stack[v] = false; // remove from stack
            comps.back().push_back(v); // add node to current component
            comp_idx[v] = sz(comps) - 1; // set component index
        }
    }
}

void SCC(int n){
    for(int i = 0; i < n; i++)
        if(node_idx[i] == -1)
            Tarjan(i);
}

void solve(){

}

signed main(){
  M7MDAN_FRAG();

	int test = 1, tt = 1;
  // cin >> test;
	while(test--){
		solve(); 
	}
}