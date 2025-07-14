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

// Extended Euclidean Algorithm
// ax + by = gcd(a, b)
// x = x1 - y1 * (a / b)
// y = y1
// O(log(min(a, b)))
int extended_gcd(int a, int b, int &x, int &y)
{
    if (b == 0)
    {
        x = 1, y = 0;
        return a;
    }
    int x1, y1;
    int d = extended_gcd(b, a % b, x1, y1);
    x = y1, y = x1 - y1 * (a / b);
    return d;
}


// Modular Inverse
// a and m are coprime
// ax + my = 1
// denote x = a^-1
// take mod m of all sides
// ax = 1 (mod m)
// so x is a^-1 mod m
// O(log(min(a, m)))
int mod_inverse(int a, int m)
{
    int x, y;
    int g = extended_gcd(a, m, x, y);
    if (g != 1)
        return -1;
    return (x % m + m) % m;
}

int gcd(int a, int b, int &x, int &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    int x1, y1;
    int d = gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

bool find_any_solution(int a, int b, int c, int &x0, int &y0, int &g)
{
    g = gcd(abs(a), abs(b), x0, y0);
    if (c % g)
    {
        return false;
    }

    x0 *= c / g;
    y0 *= c / g;
    if (a < 0)
        x0 = -x0;
    if (b < 0)
        y0 = -y0;
    return true;
}




// chinese remainder theorem (CRT)
// x = ai (mod mi)
// first case given set of relative prime mods
// you must use the modular_inverse function that uses the extended euclidean algorithm
// __int128_t can make compile error in some compilers
ll CRT_1(vector<ll> &rems, vector<ll> mods)
{
    __int128_t prod = 1, x = 0;
    for (auto m : mods)
        prod *= m;
    assert(prod <= 1e18);
    for (int i = 0; i < sz(rems); i++)
    {
        __int128_t pp = prod / mods[i];
        x +=  pp * mod_inverse(pp, mods[i]) * rems[i]; // can be overflow
    }
    return x % prod;
}

// General case
// if we can solve 2 cong equation and merge in 1, we can solve sequentially
// T = x mod N   =>  T = x + kN
// T = y mod M   =>  T = y + pM
// x + kN = y + pM  =>  k*N - p*M = y - x => linear diophantine equation
// new mod = lcm(N, M)
// new rem = T mod new mod
// __int128_t can make compile error in some compilers
// O(n * log(min(mi)))
ll CRT_2(vector<ll> &rems, vector<ll> mods){
    __int128_t rem = rems[0], mod = mods[0];
    for (int i = 1; i < rems.size(); i++){
        int x, y, g, a = mod, b = -mods[i], c = rems[i] - rem;
        if(!find_any_solution(a, b, c, x, y, g)){
            return -1;
        }
        rem += x * mod; // T = x + kN
        mod = mod / g * mods[i]; // new mod = lcm(N, M)
        rem = (rem % mod + mod) % mod; // new rem = T mod new mod
    }
    return rem;
}


// you can solve the General case using the first case ?!!
// denote D = gcd(mi for all i)
// if you have n equations then you will make 2 * n equations as follows
// T = (ai mod D) mod D
// T = (ai mod (mi / D)) mod (mi / D)     ,for all i from 1 to n
// so each equation will converted to 2 equations
// now you have 2 * n equations with relative prime mods
// you can solve them using the first case CRT_1



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