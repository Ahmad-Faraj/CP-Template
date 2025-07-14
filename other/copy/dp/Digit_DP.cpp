#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define sz(x) x.size()

ll dp[20][2][2][90], k;
string L, R;

ll cnt_num(int idx = 0, bool not_smaller = true, bool not_greater = true, int sum = 0){
    if(idx == sz(L)) return !sum;
    ll &ret = dp[idx][not_smaller][not_greater][sum];
    if(~ret) return ret;
    int low = (not_smaller ? L[idx] - '0' : 0), high = (not_greater ? R[idx] - '0' : 9);
    ret = 0;
    for(int d = low; d <= high; d++)
        ret += cnt_num(idx + 1, not_smaller & (d == (L[idx] - '0')), not_greater & (d == (R[idx] - '0')), (sum + d) % k);
    return ret;
}

void solve(ll l, ll r){
    memset(dp, -1, sizeof(dp));
    L = to_string(l);
    R = to_string(r);
    if(sz(L) < sz(R))
        L = string(sz(R) - sz(L), '0') + L;
}

signed main()
{
    // Ahmed_Faraj();
    int test = 1;
    // cin >> test;
    for (int tc = 1; tc <= test; tc++)
    {
        // cout << "Case " << tc << ":";
        // solve();
    }
    return 0;
}
