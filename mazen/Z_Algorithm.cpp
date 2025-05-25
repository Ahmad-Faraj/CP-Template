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
struct Zfunctions
{
    vector<int> z;
    void build(string s)
    {
        int n = sz(s);
        z.assign(n, 0);
        for (int i = 1, l = 0, r = 0; i < n; i++)
        {
            if (i <= r)
            {
                z[i] = min(r - i + 1, z[i - l]);
            }
            while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            {
                z[i]++;
            }
            if (i + z[i] - 1 > r)
            {
                l = i;
                r = i + z[i] - 1;
            }
        }
    }
    int operator[](int i)
    {
        return z[i];
    }
    vector<int> match(string s, string pattern)
    {
        vector<int> ans;
        string t = pattern + "#" + s;
        build(t);
        for (int i = sz(pattern) + 1; i < sz(t); i++)
        {
            if (z[i] >= sz(pattern))
            {
                ans.push_back(i - sz(pattern) - 1);
            }
        }
        return ans;
    }

    friend ostream &operator<<(ostream &out, Zfunctions &z)
    {
        for (int i = 0; i < sz(z.z); i++)
        {
            out << z.z[i] << sp;
        }
        return out;
    }
};
void solve(int tc)
{
    string s;
    cin >> s;
    Zfunctions z;
    z.build(s);
    cout << z << nl;
    vector<int> v = z.match(s, "ab");
    for (int i = 0; i < sz(v); i++)
    {
        cout << v[i] << sp;
    }
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