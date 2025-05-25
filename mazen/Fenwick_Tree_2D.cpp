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
// 2d fenwick tree
// point update rectangle sum
// 1-based query and update
// 0-based query and update
// there is no build function in fenwick tree
// so we can just update the values in the array and call the update function for each value
// it's like the idea of 2d segment tree but we are not building the segment tree
// we have fenwick tree for each row and each column

template <bool one_based = true>
struct FenwickTree2d
{
    vector<vector<int>> bit;
    int n, m;

    FenwickTree2d(int n, int m)
    {
        this->n = n + 1;
        this->m = m + 1;
        bit.assign(n + 1, vector<int>(m + 1, 0));
    }

    void update(int r, int c, int delta)
    {
        for (int x = r + (!one_based); x < n; x += x & -x)
            for (int y = c + (!one_based); y < m; y += y & -y)
                bit[x][y] += delta;
    }

    void set(int r, int c, int new_val)
    {
        int old_val = query(r, c) - query(r - 1, c) - query(r, c - 1) + query(r - 1, c - 1);
        update(r, c, new_val - old_val);
    }

    int query(int r, int c)
    {
        int ret = 0;
        for (int x = r + (!one_based); x > 0; x -= x & -x)
            for (int y = c + (!one_based); y > 0; y -= y & -y)
                ret += bit[x][y];
        return ret;
    }

    int query(int r1, int c1, int r2, int c2)
    {
        return query(r2, c2) - query(r1 - 1, c2) - query(r2, c1 - 1) + query(r1 - 1, c1 - 1);
    }
};
void solve(int tc)
{
    int n, m;
    cin >> n >> m;
    vector<vector<int>> a(n + 1, vector<int>(m + 1));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            cin >> a[i][j];
    FenwickTree2d<true> ft(n, m);
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            ft.update(i, j, a[i][j]);

    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
            cout << ft.query(i, j) << sp;
        cout << nl;
    }
    int q;
    cin >> q;
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            int x, y, new_val;
            cin >> x >> y >> new_val;
            ft.set(x, y, new_val);
        }
        else
        {
            int r1, c1, r2, c2;
            cin >> r1 >> c1 >> r2 >> c2;
            cout << ft.query(r1, c1, r2, c2) << nl;
        }
    }
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
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