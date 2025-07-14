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
template <typename T = int>
struct Fenwick_Tree
{

    vector<T> Tree;
    int n;
    T DEFAULT;

    Fenwick_Tree(int N = 0)
    {
        n = N + 1, DEFAULT = 0;
        Tree.assign(n + 10, DEFAULT);
    }

    T operation(T a, T b)
    {
        return a + b;
    }

    int lowest_bit(int idx)
    {
        return (idx & -idx);
    }

    void build(vector<T> &nums)
    {
        for (int i = 0; i < sz(nums); i++)
            add(i, nums[i]);
    }

    void add(int idx, T val)
    {
        idx++;
        while (idx <= n)
        {
            Tree[idx] = operation(Tree[idx], val);
            idx += lowest_bit(idx);
        }
    }

    T get_ans(int idx)
    {
        T ans = DEFAULT;
        idx++;
        while (idx)
        {
            ans = operation(ans, Tree[idx]);
            idx -= lowest_bit(idx);
        }
        return ans;
    }

    T query(int l, int r)
    {
        if (l > r)
            swap(l, r);
        return get_ans(r) - get_ans(l - 1);
    }
};
void solve(int tc)
{
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