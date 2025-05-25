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
// if the sum of all elements bounded <= N
void solve(int tc)
{
    int n;
    cin >> n;
    vector<int> freq(n + 1);
    for(int i = 0; i < n; i++){
        int x;
        cin >> x;
        freq[x]++;
    }
    vector<pair<int, int>> vc;
    for(int i = 0; i <= n; i++){
        if(freq[i])
            vc.push_back({i, freq[i]});
    }

    vector<int> dp(n + 1);
    dp[0] = 1;
    for(auto &[w, k] : vc){
        vector<int> ndp = dp;
        for(int p = 0; p < w; p++){
            int sum = 0;
            for(int j = p, cnt = 0; j <= n; j += w, cnt++){
                if(cnt > k){
                    sum -= dp[j - cnt * w];
                    cnt--;
                }
                if(sum)
                    ndp[j] = 1;
                sum += dp[j];
            }
        }
        swap(ndp, dp);
    }
    int q;
    cin >> q;
    while(q){
        int x;
        cin >> x;
        cout << (dp[x] ? "YES" : "NO") << nl;
    }
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