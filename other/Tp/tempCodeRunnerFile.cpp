/** حَسْبِيَ اللَّهُ لا إِلَـهَ إِلاَّ هُوَ عَلَيْهِ تَوَكَّلْتُ وَهُوَ رَبُّ الْعَرْشِ الْعَظِيمِ **/
#include <bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;
#define ll long long 
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
const ll N =3e5 +5;
const long long OO =(ll)1e18;
//int dx[] = {0, 1, -1, 0, 1, 1, -1, -1};
//int dy[] = {1, 0, 0, -1, 1, -1, 1, -1};
/*-------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/
//Sloution ->

tuple<ll,ll,ll> fun(ll n){
  if(n%2==1){
    return {1,n/2,n/2};
  
  }
  else if(n%4==0){
    return {n/2,n/4,n/4};
  }
  else{
    return {2,n/2-1,n/2-1};
  }
}

void solve(){

 ll n,k; cin >> n >> k;
  for(int i=1;i<=k-3;i++){
    cout << 1 << " ";
  }
  auto [a,b,c] = fun(n-k+3);
  cout << a << " " << b << " " << c << endl;
}

signed main(){
  M7MDAN_FRAG();cout << fixed << setprecision(1);
//freopen("easy.in", "r", stdin); freopen("easy.out", "w", stdout);
	int t=1; cin>>t;
	while(t--){
		solve(); 
	}
}