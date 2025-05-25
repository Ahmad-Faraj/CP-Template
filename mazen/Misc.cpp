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
// generate random -> use rnd() -> rnd()%n
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
inline int random(int l, int r)
{
    return rnd() % (r - l + 1) + l;
}

// another way to generate random
mt19937 seed{random_device{}()};
uniform_int_distribution<int> rnd(INT_MIN, INT_MAX);
inline int random()
{
    return rnd(seed);
}
inline int random(int l, int r)
{
    return uniform_int_distribution<int>(l, r)(seed);
}
void solve(int tc)
{
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