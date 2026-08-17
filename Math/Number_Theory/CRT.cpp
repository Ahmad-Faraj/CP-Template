#include<bits/stdc++.h>
using namespace std;
#define ll long long

ll extended_euclid(ll a, ll b, ll &x, ll &y)
{
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll d = extended_euclid(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}
// solves the equation ax + by = c
ll LDE(ll a, ll b, ll c, ll &x, ll &y, ll &found)
{
    ll g = extended_euclid(a, b, x, y);
    if (found = c % g == 0)
        x *= c / g, y *= c / g;
    return g;
}
// solves the system of linear congruences
// x ≡ rems[i] (mod mods[i]) for all i
ll congurence_system(vector<ll> &rems, vector<ll> &mods)
{
    ll rem = rems[0], mod = mods[0];
    for (int i = 1;i < (int)rems.size();i++)
    {
        ll x, y, found, a = mods[i], b = -mods[i], c = rems[i] - rem;
        ll g = LDE(a, b, c, x, y, found);
        if (!found) return -1; // no solution

        rem += mod * x;
        mod = (mod / g) * mods[i];
        rem = (rem % mod + mod) % mod;
    }
    return rem;
}