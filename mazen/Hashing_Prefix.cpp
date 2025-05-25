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
// hashing
const int N = 1e6 + 5;
const int Base = 31, Mod = 1e9 + 9;
int pw[N], inv[N];
void init()
{
    pw[0] = 1;
    for (int i = 1; i < N; i++)
        pw[i] = (pw[i - 1] * Base) % Mod;
    inv[N - 1] = 1;
    int invBase = 1;
    for (int i = N - 2; i >= 0; i--)
    {
        inv[i] = (invBase * pw[i + 1]) % Mod;
        invBase = (invBase * Base) % Mod;
    }
}
// prefix hashing
struct Hash
{
    int hsh, len;
};
struct Hashing
{
    vector<Hash> h;
    Hashing(string &s)
    {
        int n = sz(s);
        h.resize(n);
        h[0].hsh = s[0] - 'A' + 1;
        h[0].len = 1;
        for (int i = 1; i < n; i++)
        {
            h[i].hsh = (h[i - 1].hsh * Base + s[i] - 'A' + 1) % Mod;
            h[i].len = h[i - 1].len + 1;
        }
    }
    Hash query(int l, int r) // 0-based index
    {
        if (l == 0)
            return h[r];
        int len = h[r].len - h[l - 1].len;
        int hsh = (h[r].hsh - (h[l - 1].hsh * pw[len]) % Mod + Mod) % Mod;
        return {hsh, len};
    }
};
void solve(int tc)
{
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];
    if (n == 1)
    {
        cout << 1 << nl;
        return;
    }
    string ltr = "", rtl = "";
    for (int i = 1; i < n; i++)
    {
        if (a[i] > a[i - 1])
            ltr += 'U';
        else
            ltr += 'D';
    }
    for (int i = n - 2; i >= 0; i--)
    {
        if (a[i] > a[i + 1])
            rtl += 'U';
        else
            rtl += 'D';
    }

    Hashing h1(ltr), h2(rtl);
    int cnt = 0;
    for (int i = 1; i < n; i++)
    {
        int l = 1, r = n - 1, ans = 0;
        while (l <= r)
        {
            int mid = (l + r) >> 1;
            int l1 = i - mid, r1 = i + mid - 1;
            if (l1 < 0 || r1 >= n - 1)
            {
                r = mid - 1;
                continue;
            }
            int l2 = n - r1 - 2, r2 = n - l1 - 2;
            if (l2 < 0 || r2 >= n - 1)
            {
                r = mid - 1;
                continue;
            }
            // cout << l1 << " " << r1 << " " << l2 << " " << r2 << sp << mid << nl;
            // cout << h1.query(l1 - 1, r1 - 1).hsh << " " << h2.query(l2 - 1, r2 - 1).hsh << nl;
            if (h1.query(l1, r1).hsh == h2.query(l2, r2).hsh)
            {
                ans = mid;
                l = mid + 1;
            }
            else
                r = mid - 1;
        }
        // cout << i << " " << ans << nl;
        // cout << nl;
        cnt += ans;
    }
    cout << cnt + n << nl;
}
signed main(void)
{
    fastio();
    init();
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