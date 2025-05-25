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
// Fenwich Tree
// x & -x -> lowest bit of x
// 12 & -12 = 1100 & 0100 = 0100
// 8 & -8 = 1000 & 1000 = 1000
// 12 - (12 & -12) = 12 - 4 = 8
// 8 - (8 & -8) = 8 - 8 = 0
// so each number store the sum of last lowest bit numbers
// 12 = 1100 -> 12 - 8 = 4 -> 4 = 100 so 12 store the sum from 9 to 12
// 8 = 1000 -> 8 - 8 = 0 -> 0 = 0 so 8 store the sum from 1 to 8
// when we update index we want to know the numbers greater than it and contains this number in their sum
// add the lowest bit will give us the next number that contains this number in its sum
// 12 + 4 = 16 -> 16 = 10000 so 16 store the sum from 1 to 16
// 16 + 16 = 32 -> 32 = 100000 so 32 store the sum from 1 to 32 and so on
// so we can update the numbers in log(n) time
// and query the sum of numbers in log(n) time

// 1-based Fenwick Tree
//   pass 0-based array to the constructor
//   based 1 update and query
// 0-based Fenwick Tree
//   pass 0-based array to the constructor
//   based 0 update and query

// for range update and point query we use the difference array trick
// add(l, val) , add(r + 1, -val)
template<bool one_based = true>
struct FenwickTree {
    vector<int> bit;
    int n;

    FenwickTree(int n = 0) {
        this->n = n + 1;
        bit.assign(n + 1, 0);
    }

    FenwickTree(vector<int> a)
        : FenwickTree(a.size()) {
        for (size_t i = 0; i < a.size(); i++)
            add(i + one_based, a[i]);
    }

    int sum(int idx) {
        int ret = 0;
        for (idx += (!one_based); idx > 0; idx -= idx & -idx){
            ret += bit[idx];
        }
        return ret;
    }

    int sum(int l, int r) {
        return sum(r) - (l - 1 >= 0 ? sum(l - 1) : 0);
    }

    void add(int idx, int delta) {
        for (idx += (!one_based); idx < n; idx += idx & -idx){
            bit[idx] += delta;
        }
    }

    int get(int idx)
    {
        return sum(idx) - sum(idx - 1);
    }
 
    void set(int idx, int val)
    {
        add(idx, val - get(idx));
    }
    
    void update_range(int l, int r, int val)
    {
        add(l, val);
        add(r + 1, -val);
    }

    int lower_bound(int x) {
        int idx = 0, mask = 1;
        while (mask < n) mask <<= 1;
        for (mask >>= 1; mask > 0; mask >>= 1) {
            if (idx + mask < n && bit[idx + mask] < x) {
                x -= bit[idx + mask];
                idx += mask;
            }
        }
        return idx + one_based;
    }
    int upper_bound(int x) {
        int idx = 0, mask = 1;
        while (mask < n) mask <<= 1;
        for (mask >>= 1; mask > 0; mask >>= 1) {
            if (idx + mask < n && bit[idx + mask] <= x) {
                x -= bit[idx + mask];
                idx += mask;
            }
        }
        return idx + one_based;
    }
};
void solve(int tc)
{
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];
    FenwickTree<1> ft(a);
    while (q--)
    {
        int t;
        cin >> t;
        if (t == 1)
        {
            int k, u;
            cin >> k >> u;
            ft.add(k, u);
            a[k] = u;
        }
        else
        {
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