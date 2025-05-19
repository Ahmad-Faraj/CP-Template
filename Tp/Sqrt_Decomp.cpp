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
vector <int> dx {0, 0, 1, -1}, dy {1, -1, 0, 0};
//vector<int> dx{1, 1, -1, -1, 2, 2, -2, -2}, dy{2, -2, 2, -2, 1, -1, 1, -1};
 
void M7MDAN_FARAG(){
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr); 
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}

const ll Mod = 1e9+7, N = 2e5 + 5, OO = 0x3f3f3f3f, INF = 0x3f3f3f3f3f3f3f3f;

template < typename T = int > istream& operator >> (istream &in, vector < T > &v) {
    for (auto &x : v) in >> x;
    return in;
}

template < typename T = int > ostream& operator << (ostream &out, const vector < T > &v) { 
    for (const T &x : v) out << x << ' '; 
    return out;
}

template < typename T = int, int Base = 0 > struct Sqrt_Decomp {

    int n, len;
    vector < T > a; 
    vector < vector < T > > b;
    T U_Default, Q_Default;

    // calculate the length of each block
    T calc_sq(int N){
        int sq = sqrt(N);
        return sq * sq == N ? sq : sq + 1;
    }

    Sqrt_Decomp(int N = 0){
        n = N, len = calc_sq(n), U_Default = 0, Q_Default = 0;
        a = vector < T > (n + 5, U_Default);
        b = vector < vector < T > > (len + 5);
    }

    Sqrt_Decomp(int N, const vector < T >& vec){
        n = N, len = calc_sq(n), U_Default = 0, Q_Default = 0;
        a = vec;
        b = vector < vector < T > > (len + 5);
        build();
    }

    // build each block
    void build(){
        for(int i = 1; i <= n; i++)
            b[i / len].push_back(a[i - !Base]);
        for(int i = 0; i <= len; i++)
            sort(all(b[i]));
    }

    // just update this index in the block O(1)
    void update(int idx, T val){
        int idx2 = lower_bound(all(b[idx / len]), a[idx - !Base]) - b[idx / len].begin();
        b[idx / len][idx2] = a[idx - !Base] = val;
        sort(all(b[idx / len]));
    }

    // re-calculate the block again O(sqrt(n))
    void update_range(int idx, T val){
        a[idx - !Base] = val;
        b[idx / len].clear();
        for(int i = idx / len * len; i < min(n, (idx / len + 1) * len); i++)
            b[idx / len].push_back(a[i - !Base]);
        sort(all(b[idx / len]));
    }

    // query on the range from L to R
    T query(int l, int r, T x){
        T res = Q_Default;
        while(l < r && l % len != 0)
            res += a[l++ - !Base] >= x;
        while(l + len <= r){
            res += sz(b[l / len]) - (lower_bound(all(b[l / len]), x) - b[l / len].begin());
            l += len;
        }
        while(l <= r)
            res += a[l++ - !Base] >= x;
        return res;
    }
};

void Try(){

}

signed main(){
    M7MDAN_FARAG();
    int t = 1;
    cin >> t;
    while(t--){
        Try();
    }
}
