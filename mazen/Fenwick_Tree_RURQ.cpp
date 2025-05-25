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
// 1-based Fenwick Tree
//   pass 0-based array to the constructor
//   based 1 update and query
// 0-based Fenwick Tree
//   pass 0-based array to the constructor
//   based 0 update and query

// for range update and range query
// 1 2 3 4 5 6 7
// 0 0 0 0 0 0 0  , add 5 from 3 to 5
// 0 0 5 5 5 0 0  as you see here we add 5 to the numbers from 3 to 5
// as we did with point query and range update we can do the same with range query and range update
// add(bit1, l, val), add(bit1, r + 1, -val) but this will not tell you the commulitive of the numbers from l to r
// so add(bit2, l, val * (l - 1)), add(bit2, r + 1, -val * r) this will tell you the commulitive of the numbers from l to r
// so the answer to get a prefix_sum(i) = sum(bit1, i) * i - sum(bit2, i)  
//  0 * i - 0 , i < l
//  x * i - x * (l-1) , l <= i <= r 
//  0 * i - (x * (l-1) - x * r), i > r  -> x * (r - l + 1)
template<bool one_based = true>
struct FenwickTree {
    vector<int> bit1, bit2;
    int n;

    FenwickTree(int n) {
        this->n = n + 2;
        bit1.assign(n + 2, 0);
        bit2.assign(n + 2, 0);
    }

    FenwickTree(vector<int> a)
        : FenwickTree(a.size()) {
        for (size_t i = 0; i < a.size(); i++)
            update(i + one_based, i + one_based, a[i]);
    }

    int sum(vector<int> &bit, int idx) {
        int ret = 0;
        for (idx += (!one_based); idx > 0; idx -= idx & -idx){
            ret += bit[idx];
        }
        return ret;
    }

    int sum(int idx) {
        return sum(bit1, idx) * idx - sum(bit2, idx);
    }

    int sum(int l, int r) {
        return sum(r) - (l - 1 >= 0 ? sum(l - 1) : 0);
    }

    void update(vector<int> &bit, int idx, int delta) {
        for (idx += (!one_based); idx < n; idx += idx & -idx){
            bit[idx] += delta;
        }
    }

    void update(int l, int r, int delta) {
        update(bit1, l, delta);
        update(bit1, r + 1, -delta);
        update(bit2, l, delta * (l - 1));
        update(bit2, r + 1, -delta * r);
    }
};
void solve(int tc)
{
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];
    FenwickTree<true> ft(a);
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 1){
            int l, r, v;
            cin >> l >> r >> v;
            ft.update(l, r, v);
        }
        else{
            int l, r;
            cin >> l >> r;
            cout << ft.sum(l, r) << nl;
        }
    }
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
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