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

template <typename T = int , int Base = 0> struct MergeSortTree{
    vector<vector<T>> tree;
    vector<T> arr;
    int n;
    MergeSortTree(vector<T> &a){
        arr = a;
        n = sz(a);
        tree.resize(4 * n);
        build(1, 0, n - 1);
    }
    void build(int node, int start, int end){
        if(start == end){
            tree[node].push_back(arr[start]);
            return;
        }
        int mid = (start + end) / 2;
        build(node * 2, start, mid);
        build(node * 2 + 1, mid + 1, end);
        // tree[node] = merge(tree[left],tree[right]);
        tree[node].resize(sz(tree[node * 2]) + sz(tree[node * 2 + 1]));
        merge(all(tree[node * 2]), all(tree[node * 2 + 1]), tree[node].begin());
    }
    int query(int node, int start, int end, int l, int r, T x){
        if(start > r || end < l) return 0;
        if(start >= l && end <= r){
            return lower_bound(all(tree[node]), x) - tree[node].begin();
        }
        int mid = (start + end) / 2;
        return query(node * 2, start, mid, l, r, x) + query(node * 2 + 1, mid + 1, end, l, r, x);
    }
    int query(int l, int r, T x){
        return query(1, 0, n - 1, l, r, x);
    }

    int update(int node, int start, int end, int idx, T val){
        if(start == end){
            tree[node].clear();
            tree[node].push_back(val);
            return 0;
        }
        int mid = (start + end) / 2;
        int res;
        if(idx <= mid){
            res = update(node * 2, start, mid, idx, val);
        }else{
            res = update(node * 2 + 1, mid + 1, end, idx, val);
        }
        tree[node].clear();
        merge(all(tree[node * 2]), all(tree[node * 2 + 1]), back_inserter(tree[node]));
        return res;
    }
};

void Try(){
ll n;
cin >> n;

vl a(n);

for(int i = 0; i < n; i++){
    cin >> a[i];
}

MergeSortTree<ll> mst(a);

ll invcnt = 0;

for(int i = 0; i < n; i++){
    invcnt += mst.query(i + 1, n - 1, a[i]);
}

cout << invcnt << endl;
}

signed main(){
    M7MDAN_FARAG();
    int t = 1;
    cin >> t;
    while(t--){
        Try();
    }
}
