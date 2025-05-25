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
struct manacher
{
    vector<int> p;
    string s;
    manacher(string str)
    {
        s = str;
        build();
    }
    void build()
    {
        string t = "#";
        for (auto c : s)
            t += c, t += "#";
        int n = sz(t);
        s = t;
        p.assign(n, 1);
        run_manacher(t);
    }
    void run_manacher(string t)
    {
        int n = sz(t);
        int l = 1, r = 1;
        for (int i = 1; i < n; i++)
        {
            p[i] = max(0ll, min(p[l + r - i], r - i));
            while (i - p[i] >= 0 && i + p[i] < n && t[i - p[i]] == t[i + p[i]])
                p[i]++;
            if (i + p[i] > r)
                l = i - p[i], r = i + p[i];
        }
    }
    // ababa
    // #a#b#a#b#a#
    // cen = 0, odd = 1 -> pos = 1 
    // cen = 0, odd = 0 -> pos = 2
    // cen = 1, odd = 1 -> pos = 3
    // cen = 1, odd = 0 -> pos = 4
    // return the longest palindrome centered at cen with odd or even length
    int getLongest(int cen, bool odd){
        int pos = 2 * cen + 1 + (!odd);
        return p[pos] - 1;
    }
    // check if the substring [l, r] is a palindrome
    bool checkPalindrome(int l, int r){
        int len = r - l + 1;
        int cen = (l + r) / 2;
        if(len & 1)
            return getLongest(cen, true) >= len;
        return getLongest(cen, false) >= len;
    }
};
void solve(int tc)
{
    string s;
    cin >> s;
    manacher m(s);
    for (auto x : m.s)
        cout << x << sp;
    cout << nl;
    for (auto x : m.p)
        cout << x << sp;
    cout << nl;
}
signed main(void)
{
    fastio();
    int tc = 1;
    // cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}