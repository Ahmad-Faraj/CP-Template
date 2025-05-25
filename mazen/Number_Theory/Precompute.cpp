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
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;

const int MOD = 1e9 + 7;
const int N = 1e6 + 1;
int fact[N], inv[N];

int power(int a, int b)
{
    int res = 1;
    while (b)
    {
        if (b & 1)
            res = (res * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    return res;
}
// precompute factorial and inverse factorial
// O(n)
// inv[i] = (i!)^-1
// inv[i + 1] = (i + 1)!^-1
// inv[i] = (i + 1) * inv[i + 1] which is equal to i!^-1
void precompute()
{
    fact[0] = 1;
    for (int i = 1; i < N; i++)
        fact[i] = (fact[i - 1] * i) % MOD;
    inv[N - 1] = power(fact[N - 1], MOD - 2);
    for (int i = N - 2; i >= 0; i--)
        inv[i] = (inv[i + 1] * (i + 1)) % MOD;
}

static const int initialize = []() {
    precompute();
    return 0;
}();

int nCr(int n, int r)
{
    if (r > n)
        return 0;
    return (fact[n] * inv[r] % MOD * inv[n - r] % MOD) % MOD;
}

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