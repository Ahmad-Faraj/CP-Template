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

// Euclidean Algorithm
int gcd(int a, int b)
{
    return b == 0 ? a : gcd(b, a % b);
}

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

// Extended Euclidean Algorithm for three numbers
// ax + by + cz = gcd(a, b, c)
// x = x1 * X
// y = y1 * X
// z = Y
// O(log(min(a, b, c)))
int gcd3(int a, int b, int c, int& x, int& y, int& z) {
    int x1, y1;
    int d = gcd(a, b, x1, y1);  // Solve a * x1 + b * y1 = d
    
    int X, Y;
    int g = gcd(d, c, X, Y);  // Solve d * X + c * Y = gcd(a, b, c)
    
    x = x1 * X;  // Final coefficients
    y = y1 * X;
    z = Y;
    
    return g;  // gcd(a, b, c)
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