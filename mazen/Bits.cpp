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
// bit manipulation
#define set_bit(n, i) ((n) | (1 << (i)))
#define clear_bit(n, i) ((n) & ~(1 << (i)))
#define toggle_bit(n, i) ((n) ^ (1 << (i)))
#define check_bit(n, i) ((n) & (1 << (i)))
#define count_bits(n) __builtin_popcount(n)
#define count_bitsll(n) __builtin_popcountll(n)
#define count_set_bits(n) __builtin_popcount(n)
#define count_set_bitsll(n) __builtin_popcountll(n)
#define count_zero_bits(n) (sizeof(n) * 8 - __builtin_popcount(n))
#define count_zero_bitsll(n) (sizeof(n) * 8 - __builtin_popcountll(n))

void solve(int tc)
{
    
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