/** حَسْبِيَ اللَّهُ لا إِلَـهَ إِلاَّ هُوَ عَلَيْهِ تَوَكَّلْتُ وَهُوَ رَبُّ الْعَرْشِ الْعَظِيمِ **/
#include <bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;
#define ll long long 
#define int long long
#define PI acos(-1)
#define all(x) x.begin(),x.end()
#define clr(v, d) memset(v, d, sizeof(v))
#define endl "\n"
#define vl vector<ll>
#define vi vector<int>
#define pb push_back
#define MOD ((ll) 1e9 + 7)
void M7MDAN_FRAG(){
  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr); 
  #ifndef ONLINE_JUDGE
   freopen("input.txt", "r", stdin),freopen("output.txt", "w", stdout);
  #endif
}
typedef tree<int, null_type, less_equal<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
const ll aka =1e5 + 5;
const long long OO =(ll)1e18;
//int dx[8] = {0,0,1,-1,1,-1,1,-1};
//nt dy[8] = {1,-1,0,0,1,-1,-1,1};
/*-------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/
//Sloution -> 
struct Nod{
 int num;
};
const int N=2e5+1;
int n;
int s[N],s2[N],s3[N];

struct segmentTree
{
    int sz;
     vector<int>seg;
     vector<int>lazy; 
     int ret=1e9;

    segmentTree(int n)
    {
        sz=1;
        while(sz<n)sz*=2;
        seg.resize(2*sz);
        lazy.resize(2*sz);
    }

    
    int merge(int a,int b)
    {
        return min(a,b);
    }
////////////////
////////////////////////////////////////////////////////////////
   
void build_tree(int node, int l, int r) {
  	if(l > r) return; 
  	
  	if(l == r) { 
    		seg[node] = s[l]; 
    		//cout<<node<<" "<<tree[node]<<endl;
		return;
	}
	
     int  mid = (l + r) / 2;

    build_tree(node*2, l, mid); 
    build_tree(node*2+1, mid+1, r);

    seg[node]=merge(seg[node*2],seg[node*2+1]);
}

void update_tree(int node, int l, int r, int lx, int rx, int value) {
  
  	if(lazy[node] != 0) { 
   		seg[node] += lazy[node]; 

		if(l != r) {
			lazy[node*2] += lazy[node];
    			lazy[node*2+1] += lazy[node]; 
		}

   		lazy[node] = 0; 
  	}
  
	if(l > r || l > rx || r < lx) 
		return;
    
  	if(l >= lx && r <= rx) { 
    		seg[node] += value;

		if(l != r) { 
			lazy[node*2] += value;
			lazy[node*2+1] += value;
		}

    		return;
	}

	int mid = (l + r) / 2;

    update_tree(node*2, l, mid, lx, rx, value);
    update_tree(node*2 + 1, mid + 1, r, lx, rx, value);

    seg[node]=merge(seg[node*2],seg[node*2+1]);
}

int query_tree(int node, int l, int r, int lx, int rx) {
	
	if(l > r || l > rx || r < lx) return ret; 

	if(lazy[node] != 0) { 
		seg[node] += lazy[node];

		if(l!=r) {
			lazy[node*2] += lazy[node]; 
			lazy[node*2+1] += lazy[node]; 
		}

		lazy[node] = 0;
	}

	if(l >= lx && r <= rx)  return seg[node];
    
    int mid = (l + r) / 2;

    int q1 = query_tree(node*2, l, mid, lx, rx);
    int q2 = query_tree(node*2 + 1, mid + 1, r, lx, rx);

    return merge(q1,q2);
}

};

struct segmentTree2
{
    int sz;
     vector<int>seg;
     vector<int>lazy; 
     int ret=1e9;

    segmentTree2(int n)
    {
        sz=1;
        while(sz<n)sz*=2;
        seg.resize(2*sz);
        lazy.resize(2*sz);
    }

    
    int merge(int a,int b)
    {
        return min(a,b);
    }
////////////////
////////////////////////////////////////////////////////////////
   
void build_tree(int node, int l, int r) {
  	if(l > r) return; 
  	
  	if(l == r) { 
    		seg[node] = s[l]; 
    		//cout<<node<<" "<<tree[node]<<endl;
		return;
	}
	
     int  mid = (l + r) / 2;

    build_tree(node*2, l, mid); 
    build_tree(node*2+1, mid+1, r);

    seg[node]=merge(seg[node*2],seg[node*2+1]);
}

void update_tree(int node, int l, int r, int lx, int rx, int value) {
  
  	if(lazy[node] != 0) { 
   		seg[node] += lazy[node]; 

		if(l != r) {
			lazy[node*2] += lazy[node];
    			lazy[node*2+1] += lazy[node]; 
		}

   		lazy[node] = 0; 
  	}
  
	if(l > r || l > rx || r < lx) 
		return;
    
  	if(l >= lx && r <= rx) { 
    		seg[node] += value;

		if(l != r) { 
			lazy[node*2] += value;
			lazy[node*2+1] += value;
		}

    		return;
	}

	int mid = (l + r) / 2;

    update_tree(node*2, l, mid, lx, rx, value);
    update_tree(node*2 + 1, mid + 1, r, lx, rx, value);

    seg[node]=merge(seg[node*2],seg[node*2+1]);
}

int query_tree(int node, int l, int r, int lx, int rx) {
	
	if(l > r || l > rx || r < lx) return ret; 

	if(lazy[node] != 0) { 
		seg[node] += lazy[node];

		if(l!=r) {
			lazy[node*2] += lazy[node]; 
			lazy[node*2+1] += lazy[node]; 
		}

		lazy[node] = 0;
	}

	if(l >= lx && r <= rx)  return seg[node];
    
    int mid = (l + r) / 2;

    int q1 = query_tree(node*2, l, mid, lx, rx);
    int q2 = query_tree(node*2 + 1, mid + 1, r, lx, rx);

    return merge(q1,q2);
}

};

void solve(){

    int m; cin>>n;
    segmentTree st(n+5);
    for(int i=1;i<=n;i++){
        cin>>s[i];
    }
    st.build_tree(1,1,n);
   
 cin>>m;

   while(m--){
      
   
   }
}

signed main(){
  M7MDAN_FRAG();cout << fixed << setprecision(0);
//freopen("easy.in", "r", stdin); freopen("easy.out", "w", stdout);
	int t=1; 
//cin>>t;
	while(t--){
		solve(); 
	}
}