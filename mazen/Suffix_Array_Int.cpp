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
struct SuffixArray{
    #define all(x) x.begin(), x.end()
    vector<int> s;
    int n, LOG;
    vector<int> p, c, lcp;
    explicit SuffixArray() = default;
    explicit SuffixArray(vector<int> _s) : n(sz(_s)+1){
        s = _s;
        s.push_back(0);
        p = c = lcp = vector<int>(n);
        build();
        build_lcp();
    }
    inline void count_sort(){
        vector<int> cnt(n, 0), pos(n, 0), p_new(n);
        for(auto &x: c)
            cnt[x]++;
        for(int i = 1; i < n; i++)
            pos[i] = pos[i - 1] + cnt[i - 1];
        for(auto &x : p)
            p_new[pos[c[x]]++] = x;
        p = p_new;
    }
    inline void build(){
        vector<pair<int, int>> a(n);
        for(int i = 0; i < n; i++)
            a[i] = {s[i], i};
        sort(all(a));
        for(int i = 0; i < n; i++)
            p[i] = a[i].second;
        c[p[0]] = 0;
        for(int i = 1; i < n; i++)
            c[p[i]] = c[p[i - 1]] + (a[i].first != a[i - 1].first);
 
        int k = 0;
        while(((1 << k) < n) && p.back() != n - 1){
            for (int i = 0; i < n; i++)
                p[i] = (p[i] - (1 << k) + n) % n;
            count_sort();
            vector<int> c_new(n);
            c_new[p[0]] = 0;
            for(int i = 1; i < n; i++){
                pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << k)) % n]};
                pair<int, int> now = {c[p[i]], c[(p[i] + (1 << k)) % n]};
                c_new[p[i]] = c_new[p[i - 1]] + (prev != now);
            }
            c = c_new;
            k++;
        }
    }
    inline void build_lcp(){
        int k = 0;
        for(int i = 0; i < n - 1; i++){
            int pi = c[i];
            int j = p[pi - 1];
            while(s[i + k] == s[j + k])
                k++;
            lcp[pi] = k;
            k = max(k - 1, 0LL);
        }
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