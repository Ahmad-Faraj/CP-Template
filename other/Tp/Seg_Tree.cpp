#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

#define ll long long 
#define all(x) x.begin(),x.end()
#define sz(x) (int)x.size()
#define vl vector<ll>
#define pb push_back
#define ordered_set tree<ll ,  null_type ,  less_equal<> ,  rb_tree_tag ,  tree_order_statistics_node_update>

void M7MDAN_FARAG(){
  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr); 
  #ifndef ONLINE_JUDGE
   freopen("input.txt", "r", stdin),freopen("output.txt", "w", stdout);
  #endif
}

template < typename T = int , int Base = 0 > struct Segment_Tree {

    struct Node {

        T val;

        Node(T V = 0) : val(V) {}
    
        Node operator = (const T rhs) {
            val = rhs;
            return *this;
        }

    };

    int size; 
    Node DEFAULT;
    vector < Node > tree; 
    #define LEFT (idx << 1)
    #define RIGHT ((idx << 1) | 1)
    #define VAL val
    
    Segment_Tree(int n = 0){
        size = 1, DEFAULT = 0;
        while(size < n) size *= 2;
        tree = vector < Node > (2 * size, DEFAULT);
    }

    Segment_Tree(int n, const vector < T >& nums){
        size = 1, DEFAULT = 0;
        while(size < n) size *= 2;
        tree = vector < Node > (2 * size, DEFAULT);
        build(nums);
    }

    // Main operation to do

    Node operation(const Node& a, const Node& b){
        return a.val + b.val;
    }
    
    // If Base is 1 so the array is 1-based else the array is 0-based
    
    void build(const vector < T >& nums, int idx, int lx, int rx){
        if(Base ? lx >= sz(nums) : lx > sz(nums)) return;
        if(rx == lx) tree[idx] = nums[lx - !Base];
        else {
            int mx = (rx + lx) / 2;
            build(nums, LEFT, lx, mx);
            build(nums, RIGHT, mx + 1, rx);
            tree[idx] = operation(tree[LEFT], tree[RIGHT]);
        }
    }

    void build(const vector < T >& nums){
        build(nums, 1, 1, size);
    }

    void update(int index, T v, int idx, int lx, int rx){
        if(rx == lx) tree[idx] = v;
        else {  
            int mx = (rx + lx) / 2;
            if(index <= mx) update(index, v, LEFT, lx, mx);
            else update(index, v, RIGHT, mx + 1, rx);
            tree[idx] = operation(tree[LEFT], tree[RIGHT]);
        }
    }

    void update(const int index, const T v){
        update(index, v, 1, 1, size);
    }

    Node query(int l, int r, int idx, int lx, int rx){
        if(lx > r || l > rx) return DEFAULT;
        if(lx >= l && rx <= r) return tree[idx];
        int mx = (lx + rx) / 2;
        return operation(query(l, r, LEFT, lx, mx), query(l, r, RIGHT, mx + 1, rx));
    }

    Node query_Node(const int l, const int r){
        return query(l, r, 1, 1, size);
    }

    T query(const int l, const int r){
        return query_Node(l, r).VAL;
    }

    T get(const int idx){
        return query_Node(idx, idx).VAL;
    }

    friend ostream& operator << (ostream &out, const Node &node) {
        out << node.VAL << ' ';
        return out;
    }

    void print(int idx, int lx, int rx){
        if(lx == rx) cout << tree[idx] << ' ';
        else {
            int mx = (lx + rx) / 2;
            print(LEFT, lx, mx);
            print(RIGHT, mx + 1, rx);
        }
    }

    void print(){
        print(1, 1, size);
        cout << '\n';
    }
    
    // remove macro LEFT and RIGHT
    #undef LEFT
    #undef RIGHT
    #undef VAL
};

set<int> s;
set<int> v[100005];
void Try() {
ll n, q;
cin >> n >> q;
Segment_Tree <ll> st(n);
vector<ll> a(n);

for(ll i = 0; i < n; i++){
    cin >> a[i];
    v[a[i]].insert(i);
    s.insert(a[i]);
    st.update(i + 1, 1);
}


ll last = 0;
ll ans = 0;

while(sz(s)){
    ll x = *s.begin();
    s.erase(s.begin());
    ll l = *v[x].begin();
    ll r = *v[x].rbegin();
    if(l > last){
        ans += st.query(last + 1, l);
        last = r;
    }else{
        set<int> :: iterator it = v[x].lower_bound(last);
        it--;
        ans +=  st.query(1, *it) + st.query(last, n);
        last = *it;
    }
}

cout << ans << '\n';
}

signed main() {
    M7MDAN_FARAG();
    int t = 1;
    // cin >> t;
    while(t--) {
        Try();
    }
}
