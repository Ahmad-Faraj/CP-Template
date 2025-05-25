#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) int(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;
ll max(ll n1, ll n2) { return (n1 >= n2) ? n1 : n2; }
ll min(ll n1, ll n2) { return (n1 >= n2) ? n2 : n1; }
void fastio()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}
// sparse table
// O(nlogn) preprocessing
// O(1) query
// O(nlogn) memory
// sparse table used for range queries
// sparse table can be used for range sum, range min, range max, range gcd, range lcm, range xor, range and, range or
// sparse table can be used for range queries on static array
// sparse table can't be used for update queries
vector<vector<int>> sparse_table;
vector<int> arr;
int LOG = 20;
int n;
void solve(int tc)
{
    cin >> n;
    LOG = log2(n) + 1;
    arr.resize(n);
    sparse_table.resize(n, vector<int>(LOG));
    for (int i = 0; i < n; ++i)
        cin >> arr[i];
    // build sparse table
    for (int i = 0; i < n; ++i)
        sparse_table[i][0] = arr[i];
    for (int j = 1; j < LOG; ++j)
        for (int i = 0; i + (1 << j) <= n; ++i)
            sparse_table[i][j] = min(sparse_table[i][j - 1], sparse_table[i + (1 << (j - 1))][j - 1]);
    /*
    sparse table for range min query
        0 1 2 3 4 5 6 7 8 9
        
        0 0 0 0
        1 1 1 1 
        2 2 2 2 
        3 3 3 
        4 4 4 
        5 5 5 
        6 6 6
        7 7 
        8 8 
        9
    */
    // query
    int q;
    cin >> q;
    while (q--)
    {
        int l, r;
        cin >> l >> r;
        int len = r - l + 1;
        int k = log2(len);
        cout << min(sparse_table[l][k], sparse_table[r - (1 << k) + 1][k]) << nl;
    }
}
int main(void)
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