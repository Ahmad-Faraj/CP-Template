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
        sort(indexes.begin(), indexes.end()); // indexes of prefixes increasing
        return indexes;
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