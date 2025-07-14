#include <bits/stdc++.h>
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
// 2D segment tree
// 1-based indexing but pass 0-based indexing vector to the constructor
// 1-based indexing for queries and updates
// we are building segement tree of segment trees
// at first we are building segment tree for each row
// then each node in the segment tree of rows will have a segment tree for columns
// example for sum
/*
     1, 2, 3, 4
     5, 6, 7, 8
     1, 2, 3, 4
     5, 6, 7, 8

                                                                                    1, 2, 3, 4
                                                                    6, 8, 10, 12
                                                                                    5, 6, 7, 8
                                                    12, 16, 20, 24
                                                                                    1, 2, 3, 4
                                                                    6, 8, 10, 12
                                                                                    5, 6, 7, 8
    this is the structure of the segment tree on rows
    [12, 16, 20, 24] is the sum of the first 4 rows
    [6, 8, 10, 12] is the sum of row 1 and 2
    [6, 8, 10, 12] is the sum of row 3 and 4
    [1, 2, 3, 4] is the sum of the first row
    [5, 6, 7, 8] is the sum of the second row
    [1, 2, 3, 4] is the sum of the third row
    [5, 6, 7, 8] is the sum of the fourth row
    and each node in the segment tree of rows will have a segment tree for columns
    so if i'm querying the from (row=1, col=2) to (row=2, col=3)
    first i will query for the first 2 rows until i reached the node that contains the segment tree for the first 2 rows
    then i will query in this segment tree from 2 to 3 on columns
    and the result will be the sum of the first 2 rows from 2 to 3
    if i query from row 1 to row 3 so i will reach the node contains sum of the 2 rows 6, 8, 10, 12
    and the node contains the sum of the third row 1, 2, 3, 4 and query from them on columns and merge results
*/
struct SegTree2D
{
    int n, m;
    vector<vector<int>> t;
    int Default = 0; // default value for the segment tree and return value for invalid queries
    SegTree2D(int n, int m) : n(n), m(m), t(4 * n, vector<int>(4 * m, Default)) {}
    SegTree2D(int n, int m, vector<vector<int>> &a) : n(n), m(m), t(4 * n, vector<int>(4 * m, Default)){ build(a); }
    void build_y(int vx, int lx, int rx, int vy, int ly, int ry, vector<vector<int>> &a)
    {
        if (ly == ry)
        {
            if (lx == rx)
            {
                if (lx <= a.size() && ly <= a[0].size())
                    t[vx][vy] = a[lx - 1][ly - 1];
            }
            else
                t[vx][vy] = merge(t[vx * 2][vy], t[vx * 2 + 1][vy]);
        }
        else
        {
            int my = (ly + ry) / 2;
            build_y(vx, lx, rx, vy * 2, ly, my, a);
            build_y(vx, lx, rx, vy * 2 + 1, my + 1, ry, a);
            t[vx][vy] = merge(t[vx][vy * 2], t[vx][vy * 2 + 1]);
        }
    }
    void build_x(int vx, int lx, int rx, vector<vector<int>> &a)
    {
        if (lx != rx)
        {
            int mx = (lx + rx) / 2;
            build_x(vx * 2, lx, mx, a);
            build_x(vx * 2 + 1, mx + 1, rx, a);
        }
        build_y(vx, lx, rx, 1, 1, m, a);
    }
    void build(vector<vector<int>> &a)
    {
        build_x(1, 1, n, a);
    }
    int merge(int a, int b)
    {
        return a + b;
    }
    int query_y(int vx, int vy, int tly, int try_, int ly, int ry)
    {
        if (ly > ry)
            return Default;
        if (ly == tly && try_ == ry)
            return t[vx][vy];
        int tmy = (tly + try_) / 2;
        return merge(query_y(vx, vy * 2, tly, tmy, ly, min(ry, tmy)),
                     query_y(vx, vy * 2 + 1, tmy + 1, try_, max(ly, tmy + 1), ry));
    }
    int query_x(int vx, int tlx, int trx, int lx, int rx, int ly, int ry)
    {
        if (lx > rx)
            return Default;
        if (lx == tlx && trx == rx)
            return query_y(vx, 1, 1, m, ly, ry);
        int tmx = (tlx + trx) / 2;
        return merge(query_x(vx * 2, tlx, tmx, lx, min(rx, tmx), ly, ry),
                     query_x(vx * 2 + 1, tmx + 1, trx, max(lx, tmx + 1), rx, ly, ry));
    }
    int query(int lx, int rx, int ly, int ry)
    {
        return query_x(1, 1, n, lx, rx, ly, ry);
    }
    void update_y(int vx, int lx, int rx, int vy, int ly, int ry, int x, int y, int new_val)
    {
        if (ly == ry)
        {
            if (lx == rx)
                t[vx][vy] = new_val;
            else
                t[vx][vy] = merge(t[vx * 2][vy], t[vx * 2 + 1][vy]);
        }
        else
        {
            int my = (ly + ry) / 2;
            if (y <= my)
                update_y(vx, lx, rx, vy * 2, ly, my, x, y, new_val);
            else
                update_y(vx, lx, rx, vy * 2 + 1, my + 1, ry, x, y, new_val);
            t[vx][vy] = merge(t[vx][vy * 2], t[vx][vy * 2 + 1]);
        }
    }
    void update_x(int vx, int lx, int rx, int x, int y, int new_val)
    {
        if (lx != rx)
        {
            int mx = (lx + rx) / 2;
            if (x <= mx)
                update_x(vx * 2, lx, mx, x, y, new_val);
            else
                update_x(vx * 2 + 1, mx + 1, rx, x, y, new_val);
        }
        update_y(vx, lx, rx, 1, 1, m, x, y, new_val);
    }
    void update(int x, int y, int new_val)
    {
        update_x(1, 1, n, x, y, new_val);
    }
};
void solve(int tc)
{
    int n, m;
    cin >> n >> m;
    vector<vector<int>> a(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> a[i][j];
    SegTree2D st(n, m, a);
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
            st.update(x, y, new_val);
        }
        else
        {
            int r1, c1, r2, c2;
            cin >> r1 >> c1 >> r2 >> c2;
            cout << st.query(r1, r2, c1, c2) << nl;
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