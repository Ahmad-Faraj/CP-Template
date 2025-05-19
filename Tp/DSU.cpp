#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

void M7MDAN_FRAG(){
  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr); 
  #ifndef ONLINE_JUDGE
   freopen("input.txt", "r", stdin),freopen("output.txt", "w", stdout);
  #endif
}

#define cin_2d(vec, n, m) for(int i = 0; i < n; i++) for(int j = 0; j < m && cin >> vec[i][j]; j++);
#define cout_2d(vec, n, m) for(int i = 0; i < n; i++, cout << "\n") for(int j = 0; j < m && cout << vec[i][j] << " "; j++);
#define fixed(n) fixed << setprecision(n)
#define ceil(n, m) (((n) / (m)) + ((n) % (m) ? 1 : 0))
#define fill(vec, value) memset(vec, value, sizeof(vec));
#define mul_mod(a, b, m) (((a % m) * (b % m)) % m)
#define add_mod(a, b, m) (((a % m) + (b % m)) % m)
#define all(vec) vec.begin(), vec.end()
#define rall(vec) vec.rbegin(), vec.rend()
#define sz(x) int(x.size())
#define debug(x) cout << #x << ": " << (x) << "\n";
#define fi first
#define se second
#define ll long long
#define ull unsigned long long
#define Mod  1'000'000'007
#define OO 2'000'000'000
#define EPS 1e-9
#define PI acos(-1)
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

template < typename T = int, int Base = 1 > struct DSU {
    
    vector < T > parent, Gsize, nxt, tail, pos, roots;

    DSU(int MaxNodes){
        parent = Gsize = roots = tail = pos = nxt = vector < T > (MaxNodes + Base);
        for(int i = Base; i < MaxNodes + Base; i++){
            parent[i] = roots[i] = pos[i] = tail[i] = i;
            nxt[i] = -1, Gsize[i] = 1;
        }
    }
    
    T find_leader(int node){
        return parent[node] = (parent[node] == node ? node : find_leader(parent[node]));
    }

    bool is_same_sets(int u, int v){
        return find_leader(u) == find_leader(v);
    }

    void union_sets(int u, int v){
        int leader_u = find_leader(u), leader_v = find_leader(v);
        if(leader_u == leader_v) return;
        // make leader_u is the leader with the larger component
        if(Gsize[leader_u] < Gsize[leader_v]) 
            swap(leader_u, leader_v);
        int p = pos[leader_v];
        Gsize[leader_u] += Gsize[leader_v];
        parent[leader_v] = leader_u;
        roots[p] = roots.back();
        pos[roots[p]] = p;
        roots.pop_back();
        nxt[tail[leader_u]] = leader_v;
        tail[leader_u] = tail[leader_v];
    }

    void print(){
        for(int root = Base; root < sz(roots); root++){
            for(int u = roots[root]; ~u ; u = nxt[u])
                cout << u << " \n"[!~nxt[u]];
        }
    }

    int get_size(int u){
        return Gsize[find_leader(u)];
    }

    int get_components_number(){
        return sz(roots) - Base;
    }
};

void Solve(){
    int n, m;
    cin >> n >> m;

    DSU < int > dsu(n);

    for(int i = 0; i < m; i++){
        string s;
        cin >> s;

        if(s == "union"){
            int u, v;
            cin >> u >> v;
            dsu.union_sets(u, v);
            continue;
        }
        int u;
        cin >> u;
        int px = dsu.find_leader(u);
        cout << px << " " << px + dsu.get_size(u) - 1 << " " << dsu.get_size(u) << "\n";
    }
}

int main(){
    M7MDAN_FRAG();
    int t = 1;
    //cin >> t;
    while(t--)
        Solve();
    return 0;
}
