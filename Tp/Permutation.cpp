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
const ll aka =3e5 + 5;
const long long OO =(ll)1e18;
//int dx[8] = {0,0,1,-1,1,-1,1,-1};
//nt dy[8] = {1,-1,0,0,1,-1,-1,1};
/*-------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/
//Sloution ->
// https://codeforces.com/contest/1352/problem/G

// see a permutation of length n, where n is odd, such that the absolute difference between any two adjacent elements is at least 2 and at most 4.
// generate all permutations and check if it's valid or not
bool check(vl &v){

 for(int i=0;i<v.size()-1;i++){

  ll x=abs(v[i]-v[i+1]);

    if(x<=1 or x>=5){
    return false;
    }
 }

  return true;

}

void generateperm(vl v){

  sort(all(v));
  do{

    if(check(v)){
      for(auto i : v) cout << i << " ";
      cout << endl;
    }

  }while(next_permutation(all(v)));
}

void solve(){

  ll n;cin>>n;

  vl v(n);
  for(auto &i : v) cin>>i;

  generateperm(v);
  
}

signed main(){
  M7MDAN_FRAG();cout << fixed << setprecision(10);
//freopen("easy.in", "r", stdin); freopen("easy.out", "w", stdout);
	int t=1; 
cin>>t;
	while(t--){
		solve(); 
	}
}