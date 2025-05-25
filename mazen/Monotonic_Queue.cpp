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
struct monotonic_queue{
    // monotonic queue for minimum and maximum
    stack<int> rst, rmx, rmn, lst, lmx, lmn;
    void push(int x){
        if(rst.empty())
            rst.push(x), rmx.push(x), rmn.push(x);
        else
            rst.push(x), rmx.push(max(rmx.top(), x)), rmn.push(min(rmn.top(), x));
    }
    void pop(){
        if(lst.empty()){
            while(!rst.empty()){
                int x = rst.top();
                rst.pop(), rmx.pop(), rmn.pop();
                if(lmx.empty())
                    lmx.push(x), lmn.push(x);
                else
                    lmx.push(max(lmx.top(), x)), lmn.push(min(lmn.top(), x));
                lst.push(x);
            }
        }
        if(lst.empty())
            return;
        lst.pop(), lmx.pop(), lmn.pop();
    }
    int get_max(){
        if(rmx.empty())
            return lmx.top();
        if(lmx.empty())
            return rmx.top();
        return max(lmx.top(), rmx.top());
    }
    int get_min(){
        if(rmn.empty())
            return lmn.top();
        if(lmn.empty())
            return rmn.top();
        return min(lmn.top(), rmn.top());
    }
    int size(){
        return lst.size() + rst.size();
    }
};
void solve(int tc)
{
    int n, s;
    cin >> n >> s;
    vector<int> a(n);
    for(auto &i : a)
        cin >> i;
    int ans = 0;
    int l = 0, r = 0;
    monotonic_queue q;
    while(r < n){
        q.push(a[r]);
        //cout << q.get_max() << sp << q.get_min() << nl;
        while(l <= r && q.get_max() - q.get_min() > s)
            q.pop(), l++;
        ans += r - l + 1;
        r++;
    }
    cout << ans << nl;
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