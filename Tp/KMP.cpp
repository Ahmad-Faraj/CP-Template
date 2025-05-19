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
 
const ll N = 1e6 + 5, OO = 1e18 + 5, M = 1e5+5;
 
struct KMP {
 
    string pattern;
    vector < int > lp;
 
    KMP(const string& str = "") : pattern(str) {
        build();
    }
 
    void build(){
        int n = sz(pattern);
        lp.resize(n);
        for(int i = 1; i < n; i++)
            lp[i] = failure(lp[i - 1], pattern[i]);
    }
 
    int failure(int idx, char nxt){
        while(idx > 0 && pattern[idx] != nxt)
            idx = lp[idx - 1];
        return idx + (pattern[idx] == nxt);
    }
 
    vector < int > match(const string& str){
        int n = sz(str), m = sz(pattern);
        vector < int > ret;
        for(int i = 0, k = 0; i < n; i++){
            k = failure(k, str[i]);
            if(k == m)
                ret.push_back(i - m + 1); // 0-based indexing
        }
 
        for(auto& idx : ret)
            idx++; // 1-based indexing
        
        return ret;
    }
 
    vector < int > count(const string& str){ // count number of occurrences of each prefix
        int n = sz(str), m = sz(pattern);
        vector < int > cnt(m + 1);
        for(int i = 1, k = 0; i < n; i++){
            k = failure(k, str[i]);
            cnt[k]++;
        }
 
        for(int i = m - 1; i > 0; i--)
            cnt[lp[i - 1]] += cnt[i]; // number of occurrences of prefix of length lp[i - 1] is at least cnt[i]
 
        for(int i = 0; i <= m; i++) 
            cnt[i]++; // add the whole substring itself
 
        return cnt;
    }
 
    vector < int > get_prefixes(){
        vector < int > indexes = {sz(pattern)};
        for(int i = lp[sz(pattern) - 1]; i > 0; i = lp[i - 1])
            indexes.push_back(i);
        sort(all(indexes)); // indexes of prefixes increasing
        return indexes;
    }
};
 
void solve() {

}
 
signed main() {
    M7MDAN_FARAG();
 
    ll t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }
}
