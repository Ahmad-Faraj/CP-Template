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
template < typename T = int > struct Centroid_Decomposition {

    int n, treeRoot;
    const vector < vector < T > > adj;
    vector < T > SubtreeSz, isCentroid;

    // Initialize the Centroid Decomposition
    Centroid_Decomposition(int N, const vector <vector < T > > &G, int Root = 1) : adj(G){
        n = N, treeRoot = Root;
        SubtreeSz = isCentroid = vector < T > (n + 5, 0);
    }

    // update subtree size of each node
    int updateSize(int u, int p = -1){
        SubtreeSz[u] = 1;
        for (int v : adj[u]) 
            if (v != p && !isCentroid[v]) 
                SubtreeSz[u] += updateSize(v, u);
        return SubtreeSz[u];
    }

    // get centroid of subtree rooted at u
    int getCentroid(int u, int target, int p = -1){
        for(auto& v : adj[u]){
            if(v == p || isCentroid[v]) continue;
            if(SubtreeSz[v] * 2 > target) 
                return getCentroid(v, target, u);
        }
        return u;
    }

    // decompose tree into centroid tree
    void Centroid(int u, int p = 0){
        int centroidPoint = getCentroid(u, updateSize(u));
        
        // do something with centroid

        isCentroid[centroidPoint] = true;
        for(auto& v : adj[centroidPoint]){
            if(isCentroid[v]) continue;
            Centroid(v, centroidPoint);
        }
    }
    
    // call this function to decompose the tree
    void Decompose(){
        Centroid(treeRoot);
    }

};
void solve(int tc)
{
    
}
signed main(void)
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