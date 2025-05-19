#include <bits/stdc++.h>
 
using namespace std;
 
#define ll long long
#define PI acos(-1)
#define all(x) x.begin(),x.end()
#define clr(v, d) memset(v, d, sizeof(v))
#define endl "\n"
#define sz(x) (int)x.size()
#define vl vector<ll>
#define pb push_back
#define modo  1000000007
 
void M7MDAN_FARAG() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    #ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}
 
const ll N = 1e6 + 5, OO = 1LL << 60 , M = 1e5+5;
 
ll h1[N], h2[N], pw1[N], pw2[N];
 
ll add(ll a, ll b, ll mod) { return (a + b) % mod; }
ll sub(ll a, ll b, ll mod) { return (a - b + 2 * mod) % mod; }
ll mul(ll a, ll b, ll mod) { return ((a % mod) * (b % mod)) % mod; }
 
template < typename T = long long, bool Base = 0 > struct Hash {
 
  static bool done;
  static constexpr int p[2] = {31, 313};
  static constexpr int mods[2] = {1000000007, 1000000009};
 
  inline static constexpr void precumpute() {
    done = true;
    pw1[0] = pw2[0] = h1[0] = h2[0] = 1;
    for (int i = 1; i < N; ++i) {
      pw1[i] = mul(pw1[i - 1], p[0], mods[0]);
      pw2[i] = mul(pw2[i - 1], p[1], mods[1]);
    }    
  }
 
  inline static void calc(const auto& v){ // {string, vector}
    int n = sz(v);
    for(int i = 1; i <= n; i++) {
      h1[i] = (h1[i - 1] * p[0] + v[i - !Base]) % mods[0];
      h2[i] = (h2[i - 1] * p[1] + v[i - !Base]) % mods[1];
    }
  }
 
  Hash(const auto & v){
    if (!done) 
      precumpute();
    calc(v);
  }
 
  // calculate the hash of the position pos
  const inline pair < T, T > at(const T &pos){
    return get_hash(pos, pos);
  }
 
  // calculate the hash of the substring s[l..r]
  const inline pair < T, T > get_hash(const T &l, const T &r) {
    // hash of s[l..r] based on p1
    T F = h1[r];
    F = sub(F, mul(h1[l - 1], pw1[r - l + 1], mods[0]), mods[0]);
    
    // hash of s[l..r] based on p1
    T S = h2[r];
    S = sub(S, mul(h2[l - 1], pw2[r - l + 1], mods[1]), mods[1]);
    return {F, S};
  }
 
  const inline bool is_same(const T &l1, const T &r1, const T &l2, const T &r2){
    return get_hash(l1, r1) == get_hash(l2, r2);
  }
  
  // merge the hashes of two substrings s1[l1..r1] and s2[l2..r2]
  const inline pair < T, T > merge(const T &l1, const T &r1, const T &l2, const T &r2) {
    auto h1 = get_hash(l1, r1), h2 = get_hash(l2, r2);
    T F = add(mul(h1.first, pw1[r2 - l2 + 1], mods[0]), h2.first, mods[0]);
    T S = add(mul(h1.second, pw2[r2 - l2 + 1], mods[1]), h2.second, mods[1]);
    return {F, S};
  }
};
 
template <typename T, bool Base> bool Hash < T, Base > :: done = false;
 
 
void solve() {
string s;
cin >> s;
ll n = s.size();
 
Hash <ll, 0> h(s);
 
for(int i = 1; i < n; i++){
    if(h.get_hash(1, i) == h.get_hash(n-i+1, n)){
        cout << i << " ";
    }
}
}
 
signed main() {
    M7MDAN_FARAG();
 
    ll t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }
}
